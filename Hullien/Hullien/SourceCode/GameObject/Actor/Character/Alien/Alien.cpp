#include "Alien.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Arm\Arm.h"
#include "..\..\..\..\Common\Effect\EffectManager.h"

CAlien::CAlien()
	: CAlien	( nullptr )
{}

CAlien::CAlien( const SAlienParam* pParam )
	: pPARAMETER				( pParam )
	, m_pArm					( nullptr )
	, m_pAC						( nullptr )
	, m_pEffects				()
	, m_TargetPosition			( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation			( 0.0f, 0.0f, 0.0f )
	, m_KnockBackVector			( 0.0f, 0.0f, 0.0f )
	, m_pAbductUFOPosition		( nullptr )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_NowState				( alien::EAlienState::None )
	, m_NowMoveState			( alien::EMoveState::None )
	, m_HasAnyItem				( EItemList::None )
	, m_LifePoint				( 0.0f )
	, m_DeathScale				( 1.0f )
	, m_DeathCount				( 0.0f )
	, m_KnockBackCount			( 0 )
	, m_WaitCount				( 0 )
	, m_pIsAlienOtherAbduct		( nullptr )
	, m_IsBarrierHit			( false )
	, m_IsRisingMotherShip		( false )
	, m_IsExplosion				( false )
	, m_IsDelete				( false )
{
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_AnimFrameList.resize( alien::EAnimNo_Max );
}

CAlien::~CAlien()
{
}

// エフェクトの描画.
void CAlien::EffectRender()
{
	// ヒット時のエフェクト.
	m_pEffects[alien::EEffectNo_Hit]->SetScale( 2.0f );
	m_pEffects[alien::EEffectNo_Hit]->Render();

	// スポーンエフェクト.
	m_pEffects[alien::EEffectNo_Spawn]->SetLocation( m_vPosition );
	m_pEffects[alien::EEffectNo_Spawn]->Render();

	// 死亡エフェクト.
	m_pEffects[alien::EEffectNo_Dead]->Render();
}

// 相手座標の設定.
void CAlien::SetTargetPos( CActor& actor )
{
	SetGirlPos( actor );
}

// ベクトルの取得.
void CAlien::SetVector( const D3DXVECTOR3& vec )
{
	if( m_NowState == alien::EAlienState::Spawn ) return;
	if( m_NowState == alien::EAlienState::Death ) return;
	m_KnockBackVector = vec;
	m_KnockBackCount = 0;
}

// ライフ計算関数.
void CAlien::LifeCalculation( const std::function<void(float&,bool&)>& proc )
{
	if( m_NowState == alien::EAlienState::Spawn ) return;
	if( m_NowState == alien::EAlienState::Death ) return;
	if( m_NowState == alien::EAlienState::Fright ) return;

	bool isAttack = false;
	proc( m_LifePoint, isAttack );
	m_NowState = alien::EAlienState::Fright;	// 怯み状態へ遷移.
	SetAnimation( alien::EAnimNo_Damage, m_pAC );
	m_AnimSpeed = 0.01;
	m_pEffects[0]->Play( { m_vPosition.x, m_vPosition.y+4.0f, m_vPosition.z });
	if( m_pArm != nullptr ){
		// アームを片付けていなければ片付ける.
		if( m_pArm->IsCleanUp() == false ){
			m_pArm->SetCleanUp();
		}
	}

	if( m_LifePoint > 0.0f ) return;
	// 体力が 0.0以下なら死亡状態へ遷移.
	m_NowState = alien::EAlienState::Death;
	m_pEffects[alien::EEffectNo_Dead]->Play( m_vPosition );
	SetAnimation( alien::EAnimNo_Dead, m_pAC );
}

// 現在の状態の更新関数.
void CAlien::CurrentStateUpdate()
{
	switch( m_NowState )
	{
	case alien::EAlienState::Spawn:
		this->Spawning();
		break;
	case alien::EAlienState::Move:
		this->Move();
		break;
	case alien::EAlienState::Abduct:
		this->Abduct();
		break;
	case alien::EAlienState::KnockBack:
		break;
	case alien::EAlienState::Fright:
		this->KnockBack();
		this->Fright();
		break;
	case alien::EAlienState::Death:
		this->Death();
		break;
	case alien::EAlienState::Escape:
		this->Escape();
		break;
	case alien::EAlienState::RisingMotherShip:
		this->RisingMotherShip();
		break;
	default:
		break;
	}
}

// 女の子の座標を設定.
void CAlien::SetGirlPos( CActor& actor )
{
	if( m_NowMoveState == alien::EMoveState::Move ) return;
	if( *m_pIsAlienOtherAbduct == true ) return;

	// 女の子じゃなければ終了.
	if( actor.GetObjectTag() != EObjectTag::Girl ) return;
	m_TargetPosition = actor.GetPosition();	// 女の子の座標を取得.
	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f( 
		m_vPosition.x - m_TargetPosition.x,
		m_vPosition.z - m_TargetPosition.z );

	// 女の子と一定の距離置いた座標を設定.
	m_TargetPosition.x += sinf( m_TargetRotation.y ) * CArm::GRAB_DISTANCE;
	m_TargetPosition.z += cosf( m_TargetRotation.y ) * CArm::GRAB_DISTANCE;
}

// 座標設定関数.
void CAlien::SetPosition( const D3DXVECTOR3& vPos )
{
	if( *m_pIsAlienOtherAbduct == false ) return;
	m_NowState = alien::EAlienState::RisingMotherShip;
	m_vPosition	= vPos;
}

// 移動ベクトル設定関数.
void CAlien::SetMoveVector( const D3DXVECTOR3& targetPos )
{
	if( m_NowState == alien::EAlienState::Fright ) return;

	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f( 
		m_vPosition.x - targetPos.x,
		m_vPosition.z - targetPos.z );

	// 移動用ベクトルを取得.
	m_MoveVector.x = sinf( m_TargetRotation.y );
	m_MoveVector.z = cosf( m_TargetRotation.y );
}

// 目的の座標へ回転.
void CAlien::TargetRotation()
{
	if( m_NowMoveState != alien::EMoveState::Rotation ) return;

	// 自身のベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_vRotation.y );
	myVector.z = cosf( m_vRotation.y );

	// 目的の座標へ向けて回転.
	if( CCharacter::TargetRotation( m_MoveVector, pPARAMETER->RotationalSpeed, TOLERANCE_RADIAN ) == false ) return;
	m_vRotation.y			= m_TargetRotation.y;	// ターゲットへの回転取得.
	m_BeforeMoveingPosition = m_vPosition;			// 現在の座標を記憶.
	m_NowMoveState			= alien::EMoveState::Move;		// 移動状態へ遷移.
}

// 移動関数.
void CAlien::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != alien::EMoveState::Move ) return;
	
	// ベクトルを使用して移動.
	m_vPosition.x -= m_MoveVector.x * moveSpeed;
	m_vPosition.z -= m_MoveVector.z * moveSpeed;

	// 再度目的の座標を探すか比較.
	// 回転時に記憶した座標と現在の座標の距離が一定以上なら.
	if( D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition-m_vPosition)) >= pPARAMETER->ResearchLenght ) 
		m_NowMoveState = alien::EMoveState::Rotation;	// 回転状態へ移動.

	if( D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition-m_vPosition)) >= 1.0f ) return;
	// 現在の座標と目的の座標の距離が一定以上なら.
	m_NowMoveState = alien::EMoveState::Wait;	// 待機状態へ遷移.
}

