#include "PeraRenderer.h"
#include "..\D3DX\D3DX11.h"

CPeraRenderer::CPeraRenderer()
	: m_pVertexShader	( nullptr )
	, m_pPixelShader	( nullptr )
	, m_pVertexLayout	( nullptr )
	, m_pConstantBuffer	( nullptr )
	, m_pVertexBuffer	( nullptr )
	, m_pSampleLinear	( nullptr )
{
}

CPeraRenderer::~CPeraRenderer()
{
	Release();
}

// 初期化.
HRESULT CPeraRenderer::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	m_pDevice11 = CDirectX11::GetDevice();
	m_pContext11 = CDirectX11::GetContext();

	// シェーダ作成.
	if( FAILED( CreateShader() ))return E_FAIL;
	// 板ポリゴン作成.
	if( FAILED( CreateModel() )) return E_FAIL;
	// サンプラの作成.
	if( FAILED( InitSample() )) return E_FAIL;

	return S_OK;
}

// 解放.
void CPeraRenderer::Release()
{
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

// 描画関数.
void CPeraRenderer::Render( std::vector<ID3D11ShaderResourceView*> gbuffers )
{
	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// コンスタントバッファ.

	D3DXMATRIX mW;

	// 平行移動行列.
	D3DXMATRIX mTran;
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, 0.0f );

	// ワールド行列作成.
	mW = mTran;

	// バッファ内のデータの書き換え開始時にMap.
	if( SUCCEEDED( m_pContext11->Map( 
		m_pConstantBuffer, 
		0, 
		D3D11_MAP_WRITE_DISCARD, 
		0, 
		&pData ))){

		cb.mW = mW;
		D3DXMatrixTranspose( &cb.mW, &cb.mW );
		// ビューポートの幅,高さを渡す.
		cb.vViewPort.x	= static_cast<float>(WND_W);
		cb.vViewPort.y	= static_cast<float>(WND_H);

		memcpy_s(
			pData.pData, 
			pData.RowPitch,
			(void*)(&cb),
			sizeof(cb) );

		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}
	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.
																// このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ピクセルシェーダー.

																	// 頂点バッファをセット.
	UINT stride = sizeof(VERTEX); // データの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	// 頂点インプットレイアウトをセット.
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );

	// G-Bufferテクスチャをシェーダーに渡す.
	m_pContext11->PSSetShaderResources( 0, 1, &gbuffers[0] );
	m_pContext11->PSSetShaderResources( 1, 1, &gbuffers[1] );
	m_pContext11->PSSetShaderResources( 2, 1, &gbuffers[2] );

	// ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pContext11->Draw( 4, 0 );//板ﾎﾟﾘ(頂点4つ分).
}

// シェーダ作成.
HRESULT CPeraRenderer::CreateShader()
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
HRESULT CPeraRenderer::InitSample()
{
	// テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	samDesc.MinLOD			= 0;
	samDesc.MaxLOD			= D3D11_FLOAT32_MAX;
	// サンプラ作成.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear ) ) ){
		ERROR_MESSAGE( "SamplerState creation failed" );
		return E_FAIL;
	}


	D3D11_BUFFER_DESC cb;
	cb.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth			= sizeof(C_BUFFER);
	cb.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags			= 0;
	cb.StructureByteStride	= 0;
	cb.Usage				= D3D11_USAGE_DYNAMIC;

	if( FAILED(  m_pDevice11->CreateBuffer( &cb, nullptr, &m_pConstantBuffer ))){
		ERROR_MESSAGE( "Buffer creation failed" );
		return E_FAIL;
	}

	return S_OK;
}


// モデル作成.
HRESULT CPeraRenderer::CreateModel()
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
