#include "Alien.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Arm\Arm.h"

CAlien::CAlien()
	: m_pArm					( nullptr )
	, m_TargetPosition			( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation			( 0.0f, 0.0f, 0.0f )
	, m_KnockBackVector			( 0.0f, 0.0f, 0.0f )
	, m_pAbductUFOPosition		( nullptr )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_Parameter				()
	, m_NowState				( EAlienState::None )
	, m_NowMoveState			( EMoveState::None )
	, m_HasAnyItem				( EItemList::None )
	, m_LifePoint				( 0.0f )
	, m_DeathScale				( 1.0f )
	, m_DeathCount				( 0.0f )
	, m_KnockBackCount			( 0 )
	, m_WaitCount				( 0 )
	, m_pIsAlienOtherAbduct		( nullptr )
	, m_IsExplosion				( false )
	, m_IsDelete				( false )
{
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_AnimFrameList.resize( EAnimNo_Max );
}

CAlien::~CAlien()
{
}

// 相手座標の設定.
void CAlien::SetTargetPos( CActor& actor )
{
	SetGirlPos( actor );
}
// ベクトルの取得.
void CAlien::SetVector( const D3DXVECTOR3& vec )
{
	if( m_NowState == EAlienState::Spawn ) return;
	if( m_NowState == EAlienState::Death ) return;
	m_KnockBackVector = vec;
	m_KnockBackCount = 0;
}

// ライフ計算関数.
void CAlien::LifeCalculation( const std::function<void(float&,bool&)>& proc )
{
	if( m_NowState == EAlienState::Spawn ) return;
	if( m_NowState == EAlienState::Death ) return;
	if( m_NowState == EAlienState::Fright ) return;

	bool isAttack = false;
	proc( m_LifePoint, isAttack );
	m_NowState = EAlienState::Fright;	// 怯み状態へ遷移.
	SetAnimation( EAnimNo_Damage, m_pAC );
	m_AnimSpeed = 0.01;

	// アームを片付けていなければ片付ける.
	if( m_pArm->IsCleanUp() == false ){
		m_pArm->SetCleanUp();
	}

	if( m_LifePoint > 0.0f ) return;
	// 体力が 0.0以下なら死亡状態へ遷移.
	m_NowState = EAlienState::Death;
	SetAnimation( EAnimNo_Dead, m_pAC );
}

// 現在の状態の更新関数.
void CAlien::CurrentStateUpdate()
{
	switch( m_NowState )
	{
	case EAlienState::Spawn:
		this->Spawning();
		break;
	case EAlienState::Move:
		this->Move();
		break;
	case EAlienState::Abduct:
		this->Abduct();
		break;
	case EAlienState::KnockBack:
		break;
	case EAlienState::Fright:
		this->KnockBack();
		this->Fright();
		break;
	case EAlienState::Death:
		this->Death();
		break;
	case EAlienState::Escape:
		this->Escape();
		break;
	case EAlienState::RisingMotherShip:
		this->RisingMotherShip();
		break;
	default:
		break;
	}
}

// 女の子の座標を設定.
void CAlien::SetGirlPos( CActor& actor )
{
	if( m_NowMoveState == EMoveState::Move ) return;
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
	m_NowState = EAlienState::RisingMotherShip;
	m_vPosition	= vPos;
}

// 移動ベクトル設定関数.
void CAlien::SetMoveVector( const D3DXVECTOR3& targetPos )
{
	if( m_NowState == EAlienState::Fright ) return;

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
	if( m_NowMoveState != EMoveState::Rotation ) return;

	// 自身のベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_vRotation.y );
	myVector.z = cosf( m_vRotation.y );

	// 目的の座標へ向けて回転.
	if( CCharacter::TargetRotation( m_MoveVector, ROTATIONAL_SPEED, TOLERANCE_RADIAN ) == false ) return;
	m_vRotation.y			= m_TargetRotation.y;	// ターゲットへの回転取得.
	m_BeforeMoveingPosition = m_vPosition;			// 現在の座標を記憶.
	m_NowMoveState			= EMoveState::Move;		// 移動状態へ遷移.
}

// 移動関数.
void CAlien::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;
	
	// ベクトルを使用して移動.
	m_vPosition.x -= m_MoveVector.x * moveSpeed;
	m_vPosition.z -= m_MoveVector.z * moveSpeed;

	// 再度目的の座標を探すか比較.
	// 回転時に記憶した座標と現在の座標の距離が一定以上なら.
	if( D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition-m_vPosition)) >= m_Parameter.ResearchLenght ) 
		m_NowMoveState = EMoveState::Rotation;	// 回転状態へ移動.

	if( D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition-m_vPosition)) >= 1.0f ) return;
	// 現在の座標と目的の座標の距離が一定以上なら.
	m_NowMoveState = EMoveState::Wait;	// 待機状態へ遷移.
}

