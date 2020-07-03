#include "Explosion.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

CExplosion::CExplosion()
	: m_Param				()
	, m_CollSphereRadius	( 0.0f )
{
}

CExplosion::~CExplosion()
{
}

// 初期化関数.
bool CExplosion::Init()
{
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
	m_CollSphereRadius += m_Param.ExplosionSpeed;
	if( m_CollSphereRadius >= m_Param.SphereMaxRadius )return;
	if( m_pCollManager == nullptr ) return;
	// 当たり判定のサイズを変更.
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Param.SphereAdjPos,
		m_CollSphereRadius );
#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CExplosion::Collision( CActor* pActor )
{
	// エフェクトの描画をする予定.
	
	if( m_CollSphereRadius >= m_Param.SphereMaxRadius )return;
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

// 当たり判定の設定.
bool CExplosion::ColliderSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	return true;
}