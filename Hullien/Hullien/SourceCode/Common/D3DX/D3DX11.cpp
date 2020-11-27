#include "D3DX11.h"

const int FULL_SCREEN_MUL = 1;

CDirectX11::CDirectX11()
	: m_hWnd					( nullptr )
	, m_pDevice11				( nullptr )
	, m_pContext11				( nullptr )
	, m_pSwapChain				( nullptr )
	, m_pBackBuffer_TexRTV		( nullptr )
	, m_pBackBuffer_DSTex		( nullptr )
	, m_pBackBuffer_DSTexDSV	( nullptr )
	, m_pDepthStencilState		( nullptr )
	, m_pDepthStencilStateOff	( nullptr )
	, m_pAlphaBlend				( nullptr )
	, m_pNoAlphaBlend			( nullptr )
	, m_pAlphaToCoverage		( nullptr )
	, m_pRsSoldAndNone			( nullptr )
	, m_pRsSoldAndBack			( nullptr )
	, m_pRsSoldAndFront			( nullptr )
	, m_pRsWireFrame			( nullptr )
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
	if( FAILED(GetInstance()->InitViewports()) )	return E_FAIL;
	if( FAILED(GetInstance()->InitRasterizer()) )	return E_FAIL;
	if( FAILED(GetInstance()->InitBlend()) )		return E_FAIL;
	if( FAILED(GetInstance()->InitDeprh()) )		return E_FAIL;
	if( FAILED(GetInstance()->InitRasterizerState()) )	return E_FAIL;

	return S_OK;
}

//-----------------------------------.
// DirectX11解放関数.
//-----------------------------------.
HRESULT CDirectX11::Release()
{

	SAFE_RELEASE( GetInstance()->m_pDepthStencilState );
	SAFE_RELEASE( GetInstance()->m_pDepthStencilStateOff );
	SAFE_RELEASE( GetInstance()->m_pAlphaBlend );
	SAFE_RELEASE( GetInstance()->m_pNoAlphaBlend );
	SAFE_RELEASE( GetInstance()->m_pAlphaToCoverage );
	SAFE_RELEASE( GetInstance()->m_pRsSoldAndNone );
	SAFE_RELEASE( GetInstance()->m_pRsSoldAndBack );
	SAFE_RELEASE( GetInstance()->m_pRsSoldAndFront );
	SAFE_RELEASE( GetInstance()->m_pRsWireFrame );

	SAFE_RELEASE( GetInstance()->m_pBackBuffer_DSTexDSV );
	SAFE_RELEASE( GetInstance()->m_pBackBuffer_DSTex );
	SAFE_RELEASE( GetInstance()->m_pBackBuffer_TexRTV );
	SAFE_RELEASE( GetInstance()->m_pSwapChain );
	SAFE_RELEASE( GetInstance()->m_pContext11 );
	SAFE_RELEASE( GetInstance()->m_pDevice11 );

	return S_OK;
}

//-----------------------------------.
// クリアバックバッファ.
//-----------------------------------.
void CDirectX11::ClearBackBuffer()
{
	// カラーバックバッファ.
	GetInstance()->m_pContext11->ClearRenderTargetView( 
		GetInstance()->m_pBackBuffer_TexRTV, GetInstance()->CLEAR_BACK_COLOR );;
}

