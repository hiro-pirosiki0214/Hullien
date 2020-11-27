#include "SceneTexRenderer.h"
#include "..\D3DX\D3DX11.h"

const int FULL_SCREEN_MUL = 1;

CSceneTexRenderer::CSceneTexRenderer()
	: m_pDevice11				( nullptr )
	, m_pContext11				( nullptr )
	, m_pShadowBufferRTV		( MAX_CASCADE )
	, m_pShadowBufferSRV		( MAX_CASCADE )
	, m_pShadowBufferTex		( MAX_CASCADE )
	, m_pGBufferRTV				( EGBufferNo::enGBufferNo_MAX )
	, m_pGBufferSRV				( EGBufferNo::enGBufferNo_MAX )
	, m_pGBufferTex				( EGBufferNo::enGBufferNo_MAX )
	, m_pTransBufferRTV			( nullptr )
	, m_pTransBufferSRV			( nullptr )
	, m_pTransBufferTex			( nullptr )
	, m_pAntialiasingRTV		( nullptr )
	, m_pAntialiasingSRV		( nullptr )
	, m_pAntialiasingTex		( nullptr )
	, m_pVertexShader			( nullptr )
	, m_pPixelShader			( nullptr )
	, m_pLastPixelShader		( nullptr )
	, m_pVertexLayout			( nullptr )
	, m_pConstantBuffer			( nullptr )
	, m_pVertexBuffer			( nullptr )
	, m_pSampleLinear			( nullptr )
	, m_NowRenderPass			( ERenderPass::Shadow )
{
}

CSceneTexRenderer::~CSceneTexRenderer()
{
	Release();
}

// インスタンスの取得.
CSceneTexRenderer* CSceneTexRenderer::GetInstance()
{
	static std::unique_ptr<CSceneTexRenderer> pInstance = std::make_unique<CSceneTexRenderer>();
	return pInstance.get();
}

// 初期化.
HRESULT CSceneTexRenderer::Init()
{
	GetInstance()->m_pDevice11 = CDirectX11::GetDevice();
	GetInstance()->m_pContext11 = CDirectX11::GetContext();

	if( FAILED( GetInstance()->InitShadowBufferTex()) )		return E_FAIL;
	if( FAILED( GetInstance()->InitGBufferTex()))			return E_FAIL;
	if( FAILED( GetInstance()->InitTransBufferTex()))		return E_FAIL;
	if( FAILED( GetInstance()->InitAntialiasingTex()))		return E_FAIL;
	if( FAILED( GetInstance()->CreateShader() ))			return E_FAIL;
	if( FAILED( GetInstance()->CreateModel() ))				return E_FAIL;
	if( FAILED( GetInstance()->InitSample() ))				return E_FAIL;
	if( FAILED( GetInstance()->CreateConstantBuffer() ))	return E_FAIL;

	return S_OK;
}

// 解放.
void CSceneTexRenderer::Release()
{
	for( auto& rtv : GetInstance()->m_pGBufferRTV ) SAFE_RELEASE(rtv);
	for( auto& srv : GetInstance()->m_pGBufferSRV ) SAFE_RELEASE(srv);
	for( auto& tex : GetInstance()->m_pGBufferTex ) SAFE_RELEASE(tex);

	for( auto& rtv : GetInstance()->m_pShadowBufferRTV ) SAFE_RELEASE(rtv);
	for( auto& srv : GetInstance()->m_pShadowBufferSRV ) SAFE_RELEASE(srv);
	for( auto& tex : GetInstance()->m_pShadowBufferTex ) SAFE_RELEASE(tex);

	SAFE_RELEASE( GetInstance()->m_pAntialiasingSRV );
	SAFE_RELEASE( GetInstance()->m_pAntialiasingTex );
	SAFE_RELEASE( GetInstance()->m_pAntialiasingRTV );

	SAFE_RELEASE( GetInstance()->m_pTransBufferSRV );
	SAFE_RELEASE( GetInstance()->m_pTransBufferTex );
	SAFE_RELEASE( GetInstance()->m_pTransBufferRTV );

	SAFE_RELEASE( GetInstance()->m_pSampleLinear );
	SAFE_RELEASE( GetInstance()->m_pVertexBuffer );
	SAFE_RELEASE( GetInstance()->m_pConstantBuffer );
	SAFE_RELEASE( GetInstance()->m_pPixelShader );
	SAFE_RELEASE( GetInstance()->m_pVertexLayout );
	SAFE_RELEASE( GetInstance()->m_pVertexShader );

	GetInstance()->m_pContext11 = nullptr;
	GetInstance()->m_pDevice11 = nullptr;
}

