#include "Alien.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Arm\Arm.h"

#define IS_TEMP_MODEL_RENDER

CAlien::CAlien()
	: m_pArm					( nullptr )
	, m_TargetPosition			( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation			( 0.0f, 0.0f, 0.0f )
	, m_pAbductUFOPosition		( nullptr )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_Parameter				()
	, m_NowState				( EAlienState::None )
	, m_NowMoveState			( EMoveState::None )
	, m_HasAnyItem				( EItemList::None )
	, m_LifePoint				( 0.0f )
	, m_ModelAlpha				( 0.0f )
	, m_WaitCount				( 0 )
	, m_pIsAlienOtherAbduct		( nullptr )
	, m_IsExplosion				( false )
	, m_IsDelete				( false )
	, m_IsRisingMotherShip		( false )
{
}

CAlien::~CAlien()
{
}

// 相手座標の設定.
void CAlien::SetTargetPos( CActor& actor )
{
	SetGirlPos( actor );
}

// ライフ計算関数.
void CAlien::LifeCalculation( const std::function<void(float&)>& proc )
{
	if( m_NowState == EAlienState::Spawn ) return;
	if( m_NowState == EAlienState::Death ) return;
	if( m_NowState == EAlienState::Fright ) return;

	proc( m_LifePoint );
	m_NowState = EAlienState::Fright;
	if( m_LifePoint > 0.0f ) return;
	m_NowState = EAlienState::Death;
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
	case EAlienState::Fright:
		this->Fright();
		break;
	case EAlienState::Death:
		this->Death();
		break;
	case EAlienState::Escape:
		this->Escape();
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
		m_TargetPosition.x - m_vPosition.x,
		m_TargetPosition.z - m_vPosition.z );

	// 移動用ベクトルを取得.
	m_TargetPosition.x -= sinf( m_TargetRotation.y ) * CArm::GRAB_DISTANCE;
	m_TargetPosition.z -= cosf( m_TargetRotation.y ) * CArm::GRAB_DISTANCE;
}

// 座標設定関数.
void CAlien::SetPosition( const D3DXVECTOR3& vPos )
{
	if( *m_pIsAlienOtherAbduct == false ) return;
	m_vPosition = vPos;
	m_IsRisingMotherShip = true;
}

// 移動ベクトル設定関数.
void CAlien::SetMoveVector( const D3DXVECTOR3& targetPos )
{
	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f( 
		targetPos.x - m_vPosition.x,
		targetPos.z - m_vPosition.z );

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

	// ベクトルの長さを求める.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(m_MoveVector.x*m_MoveVector.x + m_MoveVector.z*m_MoveVector.z);

	// 目的のベクトルと、自分のベクトルの外積を求める.
	float cross = myVector.x * m_MoveVector.z-myVector.z * m_MoveVector.x;
	float dot = myVector.x * m_MoveVector.x+myVector.z * m_MoveVector.z;
	dot = acosf( dot /( myLenght * targetLenght ));

	// 外積が0.0より少なければ 時計回り : 反時計回り に回転する.
	m_vRotation.y += cross < 0.0f ? ROTATIONAL_SPEED : -ROTATIONAL_SPEED;

	// 内積が許容範囲なら.
	if( -TOLERANCE_RADIAN < dot && dot < TOLERANCE_RADIAN ){
		m_vRotation.y = m_TargetRotation.y;	// ターゲットへの回転取得.
		// 移動用ベクトルを取得.
		m_MoveVector.x = sinf( m_vRotation.y );
		m_MoveVector.z = cosf( m_vRotation.y );
		m_BeforeMoveingPosition = m_vPosition;
		m_NowMoveState = EMoveState::Move;
	}
}

// 移動関数.
void CAlien::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;

	float lenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

	float researchLengh = D3DXVec3Length( &D3DXVECTOR3(m_BeforeMoveingPosition - m_vPosition) );
	if( researchLengh >= m_Parameter.ResearchLenght ) m_NowMoveState = EMoveState::Rotation;

	if( lenght >= 1.0f ) return;

	m_NowMoveState = EMoveState::Wait;
}

