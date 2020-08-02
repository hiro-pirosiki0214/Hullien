#include "CollsionManager.h"
#include "..\Colliders\Sphere\SphereCollision.h"
#include "..\Colliders\OBBox\OBBoxCollision.h"
#include "..\Colliders\Capsule\CapsuleCollision.h"
#include "..\..\Global.h"

CCollisionManager::CCollisionManager()
	: m_pSphere		( nullptr )
	, m_pBox		( nullptr )
	, m_pCapsule	( nullptr )
{
}

CCollisionManager::~CCollisionManager()
{
	SAFE_DELETE( m_pSphere );
	SAFE_DELETE( m_pBox );
	SAFE_DELETE( m_pCapsule );
}

//----------------------------------.
// 初期化.
//----------------------------------.
HRESULT CCollisionManager::Init( LPD3DXMESH pMesh, D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, float* pScale )
{
	if( m_pSphere == nullptr ) m_pSphere = new CSphereCollision();
	m_pSphere->SetPosition( pPos );
	m_pSphere->SetRotation( pRot );
	m_pSphere->SetScale( pScale );
	if( FAILED( m_pSphere->InitModelRadius( pMesh ))) return E_FAIL;

	if( m_pBox == nullptr ) m_pBox = new COBBoxCollision();
	m_pBox->SetPosition( pPos );
	m_pBox->SetRotation( pRot );
	m_pBox->SetScale( pScale );
	if( FAILED( m_pBox->InitModelBox( pMesh ))) return E_FAIL;

	if( m_pCapsule == nullptr ) m_pCapsule = new CCapsuleCollision();
	m_pCapsule->SetPosition( pPos );
	m_pCapsule->SetRotation( pRot );
	m_pCapsule->SetScale( pScale );
	if( FAILED( m_pCapsule->InitModelCapsule( pMesh ))) return E_FAIL;

	return S_OK;
}

//-------------------------.
// スフィアの初期化.
//-------------------------.
HRESULT CCollisionManager::InitSphere( 
	LPD3DXMESH pMesh, D3DXVECTOR3* pPos, 
	D3DXVECTOR3* pRot, float* pScale,
	const D3DXVECTOR3& adjPos, const float& adjRadius )
{
	if( m_pSphere == nullptr ) m_pSphere = new CSphereCollision();
	m_pSphere->SetAdjPosition( adjPos );
	m_pSphere->SetAdjRadius( adjRadius );
	m_pSphere->SetPosition( pPos );
	m_pSphere->SetRotation( pRot );
	m_pSphere->SetScale( pScale );
	if( FAILED( m_pSphere->InitModelRadius( pMesh ))) return E_FAIL;
	return S_OK;
}

//-------------------------.
// スフィアの初期化.
//-------------------------.
HRESULT CCollisionManager::InitSphere( 
	D3DXVECTOR3* pPos, 
	D3DXVECTOR3* pRot, float* pScale,
	const D3DXVECTOR3& adjPos, const float& radius )
{
	if( m_pSphere == nullptr ) m_pSphere = new CSphereCollision();
	m_pSphere->SetAdjPosition( adjPos );
	m_pSphere->SetAdjRadius( 0.0f );
	m_pSphere->SetPosition( pPos );
	m_pSphere->SetRotation( pRot );
	m_pSphere->SetScale( pScale );
	if( FAILED( m_pSphere->InitModelRadius( radius ))) return E_FAIL;
	return S_OK;
}

//-------------------------.
// ボックスの初期化.
//-------------------------.
HRESULT CCollisionManager::InitBox( 
	LPD3DXMESH pMesh, D3DXVECTOR3* pPos, 
	D3DXVECTOR3* pRot, float* pScale,
	const D3DXVECTOR3& adjPos, const D3DXVECTOR3& adjlength )
{
	if( m_pBox == nullptr ) m_pBox = new COBBoxCollision();
	m_pBox->SetAdjPosition( adjPos );
	m_pBox->SetAdjLength( adjlength );
	m_pBox->SetPosition( pPos );
	m_pBox->SetRotation( pRot );
	m_pBox->SetScale( pScale );
	if( FAILED( m_pBox->InitModelBox( pMesh ))) return E_FAIL;
	return S_OK;
}