// 描画関数.
void CSceneTexRenderer::Render()
{
	// レンダーターゲットの設定.
	GetInstance()->m_pContext11->OMSetRenderTargets( 1, &GetInstance()->m_pAntialiasingRTV, CDirectX11::GetDepthSV() );
	// デプスステンシルバッファ.
	GetInstance()->m_pContext11->ClearDepthStencilView( CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// 使用するシェーダのセット.
	GetInstance()->m_pContext11->VSSetShader( GetInstance()->m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	GetInstance()->m_pContext11->PSSetShader( GetInstance()->m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.
	GetInstance()->m_pContext11->PSSetSamplers( 0, 1, &GetInstance()->m_pSampleLinear );	// サンプラのセット.

	// コンスタントバッファの値は変化しないので、
	// 初めに設定したコンスタントバッファを送る.
	GetInstance()->m_pContext11->VSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );	// 頂点シェーダ.
	GetInstance()->m_pContext11->PSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );	// ピクセルシェーダー.

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	GetInstance()->m_pContext11->IASetVertexBuffers( 0, 1, &GetInstance()->m_pVertexBuffer, &stride, &offset );
	GetInstance()->m_pContext11->IASetInputLayout( GetInstance()->m_pVertexLayout );
	GetInstance()->m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	for( int i = 0; i < EGBufferNo::enGBufferNo_MAX; i++ ){
		GetInstance()->m_pContext11->PSSetShaderResources( i, 1, &GetInstance()->m_pGBufferSRV[i] );// G-Buffer.
	}
	GetInstance()->m_pContext11->PSSetShaderResources( 3, 1, &GetInstance()->m_pTransBufferSRV );	// Trans.
	GetInstance()->m_pContext11->Draw( 4, 0 );

	CDirectX11::SetBackBuffer();
	GetInstance()->m_pContext11->PSSetShader( GetInstance()->m_pLastPixelShader, nullptr, 0 );	// ピクセルシェーダ.
	GetInstance()->m_pContext11->PSSetShaderResources( 4, 1, &GetInstance()->m_pAntialiasingSRV );	// Trans.
	GetInstance()->m_pContext11->Draw( 4, 0 );
}

// バッファのクリア.
void CSceneTexRenderer::ClearBuffer()
{
	// 深度バッファテクスチャのクリア.
	for( auto& rtv : GetInstance()->m_pShadowBufferRTV ){
		GetInstance()->m_pContext11->ClearRenderTargetView( rtv, GetInstance()->CLEAR_BACK_COLOR );
	}
	// G-Bufferテクスチャのクリア.
	for( auto& rtv : GetInstance()->m_pGBufferRTV ){
		GetInstance()->m_pContext11->ClearRenderTargetView( rtv, GetInstance()->CLEAR_BACK_COLOR );
	}
	// Transテクスチャのクリア.
	GetInstance()->m_pContext11->ClearRenderTargetView( 
		GetInstance()->m_pTransBufferRTV, GetInstance()->CLEAR_BACK_COLOR );
	// Antialiasingテクスチャのクリア.
	GetInstance()->m_pContext11->ClearRenderTargetView( 
		GetInstance()->m_pAntialiasingRTV, GetInstance()->CLEAR_BACK_COLOR );
}

// Shadowバッファの設定.
void CSceneTexRenderer::SetShadowBuffer( int i )
{
	// レンダーターゲットの設定.
	GetInstance()->m_pContext11->OMSetRenderTargets( 
		1, 
		&GetInstance()->m_pShadowBufferRTV[i], 
		CDirectX11::GetDepthSV() );
	
	// デプスステンシルバッファ.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

// G-Bufferの設定.
void CSceneTexRenderer::SetGBuffer()
{
	// レンダーターゲットの設定.
	GetInstance()->m_pContext11->OMSetRenderTargets( 
		EGBufferNo::enGBufferNo_MAX,
		&GetInstance()->m_pGBufferRTV[0],
		CDirectX11::GetDepthSV() );
	// デプスステンシルバッファ.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

// TransBufferの設定.
void CSceneTexRenderer::SetTransBuffer()
{
	// レンダーターゲットの設定.
	GetInstance()->m_pContext11->OMSetRenderTargets( 
		1, 
		&GetInstance()->m_pTransBufferRTV,
		CDirectX11::GetDepthSV() );
	// デプスステンシルバッファ.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

//-----------------------------------.
// ShadowBufferの作成.
//-----------------------------------.
HRESULT CSceneTexRenderer::InitShadowBufferTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= WND_W*FULL_SCREEN_MUL;		// 幅.
	texDesc.Height				= WND_H*FULL_SCREEN_MUL;		// 高さ.
	texDesc.MipLevels			= 1;							// ミップマップレベル:1.
	texDesc.ArraySize			= 1;							// 配列数:1.
	texDesc.SampleDesc.Count	= 1;							// 32ビットフォーマット.
	texDesc.SampleDesc.Quality	= 0;							// マルチサンプルの数.
	texDesc.Format				= DXGI_FORMAT_B8G8R8A8_UNORM;	// マルチサンプルのクオリティ.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;			// 使用方法:デフォルト.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
	texDesc.CPUAccessFlags		= 0;							// CPUからはアクセスしない.
	texDesc.MiscFlags			= 0;							// その他の設定なし.

	for( int i = 0; i < MAX_CASCADE; i++ ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pShadowBufferRTV[i],
			&m_pShadowBufferSRV[i],
			&m_pShadowBufferTex[i] ))) return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------.
// G-Bufferの作成.
//-----------------------------------.
HRESULT CSceneTexRenderer::InitGBufferTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= WND_W*FULL_SCREEN_MUL;			// 幅.
	texDesc.Height				= WND_H*FULL_SCREEN_MUL;			// 高さ.
	texDesc.MipLevels			= 1;								// ミップマップレベル:1.
	texDesc.ArraySize			= 1;								// 配列数:1.
	texDesc.Format				= DXGI_FORMAT_R16G16B16A16_FLOAT;	// 32ビットフォーマット.
	texDesc.SampleDesc.Count	= 1;								// マルチサンプルの数.
	texDesc.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法:デフォルト.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
	texDesc.CPUAccessFlags		= 0;								// CPUからはアクセスしない.
	texDesc.MiscFlags			= 0;								// その他の設定なし.

	for( int i = 0; i < EGBufferNo::enGBufferNo_MAX-1; i++ ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pGBufferRTV[i],
			&m_pGBufferSRV[i],
			&m_pGBufferTex[i] ))) return E_FAIL;
	}
	// 深度用のフォーマットに変更.
	texDesc.Format				= DXGI_FORMAT_R32_FLOAT;// 32ﾋﾞｯﾄﾌｫｰﾏｯﾄ.
	if( FAILED( CreateBufferTex(
		texDesc,
		&m_pGBufferRTV[EGBufferNo::enGBufferNo_Z_DEPTH],
		&m_pGBufferSRV[EGBufferNo::enGBufferNo_Z_DEPTH],
		&m_pGBufferTex[EGBufferNo::enGBufferNo_Z_DEPTH] ))) return E_FAIL;
	return S_OK;
}

