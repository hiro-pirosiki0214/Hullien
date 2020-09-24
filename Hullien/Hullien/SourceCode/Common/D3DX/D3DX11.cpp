#include "D3DX11.h"

CDirectX11::CDirectX11()
	: m_hWnd					( nullptr )
	, m_pDevice11				( nullptr )
	, m_pContext11				( nullptr )
	, m_pSwapChain				( nullptr )
	, m_pBackBuffer_TexRTV		( nullptr )
	, m_pBackBuffer_DSTex		( nullptr )
	, m_pBackBuffer_DSTexDSV	( nullptr )
	, m_pZBufferRTV				( MAX_CASCADE )
	, m_pZBufferSRV				( MAX_CASCADE )
	, m_pZBufferTex				( MAX_CASCADE )
	, m_pGBufferRTV				( G_BUFFER_NUM )
	, m_pGBufferSRV				( G_BUFFER_NUM )
	, m_pGBufferTex				( G_BUFFER_NUM )
	, m_pTransBufferRTV			( nullptr )
	, m_pTransBufferSRV			( nullptr )
	, m_pTransBufferTex			( nullptr )
{
}

CDirectX11::~CDirectX11()
{
}

//-----------------------------------.
// インスタンスの取得.
//-----------------------------------.
CDirectX11* CDirectX11::GetInstance()
{
	static std::unique_ptr<CDirectX11> pInstance = 
		std::make_unique<CDirectX11>();
	return pInstance.get();
}

//-----------------------------------.
// DirectX11構築関数.
//-----------------------------------.
HRESULT CDirectX11::Create( HWND hWnd )
{
	GetInstance()->m_hWnd = hWnd;

	if( FAILED(GetInstance()->InitDevice11()) )		return E_FAIL;
	if( FAILED(GetInstance()->InitTexRTV()) )		return E_FAIL;
	if( FAILED(GetInstance()->InitDSTex()) )		return E_FAIL;
	if( FAILED(GetInstance()->InitZBufferTex()) )	return E_FAIL;
	if( FAILED(GetInstance()->InitGBufferTex()))	return E_FAIL;
	if( FAILED(GetInstance()->InitTransBufferTex()))return E_FAIL;
	if( FAILED(GetInstance()->InitViewports()) )	return E_FAIL;
	if( FAILED(GetInstance()->InitRasterizer()) )	return E_FAIL;

	return S_OK;
}

//-----------------------------------.
// DirectX11解放関数.
//-----------------------------------.
HRESULT CDirectX11::Release()
{
	for( auto& rtv : GetInstance()->m_pGBufferRTV ) SAFE_RELEASE(rtv);
	for( auto& srv : GetInstance()->m_pGBufferSRV ) SAFE_RELEASE(srv);
	for( auto& tex : GetInstance()->m_pGBufferTex ) SAFE_RELEASE(tex);

	for( auto& rtv : GetInstance()->m_pZBufferRTV ) SAFE_RELEASE(rtv);
	for( auto& srv : GetInstance()->m_pZBufferSRV ) SAFE_RELEASE(srv);
	for( auto& tex : GetInstance()->m_pZBufferTex ) SAFE_RELEASE(tex);

	SAFE_RELEASE(GetInstance()->m_pTransBufferSRV);
	SAFE_RELEASE(GetInstance()->m_pTransBufferTex);
	SAFE_RELEASE(GetInstance()->m_pTransBufferRTV);

	SAFE_RELEASE(GetInstance()->m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(GetInstance()->m_pBackBuffer_DSTex);
	SAFE_RELEASE(GetInstance()->m_pBackBuffer_TexRTV);
	SAFE_RELEASE(GetInstance()->m_pSwapChain);
	SAFE_RELEASE(GetInstance()->m_pContext11);
	SAFE_RELEASE(GetInstance()->m_pDevice11);

	return S_OK;
}

//-----------------------------------.
// クリアバックバッファ.
//-----------------------------------.
void CDirectX11::ClearBackBuffer()
{
	// カラーバックバッファ.
	GetInstance()->m_pContext11->ClearRenderTargetView( 
		GetInstance()->m_pBackBuffer_TexRTV, GetInstance()->CLEAR_BACK_COLOR );

	// 深度バッファテクスチャのクリア.
	for( auto& rtv : GetInstance()->m_pZBufferRTV ){
		GetInstance()->m_pContext11->ClearRenderTargetView( rtv, GetInstance()->CLEAR_BACK_COLOR );
	}
	// G-Bufferテクスチャのクリア.
	for( auto& rtv : GetInstance()->m_pGBufferRTV ){
		GetInstance()->m_pContext11->ClearRenderTargetView( rtv, GetInstance()->CLEAR_BACK_COLOR );
	}
	GetInstance()->m_pContext11->ClearRenderTargetView( 
		GetInstance()->m_pTransBufferRTV, GetInstance()->CLEAR_BACK_COLOR );
}

//-----------------------------------.
// スワップチェーンプレゼント.
//-----------------------------------.
void CDirectX11::SwapChainPresent()
{
	GetInstance()->m_pSwapChain->Present( 0, 0 );
}

//-----------------------------------.
// 深度バッファの設定.
//-----------------------------------.
void CDirectX11::SetZBuffer( int i )
{
	// レンダーターゲットの設定.
	GetInstance()->m_pContext11->OMSetRenderTargets( 
		1, 
		&GetInstance()->m_pZBufferRTV[i], 
		GetInstance()->m_pBackBuffer_DSTexDSV);
	// デプスステンシルバッファ.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH,
		1.0f, 0 );
}

