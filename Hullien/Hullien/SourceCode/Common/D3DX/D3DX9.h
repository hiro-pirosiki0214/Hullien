#ifndef D3DX9_H
#define D3DX9_H

#include "..\..\Global.h"

/*********************************
*	DirectX9
*/
class CDirectX9
{
public:
	CDirectX9();
	~CDirectX9();

	// DirectX9の構築.
	HRESULT Create( HWND hWnd );
	// DirectX9の解放.
	void Release();

	// デバイスの取得関数.
	static LPDIRECT3DDEVICE9 GetDevice(){ return m_pDevice9; }

private:
	// デバイス9の構築.
	HRESULT CreateDevice9();

private:
	static LPDIRECT3DDEVICE9	m_pDevice9;	// デバイス9.
	HWND	m_hWnd;	// ウィンドウハンドル.
};

#endif	// #ifndef D3DX9_H.