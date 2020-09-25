#ifndef D3DX_H
#define D3DX_H

#include "..\..\Global.h"
#include <vector>

class CDirectX11
{
private: //----- 定数 ------.
	const float CLEAR_BACK_COLOR[4] = { 0.6f, 0.6f, 0.6f, 1.0f };	// バックカラー.

public: //----- 関数 ------.
	CDirectX11();
	~CDirectX11();

	// インスタンスの取得.
	static CDirectX11* GetInstance();

	// DirectX11構築関数.
	static HRESULT Create( HWND hWnd );
	// DirectX11解放関数.
	static HRESULT Release();

	// クリアバックバッファ.
	static void ClearBackBuffer();
	// スワップチェーンプレゼント.
	static void SwapChainPresent();

	// デバイス11の取得関数.
	static ID3D11Device*		GetDevice() { return GetInstance()->m_pDevice11; }
	// コンテキスト11の取得関数.
	static ID3D11DeviceContext*	GetContext() { return GetInstance()->m_pContext11; }
	// デプスステンシルビューの取得.
	static ID3D11DepthStencilView* GetDepthSV(){ return GetInstance()->m_pBackBuffer_DSTexDSV; }
	// BackBufferの設定.
	static void SetBackBuffer();

private: //----- 関数 ------.
		 // デバイス11の作成.
	HRESULT InitDevice11();
	// レンダーターゲットビューの作成.
	HRESULT InitTexRTV();
	// ステンシルビューの作成.
	HRESULT InitDSTex();
	// ビューポートの作成.
	HRESULT InitViewports();
	// ラスタライザの作成.
	HRESULT InitRasterizer();

private: //----- 変数 ------.
	HWND	m_hWnd;	// ウィンドウハンドル.

	ID3D11Device*			m_pDevice11;	// デバイス11.
	ID3D11DeviceContext*	m_pContext11;	// コンテキスト11.

	IDXGISwapChain*			m_pSwapChain;			// スワップチェーン.

	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;	// レンダーターゲットビュー.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;	// ステンシルビュー.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;	// テクスチャー2D.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDirectX11( const CDirectX11 & )				= delete;
	CDirectX11& operator = ( const CDirectX11 & )	= delete;
	CDirectX11( CDirectX11 && )						= delete;
	CDirectX11& operator = ( CDirectX11 && )		= delete;
};

#endif	// #ifndef D3DX_H.