//-----------------------------------.
// スワップチェーンプレゼント.
//-----------------------------------.
void CDirectX11::SwapChainPresent()
{
	GetInstance()->m_pSwapChain->Present( 0, 0 );
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

//--------------------------------------------.
// ブレンドを有効:無効に設定する.
//--------------------------------------------.
void CDirectX11::SetBlend( bool EnableAlpha )
{
	// ブレンドステートの設定.
	UINT mask = 0xffffffff;	// マスク値.
	ID3D11BlendState* blend = 
		EnableAlpha == true ? GetInstance()->m_pAlphaBlend : GetInstance()->m_pNoAlphaBlend;
	GetInstance()->m_pContext11->OMSetBlendState( blend, nullptr, mask );
}

//--------------------------------------------.
// アルファカバレージを有効:無効に設定する.
//--------------------------------------------.
void CDirectX11::SetCoverage( bool EnableCoverage )
{
	// ブレンドステートの設定.
	UINT mask = 0xffffffff;	// マスク値.
	ID3D11BlendState* blend = 
		EnableCoverage == true ? GetInstance()->m_pAlphaToCoverage : GetInstance()->m_pNoAlphaBlend;
	GetInstance()->m_pContext11->OMSetBlendState( blend, nullptr, mask );
}

//--------------------------------------------.
// 深度テストを有効:無効に設定する.
//--------------------------------------------.
void CDirectX11::SetDeprh( bool flag )
{
	ID3D11DepthStencilState* pTmp
		= ( flag == true ) ? GetInstance()->m_pDepthStencilState : GetInstance()->m_pDepthStencilStateOff;
	// 深度設定をセット.
	GetInstance()->m_pContext11->OMSetDepthStencilState( pTmp, 1 );
}

//--------------------------------------------.
// ラスタライザステート設定.
//--------------------------------------------.
void CDirectX11::SetRasterizerState( const ERS_STATE& rsState )
{
	switch( rsState )
	{
	case enRS_STATE::None:	// 正背面描画.
		GetInstance()->m_pContext11->RSSetState( GetInstance()->m_pRsSoldAndNone );
		break;
	case enRS_STATE::Back:	// 背面を描画しない.
		GetInstance()->m_pContext11->RSSetState( GetInstance()->m_pRsSoldAndBack );
		break;
	case enRS_STATE::Front:	// 正面を描画しない.
		GetInstance()->m_pContext11->RSSetState( GetInstance()->m_pRsSoldAndFront );
		break;
	case enRS_STATE::Wire:	// ワイヤーフレーム描画.
		GetInstance()->m_pContext11->RSSetState( GetInstance()->m_pRsWireFrame );
		break;
	default:
		break;
	}
}

//-----------------------------------.
// フルスクリーンの設定.
//-----------------------------------.
bool CDirectX11::SetFullScreen( const bool& isOn )
{
	// 現在のスクリーン情報を取得.
	BOOL isState = FALSE;
	GetInstance()->m_pSwapChain->GetFullscreenState( &isState, nullptr );
	if( isOn == true ){
		// 現在の状態がウィンドウ状態なら.
		if( isState == FALSE ){
			// フルスクリーンに変更.
			GetInstance()->m_pSwapChain->SetFullscreenState( TRUE, nullptr );
		}
		return true;
	} else {
		// 現在の状態がフルスクリーンなら.
		if( isState == TRUE ){
			// ウィンドウに変更.
			GetInstance()->m_pSwapChain->SetFullscreenState( FALSE, nullptr );
		}
		return false;
	}
	return true;
}

//-----------------------------------.
// デバイス11の作成.
//-----------------------------------.
HRESULT CDirectX11::InitDevice11()
{
#if 0	// アンチエイリアス処理.
	//デバイスの生成
	HRESULT hr;
	hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&m_pDevice11,
		NULL,
		&m_pContext11);
	if(FAILED(hr)){
		MessageBoxW(m_hWnd, L"D3D11CreateDevice", L"Err", MB_ICONSTOP);
	}

	for(int i=0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++){
		UINT Quality;
		if( SUCCEEDED(
			m_pDevice11->CheckMultisampleQualityLevels(
				DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality ))){
			if(0 < Quality){
				m_MSAASampleDesc.Count = i;
				m_MSAASampleDesc.Quality = Quality - 1;
			}
		}
	}

	//インターフェース取得
	IDXGIDevice1* hpDXGI = NULL;
	if(FAILED(m_pDevice11->QueryInterface(__uuidof(IDXGIDevice1), (void**)&hpDXGI))){
		MessageBoxW(m_hWnd, L"QueryInterface", L"Err", MB_ICONSTOP);
	}

	//アダプター取得
	IDXGIAdapter* hpAdapter = NULL;
	if(FAILED(hpDXGI->GetAdapter(&hpAdapter))){
		MessageBoxW(m_hWnd, L"GetAdapter", L"Err", MB_ICONSTOP);
	}

	//ファクトリー取得
	IDXGIFactory* hpDXGIFactory = NULL;
	hpAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&hpDXGIFactory);
	if(hpDXGIFactory == NULL){
		MessageBoxW(m_hWnd, L"GetParent", L"Err", MB_ICONSTOP);
	}

	//スワップチェイン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width	= WND_W;
	swapChainDesc.BufferDesc.Height	= WND_H;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling			= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc	= m_MSAASampleDesc;
	swapChainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount	= 1;
	swapChainDesc.OutputWindow	= m_hWnd;
	swapChainDesc.Windowed		= TRUE;
	swapChainDesc.SwapEffect	= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags			= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if(FAILED(hpDXGIFactory->CreateSwapChain(m_pDevice11, &swapChainDesc, &m_pSwapChain))){
		MessageBoxW(m_hWnd, L"CreateSwapChain", L"Err", MB_ICONSTOP);
	}
