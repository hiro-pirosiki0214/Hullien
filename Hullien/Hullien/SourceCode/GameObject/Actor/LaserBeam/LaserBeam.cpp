#include "LaserBeam.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Common\Shader\Trajectory\Trajectory.h"

CLaserBeam::CLaserBeam()
	: m_pStaticMesh			( nullptr )
	, m_MoveSpeed			( DEFAULT_MOVE_SPEED )
	, m_ParalysisTime		( DEFAULT_PARALYSIS_TIME )
	, m_TargetPosition		( 0.0f, 0.0f, 0.0f )
	, m_IsInAttack			( false )
	, m_IsEndAttack			( true )
	, m_FrameCount			( 0.0f )
	, m_FrameTime			( 1.0f )
	, m_InitPosition		( 0.0f, 0.0f, 0.0f )
	, m_ControlPointList	()
	, m_VertexPointHeight	()
	, m_VertexPointWidth	()
	, m_VertexCount			( 0 )
	, m_pTrajectory			( nullptr )
{
	m_ObjectTag = EObjectTag::LaserBeam;
	m_pTrajectory = std::make_unique<CTrajectory>();
}

CLaserBeam::~CLaserBeam()
{
	m_pTrajectory->Release();
}

// 初期化関数.
bool CLaserBeam::Init()
{
	if( GetModel() == false ) return false;
	if( CollisionSetting() == false ) return false; 
	if( FAILED( m_pTrajectory->Init( nullptr, nullptr ) ) ) return false;
	return true;
}

// 更新関数.
void CLaserBeam::Update()
{
	if( m_IsInAttack == false ) return;

	m_VertexCount++;
	// 操作座標のサイズで比較.
	switch( m_ControlPointList.size() )
	{
	case 0:
		break;
	case 1:
		// 二次元ベジェ曲線.
		SecondaryBeziercurve();
		break;
	case 2:
		// 三次元ベジェ曲線.
		ThirdBezierCurve();
		break;
	default:
		break;
	}
	CreateVertex();
	// カウントがタイム以上になればショットフラグを下す.
	if( m_FrameCount >= m_FrameTime ){
		m_IsInAttack = false;
		m_IsEndAttack = true;
	}
}

