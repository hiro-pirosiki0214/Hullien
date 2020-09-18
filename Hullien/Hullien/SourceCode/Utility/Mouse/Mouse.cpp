#include "Mouse.h"
#include "..\..\Global.h"

CMouse::CMouse()
	: m_Point		{ 0, 0 }
	, m_OldPoint	{ 0, 0 }
{
}

CMouse::~CMouse()
{
}

// マウスの更新.
void CMouse::UpdateMouse( HWND hWnd )
{
	m_OldPoint = m_Point;
	// マウスカーソル位置取得.
	GetCursorPos( &m_Point );
	// スクリーン座標→クライアント座標に変換.
	ScreenToClient( hWnd, &m_Point );
}

// 座標Xの取得.
float CMouse::GetPosisionX()
{
	return static_cast<float>(m_Point.x);
}

// 座標Ｙの取得.
float CMouse::GetPosisionY()
{
	return static_cast<float>(m_Point.y);
}

// 前回の座標Xの取得.
float CMouse::GetOldPosisionX()
{
	return static_cast<float>(m_OldPoint.x);
}

// 前回の座標Ｙの取得.
float CMouse::GetOldPosisionY()
{
	return static_cast<float>(m_OldPoint.y);
}

// スクリーンの中か.
bool CMouse::IsScreenMiddle()
{
	return (( 0 < m_Point.x && m_Point.x < WND_W ) &&
			( 0 < m_Point.y && m_Point.y < WND_H ));
}