//-------------------------.
// ボックスの初期化.
//-------------------------.
HRESULT CCollisionManager::InitBox( 
	D3DXVECTOR3* pPos, 
	D3DXVECTOR3* pRot, float* pScale,
	const D3DXVECTOR3& adjPos, 
	const float& height,
	const float& wight )
{
	if( m_pBox == nullptr ) m_pBox = new COBBoxCollision();
	m_pBox->SetAdjPosition( adjPos );
	m_pBox->SetAdjLength( { 0.0f, 0.0f, 0.0f } );
	m_pBox->SetPosition( pPos );
	m_pBox->SetRotation( pRot );
	m_pBox->SetScale( pScale );
	if( FAILED( m_pBox->InitModelBox( height, wight ))) return E_FAIL;
	return S_OK;
}

//-------------------------.
// カプセルの初期化.
//-------------------------.
HRESULT CCollisionManager::InitCapsule( 
	LPD3DXMESH pMesh, D3DXVECTOR3* pPos, 
	D3DXVECTOR3* pRot, float* pScale,
	const D3DXVECTOR3& adjPos,
	const float& adjRadius, const float& adjHeight )
{
	if( m_pCapsule == nullptr ) m_pCapsule = new CCapsuleCollision();
	m_pCapsule->SetAdjRadius( adjRadius );
	m_pCapsule->SetAdjHeight( adjHeight );
	m_pCapsule->SetAdjPosition( adjPos );
	m_pCapsule->SetPosition( pPos );
	m_pCapsule->SetRotation( pRot );
	m_pCapsule->SetScale( pScale );
	if( FAILED( m_pCapsule->InitModelCapsule( pMesh ))) return E_FAIL;
	return S_OK;
}

//-------------------------.
// カプセルの初期化.
//-------------------------.
HRESULT CCollisionManager::InitCapsule( 
	D3DXVECTOR3* pPos, 
	D3DXVECTOR3* pRot, float* pScale,
	const D3DXVECTOR3& adjPos,
	const float& radius, const float& height )
{
	if( m_pCapsule == nullptr ) m_pCapsule = new CCapsuleCollision();
	m_pCapsule->SetAdjRadius( 0.0f );
	m_pCapsule->SetAdjHeight( 0.0f );
	m_pCapsule->SetAdjPosition( adjPos );
	m_pCapsule->SetPosition( pPos );
	m_pCapsule->SetRotation( pRot );
	m_pCapsule->SetScale( pScale );
	if( FAILED( m_pCapsule->InitModelCapsule( height, radius ))) return E_FAIL;
	return S_OK;
}

//-------------------------.
// デバッグ用描画.
//-------------------------.
void CCollisionManager::DebugRender()
{
	SphereRender();
	BoxRender();
	CapsuleRender();
}

//----------------------------------.
// スフィア同士の当たり判定.
//----------------------------------.
bool CCollisionManager::IsShereToShere( CCollisionManager* pManager )
{
	if( pManager == nullptr ) return false;
	if( pManager->GetSphere() == nullptr ) return false;

	// 2点間(中心点)の距離を求める.
	const D3DXVECTOR3 vLength = pManager->GetSphere()->GetPosition() - m_pSphere->GetPosition();
	// 長さに変換する.
	const float length = D3DXVec3Length( &vLength );
	if( pManager->GetSphere()->GetRadius() <= 0.0f ) return false;
	if( m_pSphere->GetRadius() <= 0.0f ) return false;
	// 2つの半径の合計.
	float twoRadiusTotal = m_pSphere->GetRadius() + pManager->GetSphere()->GetRadius();
	
	// 2点間の距離が2つの半径の合計より大きいので.
	// 衝突していない.
	if( length >= twoRadiusTotal ){
		m_pSphere->SetChangeColor( false );
		return false;
	}

	m_pSphere->SetChangeColor( true );

	// 衝突している.
	return true;
}

