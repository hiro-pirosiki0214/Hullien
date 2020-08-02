#include "Barrier.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

CBarrier::CBarrier()
	: m_StaticMesh			( nullptr )
	, m_IsActive			( false )
	, m_ActiveCount			( 0.0f )
	, m_CollSphereRadius	( 0.0f )
#if _DEBUG
	, m_ResizeCollTime		( 0.0f )
#endif	// #if _DEBUG.
{
	m_ObjectTag = EObjectTag::Bariier;
}

CBarrier::~CBarrier()
{
}

// 初期化関数.
bool CBarrier::Init()
{
	if( m_IsActive == true ) return false;
	if( GetModel() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_IsActive			= true;
	m_CollSphereRadius	= 0.0f;
	m_ActiveCount		= 0.0f;

	return true;
}

// 更新関数.
void CBarrier::Update()
{
	if( m_IsActive == false ) return;	// 動作してなければ終了.
	if( m_CollSphereRadius <= COLLISION_SPHERE_RDIUS_MAX ){
		m_CollSphereRadius += COLLISION_SPHERE_RDIUS_ADD_VALUE;	// 当たり半径の加算.
		return;
	}

	m_ActiveCount++;	// 動作カウントの加算.
	if( m_ActiveCount < ACTIVE_TIME*FPS ) return;
	m_IsActive = false;	// 動作しない.
	m_CollSphereRadius	= 0.0f;
	m_ActiveCount		= 0.0f;
	// 当たり判定のサイズを初期化.
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_CollSphereRadius,
		{ 0.0f, 0.0f, 0.0f },
		m_CollSphereRadius );
}

// 描画関数.
void CBarrier::Render()
{
	if( m_IsActive == false ) return;		// 動作してなければ終了.
	if( m_StaticMesh == nullptr ) return;
	if( m_pCollManager == nullptr ) return;

	m_StaticMesh->SetPosition( m_vPosition );
	m_StaticMesh->SetRotation( m_vRotation );
	m_StaticMesh->SetScale( m_CollSphereRadius );
	m_StaticMesh->SetColor( { 0.2f, 0.2f, 1.0f, 0.3f } );
	m_StaticMesh->SetBlend( true );
	m_StaticMesh->Render();
	m_StaticMesh->SetBlend( false );

#if _DEBUG
	m_pCollManager->DebugRender();
	m_ResizeCollTime++;
	if( m_ResizeCollTime < 15.0f ) return;
	m_ResizeCollTime = 0.0f;
#endif	// #if _DEBUG.
	// 当たり判定のサイズを変更.
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_CollSphereRadius,
		{ 0.0f, 0.0f, 0.0f },
		m_CollSphereRadius );
}

// 当たり判定関数.
void CBarrier::Collision( CActor* pActor )
{
	if( m_IsActive == false ) return;
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// 対象オブジェクトじゃなければ終了.
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	pActor->SetTargetPos( *this );
	
}

// 相手座標の設定関数.
void CBarrier::SetTargetPos( CActor& pActor )
{
	if( m_IsActive == false ) return;
	m_vPosition = pActor.GetPosition();
}

// モデルの取得.
bool CBarrier::GetModel()
{
	// 既に読み込めていたら終了.
	if( m_StaticMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetStatic( m_StaticMesh, MODEL_NAME );
	// モデルが読み込めてなければ false.
	if( m_StaticMesh == nullptr ) return false;
	return true;
}

// 当たり判定の設定.
bool CBarrier::ColliderSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	return true;
}