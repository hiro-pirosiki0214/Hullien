#ifndef EDIT_CAMERA_H
#define EDIT_CAMERA_H

#include "..\Camera.h"
#include <memory>

class CMouse;	// マウスクラス.

/****************************************
* エディタ用カメラ.
**/
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
	// マウスの更新.
	void MouseUpdate();

private:
	HWND					m_hWnd;		// ウィンドウハンドル.
	std::unique_ptr<CMouse> m_pMouse;	// マウスクラス.
	D3DXVECTOR2				m_Radian;	// ラジアン.
};

#endif	// #ifndef EDIT_CAMERA_H.