//-----------------------------------.
// G-Bufferの設定.
//-----------------------------------.
void CDirectX11::SetGBuufer()
{
	// レンダーターゲットの設定.
	GetInstance()->m_pContext11->OMSetRenderTargets( 
		enGBUFFER_MAX,
		&GetInstance()->m_pGBufferRTV[0],
		GetInstance()->m_pBackBuffer_DSTexDSV );
	// デプスステンシルバッファ.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH,
		1.0f, 0 );
}

//-----------------------------------.
// BackBufferの設定.
//-----------------------------------.
void CDirectX11::SetBackBuffer()
{
	// レンダーターゲットの設定.
	GetInstance()->m_pContext11->OMSetRenderTargets( 
		1, 
		&GetInstance()->m_pBackBuffer_TexRTV,
		GetInstance()->m_pBackBuffer_DSTexDSV );
	// デプスステンシルバッファ.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );
}
//
void CDirectX11::SetTransBuffer()
{
	// レンダーターゲットの設定.
	GetInstance()->m_pContext11->OMSetRenderTargets( 
		1,
		&GetInstance()->m_pTransBufferRTV,
		GetInstance()->m_pBackBuffer_DSTexDSV );
	// デプスステンシルバッファ.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH,
		1.0f, 0 );
}

//-----------------------------------.
// デバイス11の作成.
//-----------------------------------.
HRESULT CDirectX11::InitDevice11()
{
	// スワップチェーン構造体.
	DXGI_SWAP_CHAIN_DESC sd = {0};
	sd.BufferCount			= 1;								// バックバッファの数.
	sd.BufferDesc.Width		= WND_W;							// バックバッファの幅.
	sd.BufferDesc.Height	= WND_H;							// バックバッファの高さ.
	sd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;		// フォーマット(32ﾋﾞｯﾄｶﾗｰ).
	sd.BufferDesc.RefreshRate.Numerator		= 60;				// リフレッシュレート(分母) ※FPS:60.
	sd.BufferDesc.RefreshRate.Denominator	= 1;				// リフレッシュレート(分子).
	sd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 使い方(表示先).
	sd.OutputWindow			= m_hWnd;							// ウィンドウハンドル.
	sd.SampleDesc.Count		= 1;								// マルチサンプルの数.
	sd.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	sd.Windowed				= TRUE;								// ウィンドウモード(フルスクリーン時はFALSE).

																//作成を試みる機能レベルの優先を指定.
																// (GPUがサポートする機能ｾｯﾄの定義).
																// D3D_FEATURE_LEVEL列挙型の配列.
																// D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 の GPUレベル.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;	// 配列の要素数.

	auto createDevice = [&]( const D3D_DRIVER_TYPE& type ) -> HRESULT
	{
		return D3D11CreateDeviceAndSwapChain(
			nullptr,			// ビデオアダプタへのポインタ.
			type,				// 作成するﾃデバイスの種類.
			nullptr,			// ソフトウェア ラスタライザを実装するDLLのハンドル.
			0,					// 有効にするランタイムレイヤー.
			&pFeatureLevels,	// 作成を試みる機能レベルの順序を指定する配列へのポインタ.
			1,					// ↑の要素数.
			D3D11_SDK_VERSION,	// SDKのバージョン.
			&sd,				// スワップチェーンの初期化パラメータのポインタ.
			&m_pSwapChain,		// (out) レンダリングに使用するスワップチェーン.
			&m_pDevice11,		// (out) 作成されたデバイス.
			pFeatureLevel,		// 機能ﾚﾍﾞﾙの配列にある最初の要素を表すポインタ.
			&m_pContext11 );	// (out) デバイスコンテキスト.
	};

	if( FAILED(createDevice(D3D_DRIVER_TYPE_HARDWARE)) ){
		if( FAILED(createDevice(D3D_DRIVER_TYPE_WARP)) ){
			if( FAILED(createDevice(D3D_DRIVER_TYPE_REFERENCE)) ){
				MessageBox( nullptr,
					"デバイスとスワップチェーン作成失敗",
					"Error", MB_OK );
				return E_FAIL;
			}
		}
	}
	pFeatureLevel = nullptr;

	return S_OK;
}

