#include "CapsuleCollision.h"
#include "CapsuleModel.h"


#ifdef _DEBUG
bool CCapsuleCollision::m_IsRender = false;
#endif	// #ifdef _DEBUG.

CCapsuleCollision::CCapsuleCollision()
	: m_Segment			()
	, m_Radius			( 0.0f )
	, m_AdjRadius		( 0.0f )
	, m_Height			( 0.0f )
	, m_AdjHeight		( 0.0f )
#ifdef _DEBUG
	, m_pDebugCapsule	( nullptr )
#endif	// #ifdef _DEBUG.
{
	m_pDebugCapsule = std::make_unique<CCapsuleModel>();
}

CCapsuleCollision::~CCapsuleCollision()
{
}

//----------------------------------.
// モデルの半径の作成.
//----------------------------------.
HRESULT CCapsuleCollision::InitModelCapsule( LPD3DXMESH pMesh )
{
	GetBoundingSphere( pMesh, m_Radius );
	D3DXVECTOR3 Max, Min;
	GetBoundingBox( pMesh, Max, Min );

	m_Radius = (m_Radius * (*m_pScale)) + m_AdjRadius;
	m_Height = ((fabsf( Max.y - Min.y ) * (*m_pScale) * 0.5f )) + m_AdjHeight;
	// 半径が0.0fより小さいとだめなので、0.0より少し大きくする.
	m_Radius = m_Radius <= 0.0f ? 0.001f : m_Radius;
	// 高さより半径のほうが大きいとカプセルがつぶれるので、
	// 高さに半径+1.0fを入れて調整する.
	m_Height = m_Radius >= m_Height ? m_Radius + 0.001f : m_Height;
#ifdef _DEBUG
	if( FAILED( m_pDebugCapsule->Init( m_Radius, m_Height ))){
		ERROR_MESSAGE("Model creation failed");
		return E_FAIL;
	}
#endif	// #ifdef _DEBUG.
	return S_OK;
}

//----------------------------------.
// モデルの半径の作成.
//----------------------------------.
HRESULT CCapsuleCollision::InitModelCapsule( const float& height, const float& radius )
{
	m_Radius = radius;
	m_Height = height;
	m_Radius = m_Radius <= 0.0f ? 0.001f : m_Radius;
	// 高さより半径のほうが大きいとカプセルがつぶれるので、
	// 高さに半径+1.0fを入れて調整する.
	m_Height = m_Radius >= m_Height ? m_Radius + 0.001f : m_Height;
#ifdef _DEBUG
	if( FAILED( m_pDebugCapsule->Init( m_Radius, m_Height ))){
		ERROR_MESSAGE("Model creation failed");
		return E_FAIL;
	}
#endif	// #ifdef _DEBUG.
	return S_OK;
}

//----------------------------------.
// 当たり判定の表示.
//----------------------------------.
void CCapsuleCollision::DebugRender()
{
#ifdef _DEBUG
	if( GetAsyncKeyState(VK_F3) & 0x0001 ) m_IsRender = !m_IsRender;
	if( m_IsRender == false ) return;
	m_pDebugCapsule->SetRotation( GetRotation() );
	m_pDebugCapsule->SetPosition( GetPosition() );
	m_pDebugCapsule->Render();
#endif	// #ifdef _DEBUG.
}

//----------------------------------.
// 色を変えるかどうか.
//----------------------------------.
void CCapsuleCollision::SetChangeColor( const bool& changed )
{
#ifdef _DEBUG
	m_pDebugCapsule->ChangeColor( changed );
#endif	// #ifdef _DEBUG.
}

//----------------------------------.
// 点と直線の最短距離.
//----------------------------------.
float CCapsuleCollision::calcPointLineDist( const Point& p, const Line& l, Point& h, float& t )
{
	float lenSqV = l.v.lengthSq();
	t = 0.0f;
	if ( lenSqV > 0.0f )
		t = l.v.dot( p - l.p ) / lenSqV;

	h = l.p + t * l.v;
	return ( h - p ).length();
}

//----------------------------------.
// ∠p1p2p3は鋭角？.
//----------------------------------.
bool CCapsuleCollision::isSharpAngle( const Point& p1, const Point& p2, const Point& p3 )
{
	return Vec3( p1 - p2 ).isSharpAngle( p3 - p2 );
}

//----------------------------------.
// 点と線分の最短距離.
//----------------------------------.
float CCapsuleCollision::calcPointSegmentDist( const Point& p, const Segment& seg, Point& h, float& t )
{
	const Point e = seg.getEndPoint();

	// 垂線の長さ、垂線の足の座標及びtを算出
	float len = calcPointLineDist( p, Line( seg.p, e - seg.p ), h, t );

	if ( isSharpAngle( p, seg.p, e ) == false ) {
		// 始点側の外側
		h = seg.p;
		return ( seg.p - p ).length();
	}
	else if ( isSharpAngle( p, e, seg.p ) == false ) {
		// 終点側の外側
		h = e;
		return ( e - p ).length();
	}

	return len;
}