// TransBufferの作成.
HRESULT CSceneTexRenderer::InitTransBufferTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= WND_W*FULL_SCREEN_MUL;			// 幅.
	texDesc.Height				= WND_H*FULL_SCREEN_MUL;			// 高さ.
	texDesc.MipLevels			= 1;								// ミップマップレベル:1.
	texDesc.ArraySize			= 1;								// 配列数:1.
	texDesc.Format				= DXGI_FORMAT_R11G11B10_FLOAT;		// 32ビットフォーマット.
	texDesc.SampleDesc.Count	= 1;								// マルチサンプルの数.
	texDesc.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法:デフォルト.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
	texDesc.CPUAccessFlags		= 0;								// CPUからはアクセスしない.
	texDesc.MiscFlags			= 0;								// その他の設定なし.

	if( FAILED( CreateBufferTex(
		texDesc,
		&m_pTransBufferRTV,
		&m_pTransBufferSRV,
		&m_pTransBufferTex ))) return E_FAIL;
	return S_OK;
}

// Antialiasingの作成.
HRESULT CSceneTexRenderer::InitAntialiasingTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= WND_W*FULL_SCREEN_MUL;			// 幅.
	texDesc.Height				= WND_H*FULL_SCREEN_MUL;			// 高さ.
	texDesc.MipLevels			= 1;								// ミップマップレベル:1.
	texDesc.ArraySize			= 1;								// 配列数:1.
	texDesc.Format				= DXGI_FORMAT_R11G11B10_FLOAT;		// 32ビットフォーマット.
	texDesc.SampleDesc.Count	= 1;								// マルチサンプルの数.
	texDesc.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法:デフォルト.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
	texDesc.CPUAccessFlags		= 0;								// CPUからはアクセスしない.
	texDesc.MiscFlags			= 0;								// その他の設定なし.

	if( FAILED( CreateBufferTex(
		texDesc,
		&m_pAntialiasingRTV,
		&m_pAntialiasingSRV,
		&m_pAntialiasingTex ))) return E_FAIL;
	return S_OK;
}