//-----------------------------------.
// レンダーターゲットビューの作成.
//-----------------------------------.
HRESULT CDirectX11::InitTexRTV()
{
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:式に関連付けされたGUIDを取得.
		(LPVOID*)&pBackBuffer_Tex);	//(out)バックバッファテクスチャ.

									//そのテクスチャに対してレンダーターゲットビュー(RTV)を作成.
	m_pDevice11->CreateRenderTargetView(
		pBackBuffer_Tex,
		nullptr,
		&m_pBackBuffer_TexRTV);	//(out)RTV.

								// バックバッファテクスチャを解放.
	SAFE_RELEASE(pBackBuffer_Tex);

	return S_OK;
}

//-----------------------------------.
// ステンシルビューの作成.
//-----------------------------------.
HRESULT CDirectX11::InitDSTex()
{
	int wnd_Width = WND_W;
	int wnd_Height = WND_H;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					= wnd_Width;				// 幅.
	descDepth.Height				= wnd_Height;				// 高さ.
	descDepth.MipLevels				= 1;						// ミップマップレベル:1.
	descDepth.ArraySize				= 1;						// 配列数:1.
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// 32ビットフォーマット.
	descDepth.SampleDesc.Count		= 1;						// マルチサンプルの数.
	descDepth.SampleDesc.Quality	= 0;						// マルチサンプルのクオリティ.
	descDepth.Usage					= D3D11_USAGE_DEFAULT;		// 使用方法:デフォルト.
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// 深度(ステンシルとして使用).
	descDepth.CPUAccessFlags		= 0;						// CPUからはアクセスしない.
	descDepth.MiscFlags				= 0;						// その他の設定なし.


																// そのテクスチャに対してデプスステンシル(DSTex)を作成.
	if( FAILED( m_pDevice11->CreateTexture2D( &descDepth, nullptr, &m_pBackBuffer_DSTex )) ){
		_ASSERT_EXPR( false, L"デプスステンシル作成失敗" );
		return E_FAIL;
	}
	// そのテクスチャに対してデブスステンシルビュー(DSV)を作成.
	if( FAILED( m_pDevice11->CreateDepthStencilView( m_pBackBuffer_DSTex, nullptr, &m_pBackBuffer_DSTexDSV)) ){
		_ASSERT_EXPR( false, L"デプスステンシルビュー作成失敗" );
		return E_FAIL;
	}
	// レンダーターゲットとﾃﾞﾌﾞｽステンシルビューをパイプラインにセット.
	m_pContext11->OMSetRenderTargets( 1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV );
	return S_OK;
}

