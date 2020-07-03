#include "Alien_C.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"

CAlienC::CAlienC()
{
	m_ObjectTag = EObjectTag::Alien_C;
	m_vSclae = { 0.05f, 0.05f, 0.05f };
}

CAlienC::~CAlienC()
{
}

// 初期化関数.
bool CAlienC::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	return true;
}

// 更新関数.
void CAlienC::Update()
{
	SetMoveVector( m_TargetPosition );
	CurrentStateUpdate();	// 現在の状態の更新.
	// 体力が0より多ければ終了.
	if( m_Parameter.Life > 0.0f ) return;
	// 体力がなければ爆発させる.
	m_IsExplosion = true;
}

// 描画関数.
void CAlienC::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.5f, 0.5f, 0.8f, m_ModelAlpha } );
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
void CAlienC::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	GirlCollision( pActor );
}

// スポーン.
bool CAlienC::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
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
void CAlienC::Spawning()
{
	CAlien::Spawning();
}

// 移動.
void CAlienC::Move()
{
	CAlien::Move();
}

// 拐う.
void CAlienC::Abduct()
{
	CAlien::Abduct();
}

// 怯み.
void CAlienC::Fright()
{
	CAlien::Fright();
}

// 死亡.
void CAlienC::Death()
{
	CAlien::Death();
}

// 逃げる.
void CAlienC::Escape()
{
	CAlien::Escape();
}

// 当たり判定の設定.
bool CAlienC::ColliderSetting()
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