// シェーダ作成.
HRESULT CSceneTexRenderer::CreateShader()
{
	ID3DBlob* pCompileVS = nullptr;
	ID3DBlob* pCompilePS = nullptr;
	ID3DBlob* pErrorMsg = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG
	//----------------------------.
	// 頂点シェーダー.
	//----------------------------.
	if( FAILED(
		D3DX10CompileFromFile( 
			SHADER_NAME, 
			nullptr, 
			nullptr, 
			"VS_Main",
			"vs_5_0", 
			uCompileFlag,
			0,
			nullptr,
			&pCompileVS, 
			&pErrorMsg,
			nullptr ))){
		ERROR_MESSAGE( "vs hlsl Loading Failure." );
		return E_FAIL;
	}
	if( FAILED(
		m_pDevice11->CreateVertexShader(
			pCompileVS->GetBufferPointer(),
			pCompileVS->GetBufferSize(),
			nullptr,
			&m_pVertexShader ))){
		ERROR_MESSAGE( "vs hlsl Creating Failure." );
		return E_FAIL;
	}
	//----------------------------.
	// ピクセルシェーダー.
	//----------------------------.
	if( FAILED(
		D3DX10CompileFromFile( 
			SHADER_NAME, 
			nullptr, 
			nullptr, 
			"PS_Main", 
			"ps_5_0", 
			uCompileFlag, 
			0,
			nullptr,
			&pCompilePS,
			&pErrorMsg,
			nullptr ))){
		ERROR_MESSAGE( (char*)pErrorMsg->GetBufferPointer()  );
		return E_FAIL;
	}
	if( FAILED(
		m_pDevice11->CreatePixelShader(
			pCompilePS->GetBufferPointer(),
			pCompilePS->GetBufferSize(),
			nullptr,
			&m_pPixelShader ))){
		ERROR_MESSAGE( "ps hlsl Creating Failure." );
		return E_FAIL;
	}

	//----------------------------.
	// ピクセルシェーダー.
	//----------------------------.
	if( FAILED(
		D3DX10CompileFromFile( 
			SHADER_NAME,
			nullptr,
			nullptr,
			"PS_LastMain", 
			"ps_5_0", 
			uCompileFlag, 
			0,
			nullptr,
			&pCompilePS,
			&pErrorMsg,
			nullptr ))){
		ERROR_MESSAGE( (char*)pErrorMsg->GetBufferPointer()  );
		return E_FAIL;
	}
	if( FAILED(
		m_pDevice11->CreatePixelShader(
			pCompilePS->GetBufferPointer(),
			pCompilePS->GetBufferSize(),
			nullptr,
			&m_pLastPixelShader ))){
		ERROR_MESSAGE( "ps hlsl Creating Failure." );
		return E_FAIL;
	}
	//----------------------------.
	//	頂点インプット.
	//----------------------------.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int numElements = sizeof(layout)/sizeof(layout[0]);	// 要素数算出.
	if( FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompileVS->GetBufferPointer(),
			pCompileVS->GetBufferSize(),
			&m_pVertexLayout ))){
		ERROR_MESSAGE( "vs layout Creating Failure." );
		return E_FAIL;
	}

	SAFE_RELEASE( pCompileVS );
	SAFE_RELEASE( pCompilePS );

	return S_OK;
}