// 待機関数.
void CAlien::WaitMove()
{
	if( m_NowMoveState != alien::EMoveState::Wait ) return;
	m_WaitCount++;	// 待機カウント加算.
	if( m_WaitCount < pPARAMETER->WaitTime*FPS ) return;
	m_NowMoveState	= alien::EMoveState::Rotation;	// 移動状態を回転する.
	m_WaitCount		= 0;	// 待機カウントの初期化.
}

// スポーン中.
void CAlien::Spawning()
{
	// スケールの加算.
	m_vScale += 
	{
		pPARAMETER->SpawnScaleAddValue, 
		pPARAMETER->SpawnScaleAddValue,
		pPARAMETER->SpawnScaleAddValue
	};

	// 大きさが一定値以上なら.
	if( m_vScale.x >= SCALE_MAX ){
		m_vScale = { SCALE_MAX, SCALE_MAX, SCALE_MAX };
	}

	m_vPosition.y -= pPARAMETER->SpawnDownSpeed;	// 高さを下げる.
	if( m_vPosition.y <= 0.0f ){
		m_vPosition.y = 0.0f;
	}

	// 高さが一定値より大きければ終了.
	if( m_vScale.x < SCALE_MAX || m_vPosition.y > 0.0f ) return;

	CSoundManager::NoMultipleSEPlay("AlienApp");
	m_AnimSpeed	= 0.01;
	SetAnimation( alien::EAnimNo_Move, m_pAC );
	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Rotation;
}

// 移動.
void CAlien::Move()
{
	TargetRotation();			// 回転.
	CAlien::VectorMove( m_MoveSpeed );	// 移動.
	CAlien::WaitMove();			// 待機.

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == alien::EAlienState::Abduct ) return;
	SetAnimation( alien::EAnimNo_Move, m_pAC );
	m_NowState		= alien::EAlienState::Escape;
	m_NowMoveState	= alien::EMoveState::Rotation;	// 移動状態を回転する.
}