// 待機関数.
void CAlien::WaitMove()
{
	if( m_NowMoveState != EMoveState::Wait ) return;
	m_WaitCount++;	// 待機カウント加算.
	if( m_WaitCount < m_Parameter.WaitTime*FPS ) return;
	m_NowMoveState	= EMoveState::Rotation;	// 移動状態を回転する.
	m_WaitCount		= 0;	// 待機カウントの初期化.
}

// スポーン中.
void CAlien::Spawning()
{
	// スケールの加算.
	m_vScale += { ADD_SCALE_VALUE, ADD_SCALE_VALUE, ADD_SCALE_VALUE };
	// 大きさが一定値以上なら.
	if( m_vScale.x >= SCALE_MAX ){
		m_vScale = { SCALE_MAX, SCALE_MAX, SCALE_MAX };
		m_vPosition.y -= DOWN_SPEED;	// 高さを下げる.
	}

	// 高さが一定値より大きければ終了.
	if( m_vPosition.y > 0.0f ) return;

	CSoundManager::NoMultipleSEPlay("AlienApp");
	m_AnimSpeed	= 0.01;
	SetAnimation( EAnimNo_Move, m_pAC );
	m_NowState	= EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// 移動.
void CAlien::Move()
{
	TargetRotation();			// 回転.
	CAlien::VectorMove( m_MoveSpeed );	// 移動.
	CAlien::WaitMove();			// 待機.

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState		= EAlienState::Escape;
	m_NowMoveState	= EMoveState::Rotation;	// 移動状態を回転する.
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
	m_NowState		= EAlienState::Move;		// 移動状態へ遷移.
	m_NowMoveState	= EMoveState::Rotation;		// 移動の回転状態へ遷移.
}

// ノックバック.
void CAlien::KnockBack()
{
	m_KnockBackCount++;	// 無敵カウントを加算.
	if( m_KnockBackCount <= KNOCK_BACK_TIME ){
		m_vRotation.y = atan2( m_KnockBackVector.x, m_KnockBackVector.z ) + static_cast<float>(D3DX_PI);
		m_vPosition.x -= m_KnockBackVector.x;
		m_vPosition.z -= m_KnockBackVector.z;
	}
}

// 怯み.
void CAlien::Fright()
{
	if( m_AnimFrameList[EAnimNo_Damage].IsNowFrameOver() == false ) return;

	m_KnockBackCount	= 0;	// 無敵カウントの初期化.
	SetAnimation( EAnimNo_Move, m_pAC );
	m_NowState			= EAlienState::Move;	// 移動状態へ遷移.
	m_NowMoveState		= EMoveState::Rotation;	// 移動の回転状態へ遷移.
}

// 死亡.
void CAlien::Death()
{
	if( m_AnimFrameList[EAnimNo_Dead].IsNowFrameOver() == false ) return;

	m_AnimSpeed = 0.0;
	m_DeathCount += DEATH_COUNT_ADD_VALUE;
	m_DeathScale -= DEATH_SCALE_SUB_VALUE;
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
	m_NowState		= EAlienState::Move;		// 移動状態へ遷移.
	m_NowMoveState	= EMoveState::Rotation;		// 移動の回転状態へ遷移.
}

// マザーシップに昇っている.
void CAlien::RisingMotherShip()
{
	m_vScale.x -= RISING_MOTHER_SHIP_SCALE_SUB_VALUE;
	m_vScale.y -= RISING_MOTHER_SHIP_SCALE_SUB_VALUE;
	m_vScale.z -= RISING_MOTHER_SHIP_SCALE_SUB_VALUE;
	if( m_vScale.x > 0.0f ) return;
	m_IsDelete = true;	// 死亡フラグを立てる.
}

// 女の子との当たり判定.
void CAlien::GirlCollision( CActor* pActor )
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Girl ) return;
	if( m_IsBarrierHit == true ) return;
	if( m_NowMoveState == EMoveState::Attack )	return;	// 攻撃状態は終了.
	if( m_NowState == EAlienState::Spawn )		return;	// スポーン状態なら終了.
	if( m_NowState == EAlienState::Death )		return;	// 死亡していたら終了.
	if( m_NowState == EAlienState::Fright )		return;	// 怯み状態なら終了.

	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ){
		// アームを片付けていなければ片付ける.
		if( m_pArm->IsCleanUp() == false ){
			m_AnimSpeed = 0.01;
			SetAnimation( EAnimNo_Move, m_pAC );
			m_pArm->SetCleanUpPreparation();
		}
		m_NowState		= EAlienState::Move;		// 移動状態へ遷移.
		m_NowMoveState	= EMoveState::Rotation;		// 移動の回転状態へ遷移.
		return;
	}

	if( m_NowState == EAlienState::Abduct ){
		// 連れ去っている状態なのでアームの座標を設定する.
		pActor->SetPosition( {m_pArm->GetGrabPosition().x, m_pArm->GetGrabPosition().y-5.5f, m_pArm->GetGrabPosition().z} );
		pActor->SetRotationY( m_vRotation.y );
		return;
	} else {
		// 既に連れ去っているか.
		if( *m_pIsAlienOtherAbduct == true ){
			// アームを片付けていなければ片付ける.
			if( m_pArm->IsCleanUp() == false ){
				m_AnimSpeed = 0.01;
				SetAnimation( EAnimNo_Move, m_pAC );
				m_pArm->SetCleanUpPreparation();
			}
			return;
		}
	}
	
	// 掴んでいなければ(アームを取り出してなければ).
	if( m_pArm->IsGrab() == false ){
		m_pArm->SetAppearancePreparation();	// アームを取り出す.
		SetAnimation( EAnimNo_Arm, m_pAC );
		if( m_AnimFrameList[EAnimNo_Arm].IsNowFrameOver() == true ){
			m_AnimSpeed = 0.0;
		}
		return;
	} else {
		//const float lenght = D3DXVec3Length( &D3DXVECTOR3(pActor->GetPosition()-m_vPosition) );
		//// 目的の座標との距離が一定値より少ないか比較.
		//if( lenght > m_pArm->GRAB_DISTANCE ){
		//	// アームを片付けていなければ片付ける.
		//	if( m_pArm->IsCleanUp() == false ){
		//		m_AnimSpeed = 0.01;
		//		SetAnimation( EAnimNo_Move, m_pAC );
		//		m_pArm->SetCleanUpPreparation();
		//	}
		//	m_NowState		= EAlienState::Move;		// 移動状態へ遷移.
		//	m_NowMoveState	= EMoveState::Rotation;		// 移動の回転状態へ遷移.
		//}
		// アームの座標を設定する.
//		pActor->SetPosition( {m_pArm->GetGrabPosition().x, m_pArm->GetGrabPosition().y-5.5f, m_pArm->GetGrabPosition().z} );
	}

	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState		= EAlienState::Abduct;	// 連れ去る状態へ遷移.
	m_NowMoveState	= EMoveState::Rotation;	// 移動を回転へ遷移.
}

