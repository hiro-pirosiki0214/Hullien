#include "LaserBeam.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"

CLaserBeam::CLaserBeam()
	: m_TargetPosition	( 0.0f, 0.0f, 0.0f )
	, m_IsInAttack		( false )
	, m_IsEndAttack		( false )
{
}

CLaserBeam::~CLaserBeam()
{
}

// 初期化関数.
bool CLaserBeam::Init()
{
	if( CollisionSetting() == false ) return false; 
	return true;
}

// 更新関数.
void CLaserBeam::Update()
{
	if( m_IsInAttack == false ) return;
}

// 描画関数.
void CLaserBeam::Render()
{
	if( m_IsInAttack == false ) return;

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CLaserBeam::Collision( CActor* pActor )
{
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// スフィアの当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	if( GetAsyncKeyState('M') & 0x0001 )
		pActor->SetParalysisTime( [&]( float& time ){ time = 3; } );
}

// 相手座標の設定関数.
void CLaserBeam::SetTargetPos( CActor& actor )
{
	// プレイヤーじゃなければ終了.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	if( m_IsInAttack == true ) return;
	m_TargetPosition = actor.GetPosition();	// プレイヤーの座標を取得.
}

// 攻撃開始.
void CLaserBeam::Shot()
{
	m_IsInAttack = true;
}

// パラメータを初期に戻す.
void CLaserBeam::ResetParam()
{
	m_IsInAttack	= false;
	m_IsEndAttack	= false;
}

// 当たり判定の設定.
bool CLaserBeam::CollisionSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f, 0.0f, 0.0f },
		1.0f ) )) return false;

	return true;
}