//----------------------------------.
// 2直線の最短距離.
//----------------------------------.
float CCapsuleCollision::calcLineLineDist( 
	const Line& l1, const Line& l2, 
	Point& p1, Point& p2, 
	float& t1, float& t2 )
{
	// 2直線が平行？
	if ( l1.v.isParallel( l2.v ) == true ) {

		// 点P11と直線L2の最短距離の問題に帰着
		float len = calcPointLineDist( l1.p, l2, p2, t2 );
		p1 = l1.p;
		t1 = 0.0f;

		return len;
	}

	// 2直線はねじれ関係
	float DV1V2 = l1.v.dot( l2.v );
	float DV1V1 = l1.v.lengthSq();
	float DV2V2 = l2.v.lengthSq();
	Vec3 P21P11 = l1.p - l2.p;
	t1 = ( DV1V2 * l2.v.dot( P21P11 ) - DV2V2 * l1.v.dot( P21P11 ) ) / ( DV1V1 * DV2V2 - DV1V2 * DV1V2 );
	p1 = l1.getPoint( t1 );
	t2 = l2.v.dot( p1 - l2.p ) / DV2V2;
	p2 = l2.getPoint( t2 );

	return ( p2 - p1 ).length();
}

float CCapsuleCollision::calcSegmentSegmentDist( 
	const Segment &s1, const Segment &s2, 
	Point &p1, Point &p2, 
	float &t1, float &t2 )
{
	// S1が縮退している？.
	if( s1.v.lengthSq() < _OX_EPSILON_ ){
		// S2も縮退？.
		if( s2.v.lengthSq() < _OX_EPSILON_ ){
			// 点と点の距離の問題に帰着.
			float len = ( s2.p - s1.p ).length();
			p1 = s1.p;
			p2 = s2.p;
			t1 = t2 = 0.0f;
			return len;
		} else {
			// S1の始点とS2の最短問題に帰着.
			float len = calcPointSegmentDist( s1.p, s2, p2, t2 );
			p1 = s1.p;
			t1 = 0.0f;
			clamp01( t2 );
			return len;
		}
	}

	// S2が縮退している？.
	else if ( s2.v.lengthSq() < _OX_EPSILON_ ) {
		// S2の始点とS1の最短問題に帰着.
		float len = calcPointSegmentDist( s2.p, s1, p1, t1 );
		p2 = s2.p;
		clamp01( t1 );
		t2 = 0.0f;
		return len;
	}

	/* 線分同士 */

	// 2線分が平行だったら垂線の端点の一つをP1に仮決定.
	if ( s1.v.isParallel( s2.v ) == true ) {
		t1 = 0.0f;
		p1 = s1.p;
		float len = calcPointSegmentDist( s1.p, s2, p2, t2 );
		if ( 0.0f <= t2 && t2 <= 1.0f )
			return len;
	}
	else {
		// 線分はねじれの関係.
		// 2直線間の最短距離を求めて仮のt1,t2を求める.
		float len = calcLineLineDist( s1, s2, p1, p2, t1, t2 );
		if (
			0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f
			) {
			return len;
		}
	}

	// 垂線の足が外にある事が判明.
	// S1側のt1を0～1の間にクランプして垂線を降ろす.
	clamp01( t1 );
	p1 = s1.getPoint( t1 );
	float len = calcPointSegmentDist( p1, s2, p2, t2 );
	if ( 0.0f <= t2 && t2 <= 1.0f )
		return len;

	// S2側が外だったのでS2側をクランプ、S1に垂線を降ろす.
	clamp01( t2 );
	p2 = s2.getPoint( t2 );
	len = calcPointSegmentDist( p2, s1, p1, t1 );
	if ( 0.0f <= t1 && t1 <= 1.0f )
		return len;

	// 双方の端点が最短と判明.
	clamp01( t1 );
	p1 = s1.getPoint( t1 );
	return ( p2 - p1 ).length();
}

Segment CCapsuleCollision::GetSegment() 
{
	D3DXMATRIX mRot;
	D3DXVECTOR3 vRot = *m_pvRotation;
	D3DXMATRIX mYaw, mPitch, mRoll;
	D3DXMatrixRotationX( &mPitch, vRot.x );
	D3DXMatrixRotationY( &mYaw, vRot.y );
	D3DXMatrixRotationZ( &mRoll, vRot.z );
	mRot = mYaw * mPitch * mRoll;

	// 軸ベクトルを用意.
	D3DXVECTOR3 vAxis = { 0.0, m_Height/2.0f, 0.0f };
	D3DXVECTOR3 vecAxisY;
	// Yベクトルそのものを現在の回転情報により変換する.
	D3DXVec3TransformCoord( &vecAxisY, &vAxis, &mRot );

	D3DXVECTOR3 startPos = *m_pvPosition - vecAxisY * 1.0f;
	D3DXVECTOR3 endPos = *m_pvPosition + vecAxisY * 1.0f;

	m_Segment = Segment(
		Point( startPos.x, startPos.y, startPos.z ),	// 始点.
		Point( endPos.x, endPos.y, endPos.z ) );		// 終点.

	return m_Segment;
}