#include "EditAlien.h"
#include "..\..\..\..\..\Common\Effect\EffectManager.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"

CEditAlien::CEditAlien()
	: m_Paramter	()
{}

CEditAlien::~CEditAlien()
{}

// スポーン.
bool CEditAlien::Spawn( const D3DXVECTOR3& spawnPos )
{
	m_vPosition	= spawnPos;						// スポーン座標の設定.
	m_LifePoint	= m_Paramter.LifeMax;			// 体力の設定.
	m_NowState	= alien::EAlienState::Spawn;	// 現在の状態をスポーンに変更.
	m_AnimSpeed	= 0.0;							// アニメーション速度を止める.
	m_pEffects[alien::EEffectNo_Spawn]->Play( m_vPosition );
	return true;
}

// 攻撃の再生.
void CEditAlien::PlayAttack()
{
}

// 怯みの再生.
void CEditAlien::PlayFright()
{
}

// 死亡の再生.
void CEditAlien::PlayDeath()
{
	m_NowState = alien::EAlienState::Death;
	m_pEffects[alien::EEffectNo_Dead]->Play( m_vPosition );
	SetAnimation( alien::EAnimNo_Dead, m_pAC );
}

// マザーシップに上る動作の再生.
void CEditAlien::PlayRisingMotherShip( const D3DXVECTOR3& vPos )
{
	m_NowState = alien::EAlienState::RisingMotherShip;
	m_vPosition	= vPos;
}

// 待機関数.
void CEditAlien::WaitMove()
{
	if( m_NowMoveState != alien::EMoveState::Wait ) return;

	// 移動用ベクトルを取得.
	m_MoveVector.x = sinf( m_vRotation.y );
	m_MoveVector.z = cosf( m_vRotation.y );

	// ベクトルを使用して移動.
	m_vPosition.x -= m_MoveVector.x * m_Paramter.MoveSpeed;
	m_vPosition.z -= m_MoveVector.z * m_Paramter.MoveSpeed;

	// 座標が範囲内なら終了.
	if( MOVE_DISTANCE >= m_vPosition.x && m_vPosition.x >= -MOVE_DISTANCE &&
		MOVE_DISTANCE >= m_vPosition.z && m_vPosition.z >= -MOVE_DISTANCE ) return;
	m_vPosition.x = 0.0f;
	m_vPosition.z = 0.0f;
}

// スポーン中.
void CEditAlien::Spawning()
{
	// スケールの加算.
	m_vScale += 
	{
		m_Paramter.SpawnScaleAddValue, 
		m_Paramter.SpawnScaleAddValue,
		m_Paramter.SpawnScaleAddValue
	};

	// 大きさが一定値以上なら.
	if( m_vScale.x >= SCALE_MAX ){
		m_vScale = { SCALE_MAX, SCALE_MAX, SCALE_MAX };
	}

	m_vPosition.y -= m_Paramter.SpawnDownSpeed;	// 高さを下げる.
	if( m_vPosition.y <= 0.0f ){
		m_vPosition.y = 0.0f;
	}

	// 高さが一定値より大きければ終了.
	if( m_vScale.x < SCALE_MAX || m_vPosition.y > 0.0f ) return;

	CSoundManager::NoMultipleSEPlay("AlienApp");
	m_AnimSpeed	= 0.01;
	SetAnimation( alien::EAnimNo_Move, m_pAC );
	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Wait;
}

// 移動.
void CEditAlien::Move()
{
	TargetRotation();					// 回転.
	CAlien::VectorMove( m_MoveSpeed );	// 移動.
	CAlien::WaitMove();					// 待機.
}

// 拐う.
void CEditAlien::Abduct()
{
}

// ノックバック.
void CEditAlien::KnockBack()
{
	m_KnockBackCount++;	// 無敵カウントを加算.
	if( m_KnockBackCount <= m_Paramter.KnockBackTime ){
		m_vRotation.y = atan2( m_KnockBackVector.x, m_KnockBackVector.z ) + static_cast<float>(D3DX_PI);
		m_vPosition.x -= m_KnockBackVector.x;
		m_vPosition.z -= m_KnockBackVector.z;
	}
}

// 怯み.
void CEditAlien::Fright()
{
	if( m_AnimFrameList[alien::EAnimNo_Damage].IsNowFrameOver() == false ) return;

	m_KnockBackCount	= 0;	// 無敵カウントの初期化.
	SetAnimation( alien::EAnimNo_Move, m_pAC );
	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Wait;
}

// 死亡.
void CEditAlien::Death()
{
	if( m_AnimFrameList[alien::EAnimNo_Dead].IsNowFrameOver() == false ) return;

	m_AnimSpeed = 0.0;
	m_DeathCount += m_Paramter.DeadCountAddValue;
	m_DeathScale -= m_Paramter.DeadScaleSubValue;
	// モデルのサイズの計算.
	const float scale = m_DeathScale*exp(-m_DeathCount)*sinf(DEATH_SCALE_PI*m_DeathCount);
	m_vScale = { scale, scale, scale };

	// 大きさが一定値以上なら.
	if( m_vScale.x > 0.0f ) return;
	m_vScale = { 1.0f, 1.0f, 1.0f };
	CSoundManager::PlaySE("AlienDead");

	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Wait;
}

// 逃げる.
void CEditAlien::Escape()
{
	if( m_IsBarrierHit == true ) return;

	SetMoveVector( *m_pAbductUFOPosition );		// マザーシップの座標とのベクトルを取得.
	m_TargetPosition = *m_pAbductUFOPosition;	// マザーシップの座標を記憶.

	TargetRotation();	// マザーシップの方へ回転.
	CAlien::VectorMove( m_MoveSpeed );	// 移動.

	if( *m_pIsAlienOtherAbduct == true ) return;
	// 女の子を連れ去っていなければ.
	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Wait;
}

// マザーシップに昇っている.
void CEditAlien::RisingMotherShip()
{
	m_vScale.x -= m_Paramter.MotherShipUpScaleSubValue;
	m_vScale.y -= m_Paramter.MotherShipUpScaleSubValue;
	m_vScale.z -= m_Paramter.MotherShipUpScaleSubValue;
	if( m_vScale.x > 0.0f ) return;

	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Wait;
}
