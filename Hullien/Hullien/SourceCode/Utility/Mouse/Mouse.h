#ifndef MOUSE_H
#define MOUSE_H

#include <Windows.h>

class CMouse
{
public:
	CMouse();
	~CMouse();

	// マウスの更新.
	void UpdateMouse( HWND hWnd );

	// 座標Xの取得.
	float GetPosisionX();
	// 座標Ｙの取得.
	float GetPosisionY();
	// 前回の座標Xの取得.
	float GetOldPosisionX();
	// 前回の座標Ｙの取得.
	float GetOldPosisionY();
	// スクリーンの中か.
	bool IsScreenMiddle();

private:
	POINT	m_Point;
	POINT	m_OldPoint;
};

#endif	// #ifndef MOUSE_H.