#include "Alien_A.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"

CAlienA::CAlienA()
{
	m_vSclae = { 0.05f, 0.05f, 0.05f };
}

CAlienA::~CAlienA()
{
}

// 初期化関数.
bool CAlienA::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	return true;
}

// 更新関数.
void CAlienA::Update()
{
	SetMoveVector( m_TargetPosition );
	CurrentStateUpdate();	// 現在の状態の更新.
} 

// 描画関数.
void CAlienA::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.5f, 0.8f, 0.5f, m_ModelAlpha } );
	m_pSkinMesh->SetBlend( true );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CAlienA::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	GirlCollision( pActor );
}

// スポーン.
bool CAlienA::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if( m_NowState != EAlienState::None ) return true;
	// 初期化に失敗したら終了.
	if( Init() == false ) return false;
	m_Parameter = param;	// パラメータを設定.
	m_vPosition = spawnPos;	// スポーン座標の設定.
	m_NowState = EAlienState::Spawn;	// 現在の状態をスポーンに変更.

	return true;
}

// スポーン.
void CAlienA::Spawning()
{
	// モデルのアルファ値を足していく.
	m_ModelAlpha += m_Parameter.ModelAlphaAddValue;
	if( m_ModelAlpha < MODEL_ALPHA_MAX ) return;
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// 移動.
void CAlienA::Move()
{
	switch( m_NowMoveState )
	{
	case EMoveState::Rotation:
		TargetRotation();
		break;
	case EMoveState::Move:
		CAlien::VectorMove( 0.1f );
		break;
	case EMoveState::Attack:
		break;
	case EMoveState::Wait:
		m_WaitCount++;	// 待機カウント加算.
		if( m_WaitCount < m_Parameter.WaitTime*FPS ) return;
		m_NowMoveState = EMoveState::Rotation;	// 移動状態を回転する.
		m_WaitCount = 0;	// 待機カウントの初期化.
		break;
	}
	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState		= EAlienState::Escape;
	m_NowMoveState	= EMoveState::Rotation;	// 移動状態を回転する.
}

// 拐う.
void CAlienA::Abduct()
{
	SetMoveVector( *m_pAbductUFOPosition );
	m_TargetPosition = *m_pAbductUFOPosition;
	TargetRotation();
	CAlien::VectorMove( 0.1f );
	if( *m_pIsAlienOtherAbduct == true ) return;
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// 怯み.
void CAlienA::Fright()
{
	m_InvincibleCount++;
	if( IsInvincibleTime( m_Parameter.InvincibleTime+5 ) == false ) return;
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// 死亡.
void CAlienA::Death()
{
	m_ModelAlpha -= m_Parameter.ModelAlphaSubValue;
	if( m_ModelAlpha > 0.0f ) return;
	m_IsDelete = true;
}

// 逃げる.
void CAlienA::Escape()
{
	SetMoveVector( *m_pAbductUFOPosition );
	m_TargetPosition = *m_pAbductUFOPosition;
	TargetRotation();
	CAlien::VectorMove( 0.1f );
	if( *m_pIsAlienOtherAbduct == true ) return;
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// 女の子との当たり判定.
void CAlienA::GirlCollision( CActor* pActor )
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if( m_NowState == EAlienState::Death ) return;	// 死亡していたら終了.
	if( m_NowState == EAlienState::Fright ) return;	// 怯み状態なら終了.

	bool isAbduct = false;
	if( m_NowState == EAlienState::Abduct ){
		isAbduct = true;
	} else {
		if( *m_pIsAlienOtherAbduct == true ) return;
		isAbduct = true;
	}

	if( isAbduct == false ) return;
	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	pActor->SetTargetPos( *this );

	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState = EAlienState::Abduct;
	m_NowMoveState = EMoveState::Rotation;
}

// 当たり判定の設定.
bool CAlienA::ColliderSetting()
{
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
}