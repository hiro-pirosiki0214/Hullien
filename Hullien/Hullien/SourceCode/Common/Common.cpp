#include "Common.h"

CCommon::CCommon()
	: m_pDevice11				( nullptr )
	, m_pContext11				( nullptr )
	, m_pDepthStencilState		( nullptr )
	, m_pDepthStencilStateOff	( nullptr )
	, m_pAlphaBlend				( nullptr )
	, m_pNoAlphaBlend			( nullptr )
	, m_pAlphaToCoverage		( nullptr )
	, m_pRsSoldAndNone			( nullptr )
	, m_pRsSoldAndBack			( nullptr )
	, m_pRsSoldAndFront			( nullptr )
	, m_pRsWireFrame			( nullptr )
	, m_vPos					( 0.0f, 0.0f, 0.0f )
	, m_vRot					( 0.0f, 0.0f, 0.0f )
	, m_vScale					( 1.0f, 1.0f, 1.0f )
	, m_vColor					( 1.0f, 1.0f, 1.0f, 1.0f )
{
}

CCommon::~CCommon()
{
	SAFE_RELEASE( m_pDepthStencilState );
	SAFE_RELEASE( m_pDepthStencilStateOff );
	SAFE_RELEASE( m_pAlphaBlend );
	SAFE_RELEASE( m_pNoAlphaBlend );
	SAFE_RELEASE( m_pAlphaToCoverage );
	SAFE_RELEASE( m_pRsSoldAndNone );
	SAFE_RELEASE( m_pRsSoldAndBack );
	SAFE_RELEASE( m_pRsSoldAndFront );
	SAFE_RELEASE( m_pRsWireFrame );

	m_pContext11	= nullptr;
	m_pDevice11		= nullptr;
}

//--------------------------------------------.
// デバイスの取得、各情報の初期化.
//--------------------------------------------.
HRESULT CCommon::InitPram( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pDevice11		= pDevice11;
	m_pContext11	= pContext11;

	if( FAILED( InitBlend()) ){
		ERROR_MESSAGE( "Blend creation failed" );
		return E_FAIL;
	}
	if( FAILED( InitDeprh()) ){
		ERROR_MESSAGE( "Depth test creation failed" );
		return E_FAIL;
	}
	if( FAILED( InitRasterizerState()) ){
		ERROR_MESSAGE( "RasterizerState creation failed" );
		return E_FAIL;
	}
	return S_OK;
}

//--------------------------------------------.
// ブレンドを有効:無効に設定する.
//--------------------------------------------.
void CCommon::SetBlend( bool EnableAlpha )
{
	// ブレンドステートの設定.
	UINT mask = 0xffffffff;	// マスク値.
	ID3D11BlendState* blend = 
		EnableAlpha == true ? m_pAlphaBlend : m_pNoAlphaBlend;
	m_pContext11->OMSetBlendState( blend, nullptr, mask );
}

//--------------------------------------------.
// アルファカバレージを有効:無効に設定する.
//--------------------------------------------.
void CCommon::SetCoverage( bool EnableCoverage )
{
	// ブレンドステートの設定.
	UINT mask = 0xffffffff;	// マスク値.
	ID3D11BlendState* blend = 
		EnableCoverage == true ? m_pAlphaToCoverage : m_pNoAlphaBlend;
	m_pContext11->OMSetBlendState( blend, nullptr, mask );
}

//--------------------------------------------.
// 深度テストを有効:無効に設定する.
//--------------------------------------------.
void CCommon::SetDeprh( bool flag )
{
	ID3D11DepthStencilState* pTmp
		= ( flag == true ) ? m_pDepthStencilState : m_pDepthStencilStateOff;
	// 深度設定をセット.
	m_pContext11->OMSetDepthStencilState( pTmp, 1 );
}

//--------------------------------------------.
// ラスタライザステート設定.
//--------------------------------------------.
void CCommon::SetRasterizerState( const enRS_STATE& rsState )
{
	switch( rsState )
	{
	case enRS_STATE::None:	// 正背面描画.
		m_pContext11->RSSetState( m_pRsSoldAndNone );
		break;
	case enRS_STATE::Back:	// 背面を描画しない.
		m_pContext11->RSSetState( m_pRsSoldAndBack );
		break;
	case enRS_STATE::Front:	// 正面を描画しない.
		m_pContext11->RSSetState( m_pRsSoldAndFront );
		break;
	case enRS_STATE::Wire:	// ワイヤーフレーム描画.
		m_pContext11->RSSetState( m_pRsWireFrame );
		break;
	default:
		break;
	}
}

//--------------------------------------------.
// ブレンド作成.
//--------------------------------------------.
HRESULT CCommon::InitBlend()
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
HRESULT CCommon::InitDeprh()
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
HRESULT CCommon::InitRasterizerState()
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