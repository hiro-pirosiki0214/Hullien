#include "Actor.h"
#include "..\..\Camera\CameraManager\CameraManager.h"
#include "..\InvisibleWall\BoxStruct.h"

CActor::CActor()
	: m_pCollManager	( nullptr )
	, m_pBoxWall		( nullptr )
{
}

CActor::~CActor()
{
}

// 壁に衝突したか.
bool CActor::IsCrashedWall()
{
	if( m_pBoxWall == nullptr ) return false;
	if( m_pBoxWall->MaxPosition.x > m_vPosition.x && m_vPosition.x > m_pBoxWall->MinPosition.x &&
		m_pBoxWall->MaxPosition.y > m_vPosition.z && m_vPosition.z > m_pBoxWall->MinPosition.y ) return false;

	return true;
}

// 壁に衝突したか.
bool CActor::IsCrashedWallX()
{
	if( m_pBoxWall == nullptr ) return false;
	if( m_pBoxWall->MaxPosition.x > m_vPosition.x && m_vPosition.x > m_pBoxWall->MinPosition.x ) return false;

	return true;
}

// 壁に衝突したか.
bool CActor::IsCrashedWallZ()
{
	if( m_pBoxWall == nullptr ) return false;
	if( m_pBoxWall->MaxPosition.y > m_vPosition.z && m_vPosition.z > m_pBoxWall->MinPosition.y ) return false;

	return true;
}