#else
	// スワップチェーン構造体.
	DXGI_SWAP_CHAIN_DESC sd = {0};
	sd.BufferCount			= 1;								// バックバッファの数.
	sd.BufferDesc.Width		= WND_W*FULL_SCREEN_MUL;			// バックバッファの幅.
	sd.BufferDesc.Height	= WND_H*FULL_SCREEN_MUL;			// バックバッファの高さ.
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
				ERROR_MESSAGE( "デバイスとスワップチェーン作成失敗" );
				return E_FAIL;
			}
		}
	}
	pFeatureLevel = nullptr;

#endif

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
	descDepth.Width					= WND_W*FULL_SCREEN_MUL;	// 幅.
	descDepth.Height				= WND_H*FULL_SCREEN_MUL;	// 高さ.
	descDepth.MipLevels				= 1;						// ミップマップレベル:1.
	descDepth.ArraySize				= 1;						// 配列数:1.
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// 32ビットフォーマット.
	descDepth.SampleDesc.Count		= 1;						// マルチサンプルの数.
	descDepth.SampleDesc.Quality	= 0;						// マルチサンプルのクオリティ.
	descDepth.Usage					= D3D11_USAGE_DEFAULT;		// 使用方法:デフォルト.
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// 深度(ステンシルとして使用).
	descDepth.CPUAccessFlags		= 0;						// CPUからはアクセスしない.
	descDepth.MiscFlags				= 0;						// その他の設定なし.
//	descDepth.SampleDesc			= m_MSAASampleDesc;


	// そのテクスチャに対してデプスステンシル(DSTex)を作成.
	if( FAILED( m_pDevice11->CreateTexture2D( &descDepth, nullptr, &m_pBackBuffer_DSTex )) ){
		ERROR_MESSAGE( "デプスステンシル作成失敗" );
		return E_FAIL;
	}
	// そのテクスチャに対してデブスステンシルビュー(DSV)を作成.
	if( FAILED( m_pDevice11->CreateDepthStencilView( m_pBackBuffer_DSTex, nullptr, &m_pBackBuffer_DSTexDSV)) ){
		ERROR_MESSAGE( "デプスステンシルビュー作成失敗" );
		return E_FAIL;
	}
	// レンダーターゲットとﾃﾞﾌﾞｽステンシルビューをパイプラインにセット.
	m_pContext11->OMSetRenderTargets( 1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV );
	return S_OK;
}

//-----------------------------------.
// ビューポートの作成.
//-----------------------------------.
HRESULT CDirectX11::InitViewports()
{
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT)WND_W*FULL_SCREEN_MUL;	// 幅.
	vp.Height	= (FLOAT)WND_H*FULL_SCREEN_MUL;	// 高さ.
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
		ERROR_MESSAGE( "ラスタライザー作成失敗" );
		SAFE_RELEASE( pRs );
		return E_FAIL;
	}
	m_pContext11->RSSetState( pRs );
	SAFE_RELEASE( pRs );

	return S_OK;
}

