#include "Explosion.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Common\Effect\EffectManager.h"
#include "..\..\..\Resource\EffectResource\EffectResource.h"

CExplosion::CExplosion()
	: m_pEffect				( nullptr )
	, m_Param				()
	, m_CollSphereRadius	( 0.0f )
	, m_IsEffectPlay		( false )
#if _DEBUG
	, m_ResizeCollTime		( 0.0f )
#endif	// #if _DEBUG.
{
	m_pEffect = std::make_shared<CEffectManager>();
}

CExplosion::~CExplosion()
{
}

// 初期化関数.
bool CExplosion::Init()
{
	if( EffectSetting() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_CollSphereRadius = 0.0f;
	m_IsEffectPlay = false;
	return true;
}

// 更新関数.
void CExplosion::Update()
{
}

// 描画関数.
void CExplosion::Render()
{
	// エフェクトの再生終わっていれば終了,
	if( m_IsEffectPlay == false ) return;

	// 当たり判定が最大当たり判定より大きければ終了.
	if( m_CollSphereRadius >= m_Param.SphereMaxRadius )return;
	if( m_pCollManager == nullptr ) return;
	m_CollSphereRadius += m_Param.ExplosionSpeed;	// 当たり判定を大きくする.

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
		&m_vScale.x,
		m_Param.SphereAdjPos,
		m_CollSphereRadius );
}

// エフェクトの描画.
void CExplosion::EffectRender()
{
	// エフェクトの再生終わっていれば終了,
	if( m_IsEffectPlay == false ) return;
	// エフェクトを描画.
	m_pEffect->SetScale( 1.0f );
	m_pEffect->Render();
}

// 当たり判定関数.
void CExplosion::Collision( CActor* pActor )
{
	if( m_CollSphereRadius >= m_Param.SphereMaxRadius )return;
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

	// 対象の体力を減らす.
	pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	{ 
		life -= m_Param.AttackPower; 
		isAttack = true;
	});
}

// 相手座標の設定関数.
void CExplosion::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();	// 座標を設定.
	// すでにエフェクトを再生していれば終了.
	if( m_IsEffectPlay == true ) return;
	m_pEffect->Play( m_vPosition );	// エフェクトを再生.
	m_IsEffectPlay = true;
}

void CExplosion::SetPosition( const D3DXVECTOR3& vPos )
{
	m_vPosition = vPos;	// 座標を設定.
	// すでにエフェクトを再生していれば終了.
	if( m_IsEffectPlay == true ) return;
	m_pEffect->Play( m_vPosition );	// エフェクトを再生.
	m_IsEffectPlay = true;
}

// 止まっているかどうか.
bool CExplosion::IsStop()
{
	if( m_CollSphereRadius < m_Param.SphereMaxRadius ) return false;
	if( m_pEffect->IsExists() == true ) return false;
	m_IsEffectPlay = false;
	return true;
}

// エフェクトの設定.
bool CExplosion::EffectSetting()
{
	if( m_pEffect->SetEffect( EFFECT_NAME ) == false ) return false;
	return true;
}

// 当たり判定の設定.
bool CExplosion::ColliderSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Param.SphereAdjPos,
		0.0f );
	return true;
}