//-----------------------------------.
// Z-Bufferの作成.
//-----------------------------------.
HRESULT CDirectX11::InitZBufferTex()
{
	for( int i = 0; i < MAX_CASCADE; i++ ){
		D3D11_TEXTURE2D_DESC texDepth;
		texDepth.Width				= WND_W;						// 幅.
		texDepth.Height				= WND_H;						// 高さ.
		texDepth.MipLevels			= 1;							// ミップマップレベル:1.
		texDepth.ArraySize			= 1;							// 配列数:1.
		texDepth.SampleDesc.Count	= 1;							// 32ビットフォーマット.
		texDepth.SampleDesc.Quality	= 0;							// マルチサンプルの数.
		texDepth.Format				= DXGI_FORMAT_B8G8R8A8_UNORM;	// マルチサンプルのクオリティ.
		texDepth.Usage				= D3D11_USAGE_DEFAULT;			// 使用方法:デフォルト.
		texDepth.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
		texDepth.CPUAccessFlags		= 0;							// CPUからはアクセスしない.
		texDepth.MiscFlags			= 0;							// その他の設定なし.

																	// そのテクスチャに対してデプスステンシル(DSTex)を作成.
		if( FAILED( m_pDevice11->CreateTexture2D( &texDepth, nullptr, &m_pZBufferTex[i] )) ){
			_ASSERT_EXPR( false, L"テクスチャデスク作成失敗" );
			return E_FAIL;
		}
		// レンダーターゲットビューの設定.
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		memset( &rtvDesc, 0, sizeof( rtvDesc ) );
		rtvDesc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
		rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		// RenderTargetView作成.
		if( FAILED( m_pDevice11->CreateRenderTargetView( m_pZBufferTex[i], &rtvDesc, &m_pZBufferRTV[i] ) )){
			_ASSERT_EXPR( false, L"RenderTargetView作成失敗" );
			return E_FAIL;
		}

		// シェーダリソースビューの設定
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		memset( &srvDesc, 0, sizeof( srvDesc ) );
		srvDesc.Format              = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		// テクスチャ作成時と同じフォーマット
		if( FAILED( m_pDevice11->CreateShaderResourceView( m_pZBufferTex[i], &srvDesc, &m_pZBufferSRV[i] ) )){
			_ASSERT_EXPR( false, L"デプスステンシル作成失敗" );
			return E_FAIL;
		}
	}

	return S_OK;
}

//-----------------------------------.
// G-Bufferの作成.
//-----------------------------------.
HRESULT CDirectX11::InitGBufferTex()
{
	D3D11_TEXTURE2D_DESC texDepth;
	texDepth.Width				= WND_W;							// 幅.
	texDepth.Height				= WND_H;							// 高さ.
	texDepth.MipLevels			= 1;								// ミップマップレベル:1.
	texDepth.ArraySize			= 1;								// 配列数:1.
	texDepth.Format				= DXGI_FORMAT_R11G11B10_FLOAT;		// 32ビットフォーマット.
	texDepth.SampleDesc.Count	= 1;								// マルチサンプルの数.
	texDepth.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	texDepth.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法:デフォルト.
	texDepth.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
	texDepth.CPUAccessFlags		= 0;								// CPUからはアクセスしない.
	texDepth.MiscFlags			= 0;								// その他の設定なし.

	for( int i = 0; i < G_BUFFER_NUM; i++ ){
		if( i == enGBUFFER_Z_DEPTH ){
			texDepth.Format				= DXGI_FORMAT_B8G8R8A8_UNORM;// 32ﾋﾞｯﾄﾌｫｰﾏｯﾄ.

																	 // そのテクスチャに対してデプスステンシル(DSTex)を作成.
			if( FAILED( m_pDevice11->CreateTexture2D( &texDepth, nullptr, &m_pGBufferTex[i] )) ){
				_ASSERT_EXPR( false, L"テクスチャデスク作成失敗" );
				return E_FAIL;
			}
			// レンダーターゲットビューの設定.
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			memset( &rtvDesc, 0, sizeof( rtvDesc ) );
			rtvDesc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
			rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = 0;
			// RenderTargetView作成.
			if( FAILED( m_pDevice11->CreateRenderTargetView( m_pGBufferTex[i], &rtvDesc, &m_pGBufferRTV[i] ) )){
				_ASSERT_EXPR( false, L"RenderTargetView作成失敗" );
				return E_FAIL;
			}

			// シェーダリソースビューの設定
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			memset( &srvDesc, 0, sizeof( srvDesc ) );
			srvDesc.Format              = DXGI_FORMAT_B8G8R8A8_UNORM;
			srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;

			// テクスチャ作成時と同じフォーマット
			if( FAILED( m_pDevice11->CreateShaderResourceView( m_pGBufferTex[i], &srvDesc, &m_pGBufferSRV[i] ) )){
				_ASSERT_EXPR( false, L"デプスステンシル作成失敗" );
				return E_FAIL;
			}
			continue;
		}
		// そのテクスチャに対してデプスステンシル(DSTex)を作成.
		if( FAILED( m_pDevice11->CreateTexture2D( &texDepth, nullptr, &m_pGBufferTex[i] )) ){
			_ASSERT_EXPR( false, L"テクスチャデスク作成失敗" );
			return E_FAIL;
		}
		// レンダーターゲットビューの設定
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		memset( &rtvDesc, 0, sizeof( rtvDesc ) );
		rtvDesc.Format             = DXGI_FORMAT_R11G11B10_FLOAT;
		rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
		// RenderTargetView作成.
		if( FAILED( m_pDevice11->CreateRenderTargetView( m_pGBufferTex[i], &rtvDesc, &m_pGBufferRTV[i] ) )){
			_ASSERT_EXPR( false, L"RenderTargetView作成失敗" );
			return E_FAIL;
		}

		// シェーダリソースビューの設定
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		memset( &srvDesc, 0, sizeof( srvDesc ) );
		srvDesc.Format              = rtvDesc.Format;
		srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		// テクスチャ作成時と同じフォーマット
		if( FAILED( m_pDevice11->CreateShaderResourceView( m_pGBufferTex[i], &srvDesc, &m_pGBufferSRV[i] ) )){
			_ASSERT_EXPR( false, L"デプスステンシル作成失敗" );
			return E_FAIL;
		}
	}
	return S_OK;
}