// バリアとの当たり判定.
void CAlien::BarrierCollision( CActor* pActor )
{
	if( m_NowState == EAlienState::RisingMotherShip ) return;	// マザーシップに昇っている状態なら終了.
	// オブジェクトのタグがバリアじゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Bariier ) return;
	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == true ){
		CSoundManager::PlaySE("BarrierHit");
		m_MoveSpeed		= BARRIER_HIT_MOVE_SPEED;	// バリア衝突時の移動速度に変更する.
		m_IsBarrierHit	= true;						// バリア衝突フラグを立てる.
		m_NowState		= EAlienState::Move;		// 移動状態へ遷移.
		m_NowMoveState	= EMoveState::Move;			// 移動の移動状態へ遷移.
		*m_pIsAlienOtherAbduct	= false;			// 女の子を連れ去るフラグを下す.
		SetAnimation( EAnimNo_Move, m_pAC );
		m_AnimSpeed = 0.01;
	} else {
		m_MoveSpeed		= m_Parameter.MoveSpeed;	// 通常の移動速度に戻す.
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
		0.5f,
		0.0f,
		0.0f,
	};
	if( m_pSkinMesh == nullptr ) return false;
	for( int i = EAnimNo_Begin; i < EAnimNo_End; i++ ){
		m_AnimFrameList.at(i) = { 0.0, m_pSkinMesh->GetAnimPeriod(i)-animAdjFrames[i] };
	}
	return true;
}