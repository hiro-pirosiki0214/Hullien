#include "InvisibleWall.h"
#include "..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\Utility\FileManager\FileManager.h"

CInvisibleWall::CInvisibleWall()
	: m_BoxWall		()
#ifdef _DEBUG
	, m_pCollision	( nullptr )
#endif	// #ifdef _DEBUG.
{
	m_vPosition.y = 10.0f;
}

CInvisibleWall::~CInvisibleWall()
{
}

// 初期化関数.
bool CInvisibleWall::Init()
{
	if( InitParam()		== false ) return false;
	if( InitCollision()	== false ) return false;
	return true;
}

// 更新関数.
void CInvisibleWall::Update()
{
}

// 描画関数.
void CInvisibleWall::Render()
{
#ifdef _DEBUG
	if( m_pCollision == nullptr ) return;
	m_pCollision->DebugRender();
#endif	// #ifdef _DEBUG.
}

// ボックスの設定.
void CInvisibleWall::SetBoxWall( const SBoxWall& boxWall )
{ 
	m_BoxWall = boxWall;
	// ボックスの半分の値を最大、最小として計算.
	m_BoxWall.MaxPosition = { m_BoxWall.Length.x * HALF, m_BoxWall.Length.z * HALF };
	m_BoxWall.MinPosition = { m_BoxWall.Length.x *-HALF, m_BoxWall.Length.z *-HALF };
	InitCollision();
}

// パラメータの初期化.
bool CInvisibleWall::InitParam()
{
	if( CFileManager::BinaryReading( INVISIBLE_WALL_PARAM_FILE_NAME, m_BoxWall ) == false ) return false;

	// ボックスの半分の値を最大、最小として計算.
	m_BoxWall.MaxPosition = { m_BoxWall.Length.x * HALF, m_BoxWall.Length.z * HALF };
	m_BoxWall.MinPosition = { m_BoxWall.Length.x *-HALF, m_BoxWall.Length.z *-HALF };
	return true;
}

// 当たり判定の初期化.
bool CInvisibleWall::InitCollision()
{
#ifdef _DEBUG
	if( m_pCollision == nullptr ){
		m_pCollision = std::make_unique<CCollisionManager>();
	}
	if( FAILED( m_pCollision->InitBox(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f, 0.0f, 0.0f },
		m_BoxWall.Length ))) return false;
#endif	// #ifdef _DEBUG.
	return true;
}