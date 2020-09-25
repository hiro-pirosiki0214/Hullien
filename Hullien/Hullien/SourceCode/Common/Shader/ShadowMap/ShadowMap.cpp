#include "ShadowMap.h"
#include "..\..\D3DX\D3DX11.h"
#include "..\..\..\Light\LightManager\LightManager.h"

CShadowMap::CShadowMap()
	: m_pVertexSkinShader	( nullptr )
	, m_pVertexSkinLayout	( nullptr )
{
}

CShadowMap::~CShadowMap()
{
	GetInstance()->Release();
}

CShadowMap* CShadowMap::GetInstance()
{
	static std::unique_ptr<CShadowMap> pInstance = 
		std::make_unique<CShadowMap>();
	return pInstance.get();
}

// 初期化.
HRESULT CShadowMap::Init()
{
	GetInstance()->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() );
	return S_OK;
}

// 解放.
HRESULT CShadowMap::ReleaseShader()
{
	GetInstance()->Release();
	return S_OK;
}

void CShadowMap::SetConstantBufferData( const D3DXMATRIX& mLWVP, const bool isSkin )
{
	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// コンスタントバッファ.

	GetInstance()->m_pContext11->IASetInputLayout( GetInstance()->m_pVertexLayout );

	// 使用するシェーダのセット.
	GetInstance()->m_pContext11->VSSetShader( GetInstance()->m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	GetInstance()->m_pContext11->PSSetShader( GetInstance()->m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.
	if( isSkin == true ){
		GetInstance()->m_pContext11->VSSetShader( GetInstance()->m_pVertexSkinShader, nullptr, 0 );	// 頂点シェーダ.
		GetInstance()->m_pContext11->IASetInputLayout( GetInstance()->m_pVertexSkinLayout );
	}

	// バッファ内のデータの書き換え開始時にMap.
	if( SUCCEEDED( GetInstance()->m_pContext11->Map( 
		GetInstance()->m_pConstantBuffer, 
		0, 
		D3D11_MAP_WRITE_DISCARD, 
		0, 
		&pData ))){

		cb.mLightWVP = mLWVP;
		//※行列の計算方法がDirectXとGPUで異なるため転置が必要.
		D3DXMatrixTranspose( &cb.mLightWVP, &cb.mLightWVP ); // 行列を転置する.

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );

		// バッファ内のデータの書き換え終了時にUnmap.
		GetInstance()->m_pContext11->Unmap( GetInstance()->m_pConstantBuffer, 0 );
	}

	// このコンスタントバッファをどのシェーダで使用するか？.
	GetInstance()->m_pContext11->VSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );	// 頂点シェーダ.
	GetInstance()->m_pContext11->PSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );	// ピクセルシェーダー.
}

//-------------------------------------.
// 各種シェーダの設定.
//-------------------------------------.
void CShadowMap::ShaderSet( ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer )
{
	// 使用するシェーダのセット.
	GetInstance()->m_pContext11->VSSetShader( GetInstance()->m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	GetInstance()->m_pContext11->PSSetShader( GetInstance()->m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.

																							// このコンスタントバッファをどのシェーダで使用するか？.
	GetInstance()->m_pContext11->VSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );	// 頂点シェーダ.
	GetInstance()->m_pContext11->PSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );	// ピクセルシェーダー.

																									// 頂点インプットレイアウトをセット.
	GetInstance()->m_pContext11->IASetInputLayout( GetInstance()->m_pVertexLayout );
}
// 初期化.
HRESULT CShadowMap::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if( FAILED( InitShader())) return E_FAIL;
	return S_OK;
}

// 解放.
HRESULT CShadowMap::Release()
{
	SAFE_RELEASE( m_pVertexSkinShader );
	SAFE_RELEASE( m_pVertexSkinLayout );
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBuffer );

	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;

	return S_OK;
}

// シェーダー作成.
HRESULT CShadowMap::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSLからバーテックスシェーダーのブロブを作成.
	if( FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// シェーダーファイル名.
			"VS_Main",			// シェーダーエントリーポイント.
			"vs_5_0",			// シェーダーモデル.
			uCompileFlag,		// シェーダーコンパイルフラグ.
			&pCompiledShader,	// ブロブを格納するメモリへのポインタ.
			&pErrors ))){		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE( "hlsl read failure" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	// 上記で作成したﾌﾞﾛﾌﾞから「ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ」を作成.
	if( FAILED( CShaderBase::CreateVertexShader( pCompiledShader, &m_pVertexShader ))){
		ERROR_MESSAGE( "VertexShader creation failed" );
		return E_FAIL;
	}

	// 頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{  
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	// 頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	// 頂点インプットレイアウトを作成.
	if( FAILED(
		CShaderBase::CreateInputLayout(
			layout,
			numElements,
			pCompiledShader,
			&m_pVertexLayout ))){
		ERROR_MESSAGE( "Vertex input layout creation failed" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// HLSLからバーテックスシェーダーのブロブを作成.
	if( FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// シェーダーファイル名.
			"VS_SkinMain",			// シェーダーエントリーポイント.
			"vs_5_0",			// シェーダーモデル.
			uCompileFlag,		// シェーダーコンパイルフラグ.
			&pCompiledShader,	// ブロブを格納するメモリへのポインタ.
			&pErrors ))){		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE( "hlsl read failure" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	// 上記で作成したﾌﾞﾛﾌﾞから「ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ」を作成.
	if( FAILED( CShaderBase::CreateVertexShader( pCompiledShader, &m_pVertexSkinShader ))){
		ERROR_MESSAGE( "VertexShader creation failed" );
		return E_FAIL;
	}

	// 頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC skinLayout[] = 
	{  
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONE_INDEX",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONE_WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// 頂点インプットレイアウトの配列要素数を算出.
	numElements = sizeof( skinLayout ) / sizeof( skinLayout[0] );

	// 頂点インプットレイアウトを作成.
	if( FAILED(
		CShaderBase::CreateInputLayout(
			skinLayout,
			numElements,
			pCompiledShader,
			&m_pVertexLayout ))){
		ERROR_MESSAGE( "Vertex input layout creation failed" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	//HLSLからﾋﾟｸｾﾙｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if( FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// シェーダーファイル名.
			"PS_Main",			// シェーダーエントリーポイント.
			"ps_5_0",			// シェーダーモデル.
			uCompileFlag,		// シェーダーコンパイルフラグ.
			&pCompiledShader,	// ブロブを格納するメモリへのポインタ.
			&pErrors ))){		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE( "hlsl read failure" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したﾌﾞﾛﾌﾞから「ﾋﾟｸｾﾙｼｪｰﾀﾞ」を作成.
	if( FAILED( CShaderBase::CreatePixelShader( pCompiledShader, &m_pPixelShader ))){
		ERROR_MESSAGE( "VertexShader creation failed" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// シェーダーに特定の数値を送るバッファ.
	// コンスタントバッファの作成.
	if( FAILED(
		CShaderBase::CreateConstantBuffer( &m_pConstantBuffer, sizeof(C_BUFFER) ))){
		ERROR_MESSAGE( "Instant buffer creation failed" );
		return E_FAIL;
	}

	return S_OK;
}