// 拐う.
void CAlien::Abduct()
{
	if( m_IsBarrierHit == true ) return;
	if( m_pArm == nullptr ) return;

	SetMoveVector( *m_pAbductUFOPosition );		// マザーシップの座標とのベクトルを取得.
	m_TargetPosition = *m_pAbductUFOPosition;	// マザーシップの座標を記憶.

	TargetRotation();	// マザーシップの方へ回転.
	CAlien::VectorMove( m_MoveSpeed );	// 移動.

	if( *m_pIsAlienOtherAbduct == true ) return;
	// 女の子を連れ去っていなければ.
	m_NowState		= alien::EAlienState::Move;		// 移動状態へ遷移.
	m_NowMoveState	= alien::EMoveState::Rotation;		// 移動の回転状態へ遷移.
}

// ノックバック.
void CAlien::KnockBack()
{
	m_KnockBackCount++;	// 無敵カウントを加算.
	if( m_KnockBackCount <= pPARAMETER->KnockBackTime ){
		m_vRotation.y = atan2( m_KnockBackVector.x, m_KnockBackVector.z ) + static_cast<float>(D3DX_PI);
		m_vPosition.x -= m_KnockBackVector.x;
		m_vPosition.z -= m_KnockBackVector.z;
	}
}

// 怯み.
void CAlien::Fright()
{
	if( m_AnimFrameList[alien::EAnimNo_Damage].IsNowFrameOver() == false ) return;

	m_KnockBackCount	= 0;	// 無敵カウントの初期化.
	SetAnimation( alien::EAnimNo_Move, m_pAC );
	m_NowState			= alien::EAlienState::Move;	// 移動状態へ遷移.
	m_NowMoveState		= alien::EMoveState::Rotation;	// 移動の回転状態へ遷移.
}

// 死亡.
void CAlien::Death()
{
	if( m_AnimFrameList[alien::EAnimNo_Dead].IsNowFrameOver() == false ) return;

	m_AnimSpeed = 0.0;
	m_DeathCount += pPARAMETER->DeadCountAddValue;
	m_DeathScale -= pPARAMETER->DeadScaleSubValue;
	// モデルのサイズの計算.
	const float scale = m_DeathScale*exp(-m_DeathCount)*sinf(DEATH_SCALE_PI*m_DeathCount);
	m_vScale = { scale, scale, scale };

	// 大きさが一定値以上なら.
	if( m_vScale.x > 0.0f ) return;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	CSoundManager::PlaySE("AlienDead");
	m_IsDelete = true;	// 死亡フラグを立てる.
}

// 逃げる.
void CAlien::Escape()
{
	if( m_IsBarrierHit == true ) return;

	SetMoveVector( *m_pAbductUFOPosition );		// マザーシップの座標とのベクトルを取得.
	m_TargetPosition = *m_pAbductUFOPosition;	// マザーシップの座標を記憶.

	TargetRotation();	// マザーシップの方へ回転.
	CAlien::VectorMove( m_MoveSpeed );	// 移動.

	if( *m_pIsAlienOtherAbduct == true ) return;
	// 女の子を連れ去っていなければ.
	m_NowState		= alien::EAlienState::Move;		// 移動状態へ遷移.
	m_NowMoveState	= alien::EMoveState::Rotation;		// 移動の回転状態へ遷移.
}

// マザーシップに昇っている.
void CAlien::RisingMotherShip()
{
	m_vScale.x -= pPARAMETER->MotherShipUpScaleSubValue;
	m_vScale.y -= pPARAMETER->MotherShipUpScaleSubValue;
	m_vScale.z -= pPARAMETER->MotherShipUpScaleSubValue;
	m_IsRisingMotherShip = true;
	if( m_vScale.x > 0.0f ) return;
	m_IsDelete = true;	// 死亡フラグを立てる.
}