// 待機関数.
void CAlien::WaitMove()
{
	if( m_NowMoveState != EMoveState::Wait ) return;
	m_WaitCount++;	// 待機カウント加算.
	if( m_WaitCount < m_Parameter.WaitTime*FPS ) return;
	m_NowMoveState = EMoveState::Rotation;	// 移動状態を回転する.
	m_WaitCount = 0;	// 待機カウントの初期化.
}

// スポーン中.
void CAlien::Spawning()
{
	// モデルのアルファ値を足していく.
	m_ModelAlpha += m_Parameter.ModelAlphaAddValue;
	if( m_ModelAlpha < MODEL_ALPHA_MAX ) return;
	CSoundManager::NoMultipleSEPlay("AlienAppSE");
	m_NowState = EAlienState::Move;
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

	SetMoveVector( *m_pAbductUFOPosition );
	m_TargetPosition = *m_pAbductUFOPosition;

	TargetRotation();
	CAlien::VectorMove( m_MoveSpeed );

	if( *m_pIsAlienOtherAbduct == true ) return;
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// 怯み.
void CAlien::Fright()
{
	m_InvincibleCount++;
	if( IsInvincibleTime( m_Parameter.InvincibleTime+5 ) == false ) return;
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// 死亡.
void CAlien::Death()
{
	m_ModelAlpha -= m_Parameter.ModelAlphaSubValue;
	if( m_ModelAlpha > 0.0f ) return;
	CSoundManager::PlaySE("AlienHit");
	m_IsDelete = true;
}

// 逃げる.
void CAlien::Escape()
{
	if( m_IsBarrierHit == true ) return;

	SetMoveVector( *m_pAbductUFOPosition );
	m_TargetPosition = *m_pAbductUFOPosition;
	TargetRotation();
	CAlien::VectorMove( m_MoveSpeed );
	if( *m_pIsAlienOtherAbduct == true ) return;
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// アルファブレンドの設定.
void CAlien::AlphaBlendSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ){
		if( m_ModelAlpha < 1.0f ){
			m_pSkinMesh->SetBlend( false );
		}
	}
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::GBuffer ){
		m_pSkinMesh->SetBlend( true );
		if( m_ModelAlpha >= 1.0f ){
			m_pSkinMesh->SetBlend( false );
		}
	}
#else
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ){
		if( m_ModelAlpha < 1.0f ){
			m_pTempStaticMesh->SetBlend( false );
		}
	}
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::GBuffer ){
		m_pTempStaticMesh->SetBlend( true );
		if( m_ModelAlpha >= 1.0f ){
			m_pTempStaticMesh->SetBlend( false );
		}
	}
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
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

	bool isAbduct = false;
	if( m_NowState == EAlienState::Abduct ){
		isAbduct = true;
		pActor->SetPosition( m_pArm->GetGrabPosition() );
		return;
	} else {
		if( *m_pIsAlienOtherAbduct == true ){
			if( m_pArm->IsCleanUp() == false ){
				m_pArm->SetCleanUp();
			}
			return;
		}
		isAbduct = true;
	}

	if( isAbduct == false ) return;
	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	
	if( m_pArm->IsGrab() == false ){
		m_pArm->SetAppearance();
		return;
	}
	pActor->SetPosition( m_pArm->GetGrabPosition() );

	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState = EAlienState::Abduct;
	m_NowMoveState = EMoveState::Rotation;
}

// バリアとの当たり判定.
void CAlien::BarrierCollision( CActor* pActor )
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Bariier ) return;
	const float moveSpeed = -2.0f;
	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ){
		m_MoveSpeed = m_Parameter.MoveSpeed;
		m_IsBarrierHit = false;
		
	} else {
		CSoundManager::PlaySE("BarrierHitSE");
		m_MoveSpeed = moveSpeed;
		m_IsBarrierHit = true;
		*m_pIsAlienOtherAbduct = false;
		m_NowState = EAlienState::Move;
		m_NowMoveState = EMoveState::Move;
	}
}