// サンプラの作成.
HRESULT CSceneTexRenderer::InitSample()
{
	// テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU		= D3D11_TEXTURE_ADDRESS_CLAMP;		// UV値が[0,1]を超えたら,[0,1]に設定する.
	samDesc.AddressV		= D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressW		= D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	samDesc.MinLOD			= 0;
	samDesc.MaxLOD			= D3D11_FLOAT32_MAX;
	// サンプラ作成.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear ) ) ){
		ERROR_MESSAGE( "SamplerState creation failed" );
		return E_FAIL;
	}
	return S_OK;
}

// コンスタントバッファの作成.
HRESULT CSceneTexRenderer::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth			= sizeof(C_BUFFER);
	cbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags			= 0;
	cbDesc.StructureByteStride	= 0;
	cbDesc.Usage				= D3D11_USAGE_DYNAMIC;

	if( FAILED(  m_pDevice11->CreateBuffer( &cbDesc, nullptr, &m_pConstantBuffer ))){
		ERROR_MESSAGE( "Buffer creation failed" );
		return E_FAIL;
	}

	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;

	// バッファ内のデータの書き換え開始時にMap.
	if( FAILED( m_pContext11->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
		ERROR_MESSAGE( "ConstantBuffer Map failed" );
		return E_FAIL;
	}

	C_BUFFER cb;	// コンスタントバッファ.
	D3DXMatrixIdentity( &cb.mW );
	D3DXMatrixTranspose( &cb.mW, &cb.mW );
	// ビューポートの幅,高さを渡す.
	cb.vViewPort.x	= static_cast<float>(WND_W);
	cb.vViewPort.y	= static_cast<float>(WND_H);

	// メモリ領域をコピー.
	memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
	m_pContext11->Unmap( m_pConstantBuffer, 0 );

	return S_OK;
}

// モデル作成.
HRESULT CSceneTexRenderer::CreateModel()
{
	// 板ポリ(四角形)の頂点を作成.
	VERTEX vertices[]=
	{
		// 頂点座標(x,y,z)				 
		D3DXVECTOR3( 0.0f, WND_H, 0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
		D3DXVECTOR3( WND_W, WND_H, 0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
		D3DXVECTOR3( WND_W, 0.0f, 0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),
	};
	// 最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法(デフォルト).
	bd.ByteWidth			= sizeof(VERTEX) * uVerMax;	// 頂点のサイズ.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;	// CPUからはアクセスしない.
	bd.MiscFlags			= 0;	// その他のフラグ(未使用).
	bd.StructureByteStride	= 0;	// 構造体のサイズ(未使用).

									// サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	// 板ポリの頂点をセット.

									// 頂点バッファの作成.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, 
		&InitData, 
		&m_pVertexBuffer))){
		ERROR_MESSAGE( "頂点ﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}

	return S_OK;
}

// バッファの作成.
HRESULT CSceneTexRenderer::CreateBufferTex(
	const D3D11_TEXTURE2D_DESC	texDesc,
	ID3D11RenderTargetView**	ppRTV,
	ID3D11ShaderResourceView**	ppSRV,
	ID3D11Texture2D**			ppTex )
{
	// そのテクスチャに対してデプスステンシル(DSTex)を作成.
	if( FAILED( m_pDevice11->CreateTexture2D( &texDesc, nullptr, ppTex )) ){
		_ASSERT_EXPR( false, L"テクスチャデスク作成失敗" );
		return E_FAIL;
	}
	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset( &rtvDesc, 0, sizeof( rtvDesc ) );
	rtvDesc.Format             = texDesc.Format;
	rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	// RenderTargetView作成.
	if( FAILED( m_pDevice11->CreateRenderTargetView( *ppTex, &rtvDesc, ppRTV ) )){
		_ASSERT_EXPR( false, L"RenderTargetView作成失敗" );
		return E_FAIL;
	}

	// シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset( &srvDesc, 0, sizeof( srvDesc ) );
	srvDesc.Format              = rtvDesc.Format;
	srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	// テクスチャ作成時と同じフォーマット
	if( FAILED( m_pDevice11->CreateShaderResourceView( *ppTex, &srvDesc, ppSRV ) )){
		_ASSERT_EXPR( false, L"デプスステンシル作成失敗" );
		return E_FAIL;
	}
	return S_OK;
}