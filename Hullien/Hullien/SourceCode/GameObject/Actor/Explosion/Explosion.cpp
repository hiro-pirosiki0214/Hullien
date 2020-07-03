#include "Explosion.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

CExplosion::CExplosion()
	: m_Param			()
{
}

CExplosion::~CExplosion()
{
}

// 初期化関数.
bool CExplosion::Init()
{
	const char* MODEL_NAME = "GhostA";	// モデル名(後で消す予定).
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	return true;
}

// 更新関数.
void CExplosion::Update()
{
}

// 描画関数.
void CExplosion::Render()
{
	m_Param.ExplosionTime += 0.1f;
	if( m_Param.ExplosionTime >= 1.0f )return;
#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CExplosion::Collision( CActor* pActor )
{
	// エフェクトの描画をする予定.
	
	if( m_Param.ExplosionTime >= 1.0f )return;
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// 対象オブジェクトじゃなければ終了,
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	// 攻撃関数.
	auto attackProc = [&]( float& life ){ life -= m_Param.AttackPower; };
	pActor->LifeCalculation( attackProc );	// 対象の体力を減らす.
}

// モデルの取得.
bool CExplosion::GetModel( const char* modelName )
{
	// 既に読み込めていたら終了.
	if( m_pStaticMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetStatic( m_pStaticMesh, modelName );
	// モデルが読み込めてなければ false.
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}

// 当たり判定の設定.
bool CExplosion::ColliderSetting()
{
	if( m_pStaticMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Param.SphereAdjPos,
		m_Param.SphereAdjRadius ) )) return false;
	return true;
}