// 女の子との当たり判定.
void CAlien::GirlCollision( CActor* pActor )
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Girl ) return;
	if( m_IsBarrierHit == true ) return;
	if( m_NowMoveState == alien::EMoveState::Attack )	return;	// 攻撃状態は終了.
	if( m_NowState == alien::EAlienState::Spawn )		return;	// スポーン状態なら終了.
	if( m_NowState == alien::EAlienState::Death )		return;	// 死亡していたら終了.
	if( m_NowState == alien::EAlienState::Fright )		return;	// 怯み状態なら終了.

	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	if( m_NowState == alien::EAlienState::Abduct ){
		if( m_AnimFrameList[alien::EAnimNo_Arm].IsNowFrameOver() == true ) m_AnimSpeed = 0.0;
		if( m_IsRisingMotherShip == true ){
			pActor->SetScale( m_vScale );
			m_pArm->SetCleanUpScale( m_vScale );
			m_pArm->SetPosition( { m_vPosition.x, m_vPosition.y+(5.0f*(fabsf(m_vScale.x))), m_vPosition.z } );
			pActor->SetPosition( { m_pArm->GetGrabPosition().x, m_pArm->GetGrabPosition().y-(5.0f*(fabsf(m_vScale.x))), m_pArm->GetGrabPosition().z } );
		} else {
			// 連れ去っている状態なのでアームの座標を設定する.
			pActor->SetPosition( { m_pArm->GetGrabPosition().x, m_pArm->GetGrabPosition().y-5.0f, m_pArm->GetGrabPosition().z } );
		}
		pActor->SetRotationY( m_vRotation.y );
		return;
	} else {
		// 既に連れ去っているか.
		if( *m_pIsAlienOtherAbduct == true ){
			// アームを片付けていなければ片付ける.
			if( m_pArm->IsCleanUp() == false ){
				m_AnimSpeed = 0.01;
				SetAnimation( alien::EAnimNo_Move, m_pAC );
				m_pArm->SetCleanUpPreparation();
			}
			return;
		}
	}
	
	// 掴んでいなければ(アームを取り出してなければ).
	if( m_pArm->IsGrab() == false ){
		m_pArm->SetAppearancePreparation();	// アームを取り出す.
		SetAnimation( alien::EAnimNo_Arm, m_pAC );
		if( m_AnimFrameList[alien::EAnimNo_Arm].IsNowFrameOver() == true ){
			m_AnimSpeed = 0.0;
		}
		return;
	}

	if( m_NowState == alien::EAlienState::Abduct ) return;
	m_NowState		= alien::EAlienState::Abduct;	// 連れ去る状態へ遷移.
	m_NowMoveState	= alien::EMoveState::Rotation;	// 移動を回転へ遷移.
}

// バリアとの当たり判定.
void CAlien::BarrierCollision( CActor* pActor )
{
	if( m_NowState == alien::EAlienState::RisingMotherShip ) return;	// マザーシップに昇っている状態なら終了.
	// オブジェクトのタグがバリアじゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Bariier ) return;
	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == true ){
		CSoundManager::PlaySE("BarrierHit");
		m_MoveSpeed		= BARRIER_HIT_MOVE_SPEED;	// バリア衝突時の移動速度に変更する.
		m_IsBarrierHit	= true;						// バリア衝突フラグを立てる.
		m_NowState		= alien::EAlienState::Move;	// 移動状態へ遷移.
		m_NowMoveState	= alien::EMoveState::Move;	// 移動の移動状態へ遷移.
		*m_pIsAlienOtherAbduct	= false;			// 女の子を連れ去るフラグを下す.
		SetAnimation( alien::EAnimNo_Move, m_pAC );
		m_AnimSpeed = 0.01;
	} else {
		m_MoveSpeed		= pPARAMETER->MoveSpeed;	// 通常の移動速度に戻す.
		m_IsBarrierHit	= false;	// バリア衝突フラグを下す.
	}
}

// アニメーションコントローラーの取得.
bool CAlien::GetAnimationController()
{
	if( m_pSkinMesh == nullptr ) return false;
	// アニメーションコントローラーのクローン作成.
	if( FAILED( m_pSkinMesh->GetAnimController()->CloneAnimationController(
		m_pSkinMesh->GetAnimationController()->GetMaxNumAnimationOutputs(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumAnimationSets(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumTracks(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumEvents(),
		&m_pAC ) )) return false;
	return true;
}

// アニメーションフレームの設定.
bool CAlien::SetAnimFrameList()
{
	if( m_pSkinMesh == nullptr ) return false;
	// 調整用アニメーションフレームのリストを用意.
	const double animAdjFrames[] =
	{
		0.0f,
		0.01f,
		0.0f,
		0.0f,
	};
	if( m_pSkinMesh == nullptr ) return false;
	for( int i = alien::EAnimNo_Begin; i < alien::EAnimNo_End; i++ ){
		m_AnimFrameList.at(i) = { 0.0, m_pSkinMesh->GetAnimPeriod(i)-animAdjFrames[i] };
	}
	return true;
}

// エフェクトの設定.
bool CAlien::EffectSetting()
{
	const char* effectNames[] =
	{
		HIT_EEFECT_NAME,		// ヒットエフェクト.
		SPAWN_EFFECT_NAME,		// スポーンエフェクト.
		DEAD_EFFECT_NAME,		// 死亡エフェクト.
		ATTACK_EFFECT_NAME,		// 攻撃エフェクト.
	};
	const int effectNum = sizeof(effectNames)/sizeof(effectNames[0]);
	// メモリの最大値設定.
	m_pEffects.reserve(effectNum);

	for( int i = 0; i < effectNum; i++ ){
		m_pEffects.emplace_back( std::make_shared<CEffectManager>() );
		if( m_pEffects[i]->SetEffect( effectNames[i] ) == false ) return false;
	}
	return true;
}