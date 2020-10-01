#include "RotLookAtCenter.h"
#include "..\..\Utility\MyVector\MyVector.h"

CRotLookAtCenter::CRotLookAtCenter()
	: m_vDegree			{ 0.0f, 0.0f }
	, m_Length			( DEFAULT_LENGTH )
	, m_HorizontalMax	( DEFAULT_HORIZONTAL_MAX )
	, m_HorizontalMin	( DEFAULT_HORIZONTAL_MIN )
	, m_VerticalMax		( DEFAULT_VERTICAL_MAX )
	, m_VerticalMin		( DEFAULT_VERTICAL_MIN )
{
}

CRotLookAtCenter::~CRotLookAtCenter()
{
}

// 更新.
void CRotLookAtCenter::Update()
{
}

//-------------------------------.
// オブジェクトをを注視に回転.
//-------------------------------.
void CRotLookAtCenter::RotationLookAtObject( const D3DXVECTOR3& vLookPos, const float& attenRate )
{
	// 注視位置を取得.
	m_vLookPosition = vLookPos;

	D3DXVECTOR3 pos;	// 本来の座標.
	// カメラ位置を算出.
	pos.x = m_vLookPosition.x + ( sinf(m_vDegree.x) * m_Length );
	pos.y = m_vPosition.y;
	pos.z = m_vLookPosition.z + ( cosf(m_vDegree.x) * m_Length );
	// lerp減衰.
	D3DXVec3Lerp( &m_vPosition, &m_vPosition, &pos, attenRate );
}

//-------------------------------.
// 横方向の回転.
//-------------------------------.
void CRotLookAtCenter::DegreeHorizontalMove( const float& movePower )
{
	// 横方向に足し合わせる.
	m_vDegree.x += movePower;

	// オーバーフロー対策.
	CVector::OverflowDegree( m_vDegree.y, 
		 m_HorizontalMax, m_HorizontalMin, 
		-m_HorizontalMax, m_HorizontalMin );
}

//-------------------------------.
// 縦方向の回転.
//-------------------------------.
void CRotLookAtCenter::DegreeVerticalMove( const float& movePower )
{
	// 縦方向に足し合わせる.
	m_vDegree.y += movePower;

	// オーバーフロー対策.
	CVector::OverflowDegree( m_vDegree.y, 
		m_VerticalMax, m_VerticalMax, 
		m_VerticalMin, m_VerticalMin );
}

//-------------------------------.
// カメラと注視点の長さ設定.
//-------------------------------.
void CRotLookAtCenter::SetLength( const float& length )
{
	m_Length = length;
}

//-------------------------------.
// 高さの設定.
//-------------------------------.
void CRotLookAtCenter::SetHeight( const float& height )
{
	m_vPosition.y = height;
}

//-------------------------------.
// 横方向の最大、最小の設定.
//-------------------------------.
void CRotLookAtCenter::SetHorizontalDegree( const float& max, const float& min )
{
	m_HorizontalMax	= max;
	m_HorizontalMin	= min;
}

//-------------------------------.
// 縦方向の最大、最小の設定.
//-------------------------------.
void CRotLookAtCenter::SetVerticalDegree( const float& max, const float& min )
{	
	m_VerticalMax = max;
	m_VerticalMin = min;
}