//----------------------------------.
// OBB同士の当たり判定.
//----------------------------------.
bool CCollisionManager::IsOBBToOBB( CCollisionManager* pManager )
{
	if( pManager == nullptr ) return false;
	if( pManager->GetOBB() == nullptr ) return false;

	auto LenSegOnSeparateAxis = []( D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0 )
	{
		// 3つの内積の絶対値の和で投影線分長を計算.
		// 分離軸Sepは標準化されていること.
		FLOAT r1 = fabs(D3DXVec3Dot( Sep, e1 ));
		FLOAT r2 = fabs(D3DXVec3Dot( Sep, e2 ));
		FLOAT r3 = e3 ? (fabs(D3DXVec3Dot( Sep, e3 ))) : 0;
		return r1 + r2 + r3;
	};
	// 各方向ベクトルの確保.
	// （N***:標準化方向ベクトル）.
	D3DXVECTOR3 NAe1 = m_pBox->GetDirection(0), Ae1 = NAe1 * m_pBox->GetLength().x;
	D3DXVECTOR3 NAe2 = m_pBox->GetDirection(1), Ae2 = NAe2 * m_pBox->GetLength().y;
	D3DXVECTOR3 NAe3 = m_pBox->GetDirection(2), Ae3 = NAe3 * m_pBox->GetLength().z;
	D3DXVECTOR3 NBe1 = pManager->GetOBB()->GetDirection(0), Be1 = NBe1 * pManager->GetOBB()->GetLength().x;
	D3DXVECTOR3 NBe2 = pManager->GetOBB()->GetDirection(1), Be2 = NBe2 * pManager->GetOBB()->GetLength().y;
	D3DXVECTOR3 NBe3 = pManager->GetOBB()->GetDirection(2), Be3 = NBe3 * pManager->GetOBB()->GetLength().z;
	D3DXVECTOR3 Interval = m_pBox->GetPosition() - pManager->GetOBB()->GetPosition();

	FLOAT rA, rB, L;

	auto isHitLength = [&](
		const D3DXVECTOR3& e, 
		D3DXVECTOR3 Ne,
		D3DXVECTOR3 e1, D3DXVECTOR3 e2, D3DXVECTOR3 e3 )
	{
		rA = D3DXVec3Length( &e );
		rB = LenSegOnSeparateAxis( &Ne, &e1, &e2, &e3 );
		L = fabs(D3DXVec3Dot( &Interval, &Ne ));
		if( L > rA + rB ){
			m_pBox->SetChangeColor( false );
			return false; // 衝突していない.
		}
		return true;
	};

	auto isHitCross = [&]( 
		const D3DXVECTOR3& NAe, const D3DXVECTOR3& NBe, 
		D3DXVECTOR3 Ae1, D3DXVECTOR3 Ae2,
		D3DXVECTOR3 Be1, D3DXVECTOR3 Be2 )
	{
		D3DXVECTOR3 Cross;
		D3DXVec3Cross( &Cross, &NAe, &NBe );
		rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
		rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
		L = fabs(D3DXVec3Dot( &Interval, &Cross ));
		if( L > rA + rB ){
			m_pBox->SetChangeColor( false );
			return false; // 衝突していない.
		}
		return true;
	};

	// 分離軸 : Ae.
	if( isHitLength( Ae1, NAe1, Be1, Be2, Be3 ) == false ) return false;
	if( isHitLength( Ae2, NAe2, Be1, Be2, Be3 ) == false ) return false;
	if( isHitLength( Ae3, NAe3, Be1, Be2, Be3 ) == false ) return false;
	// 分離軸 : Be.
	if( isHitLength( Be1, NBe1, Ae1, Ae2, Ae3 ) == false ) return false;
	if( isHitLength( Be2, NBe2, Ae1, Ae2, Ae3 ) == false ) return false;
	if( isHitLength( Be3, NBe3, Ae1, Ae2, Ae3 ) == false ) return false;

	// 分離軸 : C1.
	if( isHitCross( NAe1, NBe1, Ae2, Ae3, Be2, Be3 ) == false ) return false;
	if( isHitCross( NAe1, NBe2, Ae2, Ae3, Be1, Be3 ) == false ) return false;
	if( isHitCross( NAe1, NBe3, Ae2, Ae3, Be1, Be2 ) == false ) return false;
	// 分離軸 : C2.
	if( isHitCross( NAe2, NBe1, Ae1, Ae3, Be2, Be3 ) == false ) return false;
	if( isHitCross( NAe2, NBe2, Ae1, Ae3, Be1, Be3 ) == false ) return false;
	if( isHitCross( NAe2, NBe3, Ae1, Ae3, Be1, Be2 ) == false ) return false;
	// 分離軸 : C3.
	if( isHitCross( NAe3, NBe1, Ae1, Ae2, Be2, Be3 ) == false ) return false;
	if( isHitCross( NAe3, NBe2, Ae1, Ae2, Be1, Be3 ) == false ) return false;
	if( isHitCross( NAe3, NBe3, Ae1, Ae2, Be1, Be2 ) == false ) return false;

	// 分離平面が存在しないので「衝突している」.
	m_pBox->SetChangeColor( true );
	return true;
}

