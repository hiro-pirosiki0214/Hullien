#ifndef D3DX_H
#define D3DX_H

#include "..\..\Global.h"
#include <vector>

class CDirectX11
{
public: //----- 定数 ------.
	const int MAX_CASCADE = 4;	// カスケードの最大数.

private: //----- 定数 ------.
	const float CLEAR_BACK_COLOR[4] = { 0.6f, 0.6f, 0.6f, 1.0f };	// バックカラー.

	const int G_BUFFER_NUM = 3;	// バッファの数.

	enum enGBUFFER
	{
		enGBUFFER_COLOR,
		enGBUFFER_NORMAL,
		enGBUFFER_Z_DEPTH,
		enGBUFFER_MAX,

	} typedef EGBUFFER;

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

	// 深度バッファの設定.
	static void SetZBuffer( int i );
	// G-Bufferの設定.
	static void SetGBuufer();
	// BackBufferの設定.
	static void SetBackBuffer();
	// TransBufferの設定.
	static void SetTransBuffer();

	// 深度バッファテクスチャを取得.
	static std::vector<ID3D11ShaderResourceView*> GetZBuffer(){ return GetInstance()->m_pZBufferSRV; }
	// G-Bufferテクスチャを取得.
	static std::vector<ID3D11ShaderResourceView*> GetGBuffer(){ return GetInstance()->m_pGBufferSRV; }
	// TransBufferテクスチャを取得.
	static ID3D11ShaderResourceView* GetTransBaffer(){ return GetInstance()->m_pTransBufferSRV; }

private: //----- 関数 ------.
		 // デバイス11の作成.
	HRESULT InitDevice11();
	// レンダーターゲットビューの作成.
	HRESULT InitTexRTV();
	// ステンシルビューの作成.
	HRESULT InitDSTex();
	// Z-Bufferの作成.
	HRESULT InitZBufferTex();
	// G-Bufferの作成.
	HRESULT InitGBufferTex();
	// 
	HRESULT InitTransBufferTex();
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

	std::vector<ID3D11RenderTargetView*>	m_pZBufferRTV;	// 深度バッファのレンダーターゲットビュー.
	std::vector<ID3D11ShaderResourceView*>	m_pZBufferSRV;	// 深度バッファのステンシルビュー.
	std::vector<ID3D11Texture2D*>			m_pZBufferTex;	// 深度バッファのテクスチャー2D.

	std::vector<ID3D11RenderTargetView*>	m_pGBufferRTV;	// G-Bufferのレンダーターゲットビュー.
	std::vector<ID3D11ShaderResourceView*>	m_pGBufferSRV;	// G-Bufferのシェーダーリソースビュー.
	std::vector<ID3D11Texture2D*>			m_pGBufferTex;	// G-Bufferのテクスチャ2D.

	ID3D11RenderTargetView*		m_pTransBufferRTV;	// 半透明描画用のレンダーターゲットビュー.
	ID3D11ShaderResourceView*	m_pTransBufferSRV;	// 半透明描画用のシェーダーリソースビュー.
	ID3D11Texture2D*			m_pTransBufferTex;	// 半透明描画用のテクスチャ2D.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDirectX11( const CDirectX11 & )				= delete;
	CDirectX11& operator = ( const CDirectX11 & )	= delete;
	CDirectX11( CDirectX11 && )						= delete;
	CDirectX11& operator = ( CDirectX11 && )		= delete;
};

#endif	// #ifndef D3DX_H.