// 描画関数.
void CLaserBeam::Render()
{
	if( m_IsInAttack == false ) return;
	if( m_pStaticMesh == nullptr ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetScale( 1.0f );
	m_pStaticMesh->SetColor( { 0.0f, 0.0f, 1.0f, 1.0f } );
	m_pStaticMesh->Render();

	if( m_VertexPointHeight.size() >= 2 ){
		m_pStaticMesh->SetBlend( true );
		m_pTrajectory->Render();
		m_pStaticMesh->SetBlend( false );
	}

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CLaserBeam::Collision( CActor* pActor )
{
	if( m_IsInAttack == false ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// スフィアの当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	
	// プレイヤーを麻痺にさせる.
	pActor->SetParalysisTime( [&]( float& time ){ time = m_ParalysisTime; } );
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
void CLaserBeam::Shot( const D3DXVECTOR3& pos )
{
	if( m_IsInAttack == true ) return;
	m_IsInAttack	= true;
	m_IsEndAttack	= false;
	m_vPosition		= pos;
	m_InitPosition	= pos;
	m_FrameCount	= 0.0f;
	m_VertexCount	= 0;
	m_VertexPointHeight.clear();
	m_VertexPointWidth.clear();
}

// パラメータを初期に戻す.
void CLaserBeam::ResetParam()
{
	m_IsEndAttack	= false;
}

// ベジェ曲線の制御点の設定.
void CLaserBeam::SetControlPointList( std::vector<D3DXVECTOR3> pointList )
{
	m_ControlPointList = pointList; 
}

// 二次ベジェ曲線.
void CLaserBeam::SecondaryBeziercurve()
{
	if( m_FrameCount >= m_FrameTime ) return;

	float a = m_FrameCount / m_FrameTime;
	float b = m_FrameTime - a;

	D3DXVECTOR3 point[2];

	point[0].x = b * m_InitPosition.x + a * m_ControlPointList[0].x;
	point[0].y = b * m_InitPosition.y + a * m_ControlPointList[0].y;
	point[0].z = b * m_InitPosition.z + a * m_ControlPointList[0].z;

	point[1].x = b * m_ControlPointList[0].x + a * m_TargetPosition.x;
	point[1].y = b * m_ControlPointList[0].y + a * m_TargetPosition.y;
	point[1].z = b * m_ControlPointList[0].z + a * m_TargetPosition.z;

	m_vPosition.x = b * point[0].x + a * point[1].x;
	m_vPosition.y = b * point[0].y + a * point[1].y;
	m_vPosition.z = b * point[0].z + a * point[1].z;

	m_FrameCount += m_MoveSpeed;

	if( m_VertexCount >= TRAJECTORY_TIME_COUNT ){
		// 頂点数が一定以上なら、先頭の座標を取り出す.
		if( static_cast<int>(m_VertexPointHeight.size()) > MAX_TRAJECTORY_COUNT ){
			m_VertexPointHeight.pop_front();	// 高さ.
			m_VertexPointWidth.pop_front();		// 幅.
		}
		// 高さ.
		m_VertexPointHeight.emplace_back(
			D3DXVECTOR3( m_vPosition.x, m_vPosition.y+1.0f, m_vPosition.z ),
			D3DXVECTOR3( m_vPosition.x, m_vPosition.y-1.0f, m_vPosition.z ));
		// 幅.
		m_VertexPointWidth.emplace_back(
			D3DXVECTOR3( m_vPosition.x+1.0f, m_vPosition.y, m_vPosition.z+1.0f ),
			D3DXVECTOR3( m_vPosition.x-1.0f, m_vPosition.y, m_vPosition.z-1.0f ));
		m_VertexCount = 0;
	}
}

// 三次ベジェ曲線.
void CLaserBeam::ThirdBezierCurve()
{
	if( m_FrameCount >= m_FrameTime ) return;

	float a = m_FrameCount / m_FrameTime;
	float b = m_FrameTime - a;

	D3DXVECTOR3 point[3];

	point[0].x = b * m_InitPosition.x + a * m_ControlPointList[0].x;
	point[0].y = b * m_InitPosition.y + a * m_ControlPointList[0].y;
	point[0].z = b * m_InitPosition.z + a * m_ControlPointList[0].z;

	point[1].x = b * m_ControlPointList[0].x + a * m_ControlPointList[1].x;
	point[1].y = b * m_ControlPointList[0].y + a * m_ControlPointList[1].y;
	point[1].z = b * m_ControlPointList[0].z + a * m_ControlPointList[1].z;

	point[2].x = b * m_ControlPointList[1].x + a * m_TargetPosition.x;
	point[2].y = b * m_ControlPointList[1].y + a * m_TargetPosition.y;
	point[2].z = b * m_ControlPointList[1].z + a * m_TargetPosition.z;

	D3DXVECTOR3 p[2];

	p[0].x = b * point[0].x + a * point[1].x;
	p[0].y = b * point[0].y + a * point[1].y;
	p[0].z = b * point[0].z + a * point[1].z;

	p[1].x = b * point[1].x + a * point[2].x;
	p[1].y = b * point[1].y + a * point[2].y;
	p[1].z = b * point[1].z + a * point[2].z;

	m_vPosition.x = b * p[0].x + a * p[1].x;
	m_vPosition.y = b * p[0].y + a * p[1].y;
	m_vPosition.z = b * p[0].z + a * p[1].z;

	m_FrameCount += m_MoveSpeed;

	if( m_VertexCount >= TRAJECTORY_TIME_COUNT ){
		// 頂点数が一定以上なら、先頭の座標を取り出す.
		if( static_cast<int>(m_VertexPointHeight.size()) > MAX_TRAJECTORY_COUNT ){
			m_VertexPointHeight.pop_front();	// 高さ.
			m_VertexPointWidth.pop_front();		// 幅.
		}
		// 高さ.
		m_VertexPointHeight.emplace_back(
			D3DXVECTOR3( m_vPosition.x, m_vPosition.y+1.0f, m_vPosition.z ),
			D3DXVECTOR3( m_vPosition.x, m_vPosition.y-1.0f, m_vPosition.z ));
		// 幅.
		m_VertexPointWidth.emplace_back(
			D3DXVECTOR3( m_vPosition.x+1.0f, m_vPosition.y, m_vPosition.z+1.0f ),
			D3DXVECTOR3( m_vPosition.x-1.0f, m_vPosition.y, m_vPosition.z-1.0f ));
		m_VertexCount = 0;
	}
}

// モデルの取得.
bool CLaserBeam::GetModel()
{
	if( m_pStaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME );
	if( m_pStaticMesh == nullptr ) return false;
	return true;
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

void CLaserBeam::CreateVertex()
{
	if( m_VertexPointHeight.size() < 2 ) return;

	m_pTrajectory->CreateVertexBuffer( m_VertexPointWidth, m_VertexPointHeight );
}