//----------------------------------.
// カプセル同士の当たり判定.
//----------------------------------.
bool CCollisionManager::IsCapsuleToCapsule( CCollisionManager* pManager )
{
	if( pManager == nullptr ) return false;
	if( pManager->GetCapsule() == nullptr ) return false;

	Point p1, p2;
	float t1, t2;
	Segment c1 = m_pCapsule->GetSegment();
	Segment c2 = pManager->GetCapsule()->GetSegment();
	float d = m_pCapsule->calcSegmentSegmentDist( c1, c2, p1, p2, t1, t2 );
	if( d <= m_pCapsule->GetRadius() + pManager->GetCapsule()->GetRadius() == false ){
		m_pCapsule->SetChangeColor( false );
		return false;
	}
	m_pCapsule->SetChangeColor( true );
	return true;
}

//----------------------------------.
// ↓ 調整用 ↓.
//----------------------------------.
//----------------------------------.
// スフィアの調整用値設定関数.
//----------------------------------.
void CCollisionManager::SetSphereAdjPosition( const D3DXVECTOR3& pos )
{
	if( m_pSphere == nullptr ) return;
	m_pSphere->SetAdjPosition( pos );
}
//----------------------------------.
// スフィアの調整用半径設定.
//----------------------------------.
void CCollisionManager::SetSphereAdjRadius( const float& radius )
{
	if( m_pSphere == nullptr ) return;
	m_pSphere->SetAdjRadius( radius );
}
//----------------------------------.
// ボックスの調整用座標設定.
//----------------------------------.
void CCollisionManager::SetBoxAdjPosition( const D3DXVECTOR3& pos )
{
	if( m_pBox == nullptr ) return;
	m_pBox->SetAdjPosition( pos );
}
//----------------------------------.
// ボックスの調整用半径設定.
//----------------------------------.
void CCollisionManager::SetBoxAdjLength( const D3DXVECTOR3& length )
{
	if( m_pBox == nullptr ) return;
	m_pBox->SetAdjLength( length );
}

//----------------------------------.
// スフィアの描画.
//----------------------------------.
void CCollisionManager::SphereRender()
{
	if( m_pSphere == nullptr ) return;
	m_pSphere->DebugRender();
}
//----------------------------------.
// ボックスの描画.
//----------------------------------.
void CCollisionManager::BoxRender()
{
	if( m_pBox == nullptr ) return;
	m_pBox->DebugRender();
}
//----------------------------------.
// カプセルの描画.
//----------------------------------.
void CCollisionManager::CapsuleRender()
{
	if( m_pCapsule == nullptr ) return;
	m_pCapsule->DebugRender();
}