#include "GameObject.h"

CGameObject::CGameObject()
	: m_ObjectTag	( EObjectTag::None )
	, m_vPosition	( 0.0f, 0.0f, 0.0f )
	, m_vRotation	( 0.0f, 0.0f, 0.0f )
	, m_vSclae		( 1.0f, 1.0f, 1.0f )
{
}

CGameObject::~CGameObject()
{
}

//-----------------------------.
// オブジェクトタグ取得関数.
//-----------------------------.
EObjectTag CGameObject::GetObjectTag()
{
	return m_ObjectTag;
}

//-----------------------------.
// 座標取得関数.
//-----------------------------.
D3DXVECTOR3 CGameObject::GetPosition()
{
	return m_vPosition;
}