// 
HRESULT CDirectX11::InitTransBufferTex()
{
	D3D11_TEXTURE2D_DESC texDepth;
	texDepth.Width				= WND_W;							// 幅.
	texDepth.Height				= WND_H;							// 高さ.
	texDepth.MipLevels			= 1;								// ミップマップレベル:1.
	texDepth.ArraySize			= 1;								// 配列数:1.
	texDepth.Format				= DXGI_FORMAT_R11G11B10_FLOAT;		// 32ビットフォーマット.
	texDepth.SampleDesc.Count	= 1;								// マルチサンプルの数.
	texDepth.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	texDepth.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法:デフォルト.
	texDepth.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
	texDepth.CPUAccessFlags		= 0;								// CPUからはアクセスしない.
	texDepth.MiscFlags			= 0;								// その他の設定なし.

	
	// そのテクスチャに対してデプスステンシル(DSTex)を作成.
	if( FAILED( m_pDevice11->CreateTexture2D( &texDepth, nullptr, &m_pTransBufferTex )) ){
		_ASSERT_EXPR( false, L"テクスチャデスク作成失敗" );
		return E_FAIL;
	}
	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset( &rtvDesc, 0, sizeof( rtvDesc ) );
	rtvDesc.Format             = DXGI_FORMAT_R11G11B10_FLOAT;
	rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
	// RenderTargetView作成.
	if( FAILED( m_pDevice11->CreateRenderTargetView( m_pTransBufferTex, &rtvDesc, &m_pTransBufferRTV ) )){
		_ASSERT_EXPR( false, L"RenderTargetView作成失敗" );
		return E_FAIL;
	}

	// シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset( &srvDesc, 0, sizeof( srvDesc ) );
	srvDesc.Format              = rtvDesc.Format;
	srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// テクスチャ作成時と同じフォーマット
	if( FAILED( m_pDevice11->CreateShaderResourceView( m_pTransBufferTex, &srvDesc, &m_pTransBufferSRV ) )){
		_ASSERT_EXPR( false, L"デプスステンシル作成失敗" );
		return E_FAIL;
	}
	return S_OK;
}

//-----------------------------------.
// ビューポートの作成.
//-----------------------------------.
HRESULT CDirectX11::InitViewports()
{
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT)WND_W;	// 幅.
	vp.Height	= (FLOAT)WND_H;	// 高さ.
	vp.MinDepth = 0.0f;			// 最小深度(手前).
	vp.MaxDepth = 1.0f;			// 最大深度(奥).
	vp.TopLeftX = 0.0f;			// 左上位置x.
	vp.TopLeftY = 0.0f;			// 左上位置y.

	m_pContext11->RSSetViewports( 1, &vp );
	return S_OK;
}

//-----------------------------------.
// ラスタライザの作成.
//-----------------------------------.
HRESULT CDirectX11::InitRasterizer()
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode				= D3D11_FILL_SOLID;	// 塗りつぶし(ソリッド).
	rdc.CullMode				= D3D11_CULL_NONE;	// BACK:背面を描画しない, FRONT:正面を描画しない.
	rdc.FrontCounterClockwise	= FALSE;			// ポリゴンの表裏を決定するフラグ.
	rdc.DepthClipEnable			= FALSE;			// 距離についてのクリッピング有効.

	ID3D11RasterizerState* pRs = nullptr;
	if( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &pRs )) ){
		_ASSERT_EXPR( false, L"ラスタライザー作成失敗" );
	}
	m_pContext11->RSSetState( pRs );
	SAFE_RELEASE( pRs );

	return S_OK;
}