#include "LaserBeam.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

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
	, m_VertexPoint			()
	, m_VertexCount			( 0 )
{
	m_ObjectTag = EObjectTag::LaserBeam;
}

CLaserBeam::~CLaserBeam()
{
}

// 初期化関数.
bool CLaserBeam::Init()
{
	if( GetModel() == false ) return false;
	if( CollisionSetting() == false ) return false; 
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
	m_pStaticMesh->SetColor( { 1.0f, 0.5f, 0.0f, 1.0f } );
	m_pStaticMesh->Render();

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

	const float modeSpeed = 0.1f;

	m_FrameCount += m_MoveSpeed;
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

	if( m_VertexCount >= 15 ){
		if( m_VertexPoint.size() < 5 ){
			m_VertexPoint.push( 
				{ 
					{ m_vPosition.x, m_vPosition.y+1.0f, m_vPosition.z }, 
					{ m_vPosition.x, m_vPosition.y-1.0f, m_vPosition.z }
				} );
		} else {
			m_VertexPoint.pop();
			m_VertexPoint.push( 
				{ 
					{ m_vPosition.x, m_vPosition.y+1.0f, m_vPosition.z }, 
					{ m_vPosition.x, m_vPosition.y-1.0f, m_vPosition.z }
				} );
		}
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
	//if( m_VertexPoint.size() < 2 ) return;

	//std::vector<D3DXVECTOR3> vertex;
	//for( auto& v : m_VertexPoint ){

	//}
}