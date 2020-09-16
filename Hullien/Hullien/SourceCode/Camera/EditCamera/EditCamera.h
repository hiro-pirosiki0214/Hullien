#ifndef EDIT_CAMERA_H
#define EDIT_CAMERA_H

#include "..\Camera.h"
#include <memory>

class CMouse;	// マウスクラス.

class CEditCamera : public CCamera
{
public:
	CEditCamera();
	virtual ~CEditCamera();

	// 更新関数.
	void Updata();
	// ウィンドウハンドルの取得.
	void SetHWND( HWND hWnd ){ m_hWnd = hWnd; }

private:
	void MouseUpdate();

private:
	HWND m_hWnd;
	std::unique_ptr<CMouse> m_pMouse;
	D3DXVECTOR2 m_vDegree;
};

#endif	// #ifndef EDIT_CAMERA_H.