//--------------------------------------------.
// ブレンド作成.
//--------------------------------------------.
HRESULT CDirectX11::InitBlend()
{
	// ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ構造体.
	// pngﾌｧｲﾙ内にｱﾙﾌｧ情報があるので、透過するようにﾌﾞﾚﾝﾄﾞｽﾃｰﾄで設定する.
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory( &BlendDesc, sizeof( BlendDesc ) );

	BlendDesc.IndependentBlendEnable				= false;	
	BlendDesc.AlphaToCoverageEnable					= false;
	BlendDesc.RenderTarget[0].BlendEnable			= true;
	BlendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;	
	BlendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成.
	if( FAILED( m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaBlend ) ) ){
		ERROR_MESSAGE( "BlendState(AlphaBlend) creation failed" );
		return E_FAIL;
	}

	BlendDesc.AlphaToCoverageEnable = true;
	if( FAILED( m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaToCoverage ) ) ){
		ERROR_MESSAGE( "BlendState(AlphaToCoverage) creation failed" );
		return E_FAIL;
	}

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成.
	BlendDesc.RenderTarget[0].BlendEnable	= false;	//false:ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを使用しない.
	BlendDesc.AlphaToCoverageEnable			= false;
	if( FAILED( m_pDevice11->CreateBlendState( &BlendDesc, &m_pNoAlphaBlend ) ) ){
		ERROR_MESSAGE( "BlendState(NoAlphaBlend) creation failed" );
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------.
// 深度テスト作成.
//--------------------------------------------.
HRESULT CDirectX11::InitDeprh()
{
	// 深度テスト(zテスト)を有効にする.
	D3D11_DEPTH_STENCIL_DESC dsDesc = D3D11_DEPTH_STENCIL_DESC();

	dsDesc.DepthEnable		= TRUE;	// 有効.
	dsDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc		= D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable	= FALSE;
	dsDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	// 深度設定作成.
	if( FAILED( m_pDevice11->CreateDepthStencilState(
		&dsDesc, &m_pDepthStencilState ))){
		ERROR_MESSAGE( "Creation of depth setting failed" );
		return E_FAIL;
	}

	// 深度テスト(zテスト)を無効にする.
	dsDesc.DepthEnable = FALSE;	// 無効.
								// 深度設定作成.
	if( FAILED( m_pDevice11->CreateDepthStencilState(
		&dsDesc, &m_pDepthStencilStateOff ))){
		ERROR_MESSAGE( "Creation of depth setting failed" );
		return E_FAIL;
	}
	return S_OK;
}

//--------------------------------------------.
// ラスタライザステート作成.
//--------------------------------------------.
HRESULT CDirectX11::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode				= D3D11_FILL_SOLID;	// 塗りつぶし(ｿﾘｯﾄﾞ).
	rdc.CullMode				= D3D11_CULL_NONE;	// BACK:背面を描画しない, FRONT:正面を描画しない.
	rdc.FrontCounterClockwise	= FALSE;			// ﾎﾟﾘｺﾞﾝの表裏を決定するﾌﾗｸﾞ.
	rdc.DepthClipEnable			= FALSE;			// 距離についてのｸﾘｯﾋﾟﾝｸﾞ有効.

	auto createRasterizerState = [&]( ID3D11RasterizerState** ppRs )
	{
		if( FAILED( m_pDevice11->CreateRasterizerState( &rdc, ppRs )) ){
			_ASSERT_EXPR( false, L"ラスタライザー作成失敗" );
		}
	};
	createRasterizerState( &m_pRsSoldAndNone );

	rdc.FillMode = D3D11_FILL_SOLID;// 塗りつぶし(ｿﾘｯﾄﾞ).
	rdc.CullMode = D3D11_CULL_BACK;	// BACK:背面を描画しない, FRONT:正面を描画しない.
	createRasterizerState( &m_pRsSoldAndBack );

	rdc.FillMode = D3D11_FILL_SOLID;// 塗りつぶし(ｿﾘｯﾄﾞ).
	rdc.CullMode = D3D11_CULL_FRONT;// BACK:背面を描画しない, FRONT:正面を描画しない.
	createRasterizerState( &m_pRsSoldAndFront );

	rdc.FillMode = D3D11_FILL_WIREFRAME;// 塗りつぶし(ｿﾘｯﾄﾞ).
	rdc.CullMode = D3D11_CULL_NONE;		// BACK:背面を描画しない, FRONT:正面を描画しない.
	createRasterizerState( &m_pRsWireFrame );

	m_pContext11->RSSetState( m_pRsSoldAndNone );

	return S_OK;
}