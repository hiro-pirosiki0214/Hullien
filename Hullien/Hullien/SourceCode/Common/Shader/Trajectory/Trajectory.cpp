#include "Trajectory.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\D3DX\D3DX11.h"

#include <vector>

CTrajectory::CTrajectory()
	: m_pLaserTexture		( nullptr )
	, m_pLaserHeadTexture	( nullptr )
	, m_pSampleLinear		( nullptr )
	, m_pVertexBuffer		( nullptr )
	, m_pHeadVertexbuffer	( nullptr )
	, m_pNoAlphaBlend		( nullptr )
	, m_pAlphaToCoverage	( nullptr )
	, m_VertexCount			( 0 )
	, m_HeadPosition		{ 0.0f, 0.0f, 0.0f }
{
}

CTrajectory::~CTrajectory()
{
}

// 初期化.
HRESULT CTrajectory::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pDevice11 = CDirectX11::GetDevice();
	m_pContext11 = CDirectX11::GetContext();

	if( FAILED( InitShader() ))		return E_FAIL;
	if( FAILED( CreateTexture() ))	return E_FAIL;
	if( FAILED( CreateSample() ))	return E_FAIL;
	if( FAILED( InitBlend() ))		return E_FAIL;
	return S_OK;
}

// 解放.
HRESULT CTrajectory::Release()
{
	SAFE_RELEASE( m_pNoAlphaBlend );
	SAFE_RELEASE( m_pAlphaToCoverage );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pLaserHeadTexture );
	SAFE_RELEASE( m_pLaserTexture );
	SAFE_RELEASE( m_pHeadVertexbuffer )
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBuffer );

	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;

	return S_OK;
}

// 描画.
void CTrajectory::Render()
{
	SetCoverage( true );
	//////////////////////////////////////////////.
	// 頭の描画.
	//////////////////////////////////////////////.
	// ワールド行列.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );	// 行列の初期化.
	// 頭の座標を設定.
	D3DXMatrixTranslation( &mWorld, m_HeadPosition.x, m_HeadPosition.y, m_HeadPosition.z );
	// ビルボードで描画.
	D3DXMATRIX CancelRotation = CCameraManager::GetViewMatrix();
	CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0.0f; // xyzを0にする.
	// CancelRotationの逆行列を求める.
	D3DXMatrixInverse( &CancelRotation, nullptr, &CancelRotation );
	mWorld = CancelRotation * mWorld;
	D3DXMATRIX mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();

	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// コンスタントバッファ.
	// シェーダーのコンスタントバッファに各種データを渡す.
	if( SUCCEEDED( m_pContext11->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		// ワールド、ビュー、プロジェクション行列を渡す.
		cb.mWVP = mWVP;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );// 行列を転置する.

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}

	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	// コンスタントバッファのセット.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ピクセルシェーダー.

	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );				// サンプラのセット.
	m_pContext11->PSSetShaderResources( 0, 1, &m_pLaserHeadTexture );	// テクスチャのセット.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	UINT stride = sizeof(VERTEX);	// データの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pHeadVertexbuffer, &stride, &offset );
	m_pContext11->Draw( 4, 0 );	// 板ポリ頂点4つ分.

	//////////////////////////////////////////////.
	// レーザーの描画.
	//////////////////////////////////////////////.
	D3DXMatrixIdentity( &mWorld );	// 行列の初期化.
	// WVPの作成.
	mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();
	// コンスタントバッファの設定.
	if( SUCCEEDED( m_pContext11->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		// ワールド、ビュー、プロジェクション行列を渡す.
		cb.mWVP = mWVP;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);// 行列を転置する.

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}
	
	m_pContext11->PSSetShaderResources( 0, 1, &m_pLaserTexture );	// テクスチャのセット.

	stride = sizeof(VERTEX); // データの間隔.
	offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pContext11->Draw( m_VertexCount, 0 );

	SetCoverage( false );
}

// 頂点バッファの取得.
void CTrajectory::CreateVertexBuffer( const std::vector<D3DXVECTOR3>& height )
{
	m_HeadPosition = height.back();		// 頭の座標を取得する.
	m_VertexCount = height.size()*2;
	std::vector<VERTEX> vertex( m_VertexCount );	// 頂点バッファの設定.
	float x = 0.0f;
	for( int i = 0; i < static_cast<int>(height.size()); i++ ){
		// 点からカメラへの単位ベクトルを求める.
		D3DXVECTOR3 z = CCameraManager::GetPosition() - height[i];
		// 点の単位接線ベクトルを求める
		D3DXVECTOR3 t;
		if( i == 0 ){
			t = height[i + 1] - height[i];
		} else if( i == height.size() - 1 ){
			t = height[i] - height[i - 1];
		} else {
			t = height[i + 1] - height[i - 1];
		}
		// ストリップ頂点を求める
		D3DXVECTOR3 cross;
		D3DXVec3Cross( &cross, &t, &z );
		D3DXVec3Normalize( &cross, &cross );
		vertex[i*2].Pos		= height[i] - cross * 1.0f;
		vertex[i*2+1].Pos	= height[i] + cross * 1.0f;

		// UV座標を求める.
		vertex[i*2].Tex		= { x / static_cast<float>(m_VertexCount/2), 1.0f };
		vertex[i*2+1].Tex	= { x / static_cast<float>(m_VertexCount/2), 0.0f };
		x += x <= 0.0f ? 2.0f : 1.0f;
	}
	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法(デフォルト).
	bd.ByteWidth			= sizeof(VERTEX) * vertex.size();	// 頂点のサイズ.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;			// 頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;	// CPUからはアクセスしない.
	bd.MiscFlags			= 0;	// その他のフラグ(未使用).
	bd.StructureByteStride	= 0;	// 構造体のサイズ(未使用).
	
	// サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &vertex[0];	// 板ポリの頂点をセット.
	
	// 頂点バッファの作成.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, 
		&InitData,
		&m_pVertexBuffer ))){
		ERROR_MESSAGE( "頂点ﾊﾞｯﾌｧ作成失敗" );
		return;
	}
}

// シェーダー作成.
HRESULT CTrajectory::InitShader()
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
			&pErrors))) {		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE("hlsl read failure");
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	// 上記で作成したブロブからシェーダーを作成.
	if( FAILED( CShaderBase::CreateVertexShader( pCompiledShader, &m_pVertexShader ))){
		ERROR_MESSAGE("VertexShader creation failed");
		return E_FAIL;
	}

	// 頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		CShaderBase::GetPositionInputElement(),
		CShaderBase::GetTexcoordInputElement(),
	};

	// 頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// 頂点インプットレイアウトを作成.
	if( FAILED(
		CShaderBase::CreateInputLayout(
			layout,
			numElements,
			pCompiledShader,
			&m_pVertexLayout ))){
		ERROR_MESSAGE("Vertex input layout creation failed");
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );


	// HLSLからピクセルシェーダーのブロブを作成.
	if( FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// シェーダーファイル名.
			"PS_Main",			// シェーダーエントリーポイント.
			"ps_5_0",			// シェーダーモデル.
			uCompileFlag,		// シェーダーコンパイルフラグ.
			&pCompiledShader,	// ブロブを格納するメモリへのポインタ.
			&pErrors ))){		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE("hlsl read failure");
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したブロブからピクセルシェーダーを作成.
	if( FAILED( CShaderBase::CreatePixelShader(pCompiledShader, &m_pPixelShader ))){
		ERROR_MESSAGE("VertexShader creation failed");
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// シェーダーに特定の数値を送るバッファ.
	// コンスタントバッファの作成.
	if( FAILED(
		CShaderBase::CreateConstantBuffer( &m_pConstantBuffer, sizeof(C_BUFFER) ))){
		ERROR_MESSAGE("Instant buffer creation failed");
		return E_FAIL;
	}

	return S_OK;
}

// テクスチャの作成.
HRESULT CTrajectory::CreateTexture()
{
	// テクスチャの作成.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,
			IMAGE_NAME,
			nullptr,
			nullptr,
			&m_pLaserTexture,
			nullptr ))){
		_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
		return E_FAIL;
	}
	// テクスチャの作成.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,
			HEAD_IMAGE_NAME,
			nullptr,
			nullptr,
			&m_pLaserHeadTexture,
			nullptr ))){
		_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
		return E_FAIL;
	}

	const float size = 1.0f;
	// 板ポリ(四角形)の頂点を作成.
	VERTEX vertices[] =
	{
		// ポリゴンの中心を頂点とする.
		// 頂点座標(x,y,z)				 UV座標(u,v)
		D3DXVECTOR3(-size, -size, 0.0f), D3DXVECTOR2( 0.0f, 1.0f ),	//頂点１(左下).
		D3DXVECTOR3(-size,  size, 0.0f), D3DXVECTOR2( 0.0f, 0.0f ),	//頂点２(左上).
		D3DXVECTOR3( size, -size, 0.0f), D3DXVECTOR2( 1.0f, 1.0f ),	//頂点３(右下).
		D3DXVECTOR3( size,  size, 0.0f), D3DXVECTOR2( 1.0f, 0.0f )	//頂点４(右上).
	};
	int vertexSize = sizeof(vertices)/sizeof(vertices[0]);
	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法(デフォルト).
	bd.ByteWidth			= sizeof(VERTEX) * vertexSize;	// 頂点のサイズ.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;			// 頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;	// CPUからはアクセスしない.
	bd.MiscFlags			= 0;	// その他のフラグ(未使用).
	bd.StructureByteStride	= 0;	// 構造体のサイズ(未使用).
	
	// サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	// 板ポリの頂点をセット.
	// 頂点バッファの作成.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pHeadVertexbuffer ))){
		_ASSERT_EXPR(false, L"頂点ﾊﾞｯﾌｧ作成失敗");
		return E_FAIL;
	}
	return S_OK;
}

// サンプラの作成.
HRESULT CTrajectory::CreateSample()
{
	// テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// サンプラ作成.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear)))
	{
		ERROR_MESSAGE("SamplerState creation failed");
		return E_FAIL;
	}
	return S_OK;
}

//--------------------------------------------.
// ブレンド作成.
//--------------------------------------------.
HRESULT CTrajectory::InitBlend()
{
	// ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ構造体.
	// pngﾌｧｲﾙ内にｱﾙﾌｧ情報があるので、透過するようにﾌﾞﾚﾝﾄﾞｽﾃｰﾄで設定する.
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory( &BlendDesc, sizeof( BlendDesc ) );

	BlendDesc.IndependentBlendEnable				= false;	
	BlendDesc.AlphaToCoverageEnable					= true;
	BlendDesc.RenderTarget[0].BlendEnable			= true;
	BlendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;	
	BlendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	BlendDesc.AlphaToCoverageEnable = true;
	if( FAILED( m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaToCoverage ) ) ){
		ERROR_MESSAGE( "BlendState(AlphaToCoverage) creation failed" );
		return E_FAIL;
	}

	BlendDesc.RenderTarget[0].BlendEnable	= false;	//false:ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを使用しない.
	BlendDesc.AlphaToCoverageEnable			= false;
	if( FAILED( m_pDevice11->CreateBlendState( &BlendDesc, &m_pNoAlphaBlend ) ) ){
		ERROR_MESSAGE( "BlendState(NoAlphaBlend) creation failed" );
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------.
// アルファカバレージを有効:無効に設定する.
//--------------------------------------------.
void CTrajectory::SetCoverage( bool EnableCoverage )
{
	// ブレンドステートの設定.
	UINT mask = 0xffffffff;	// マスク値.
	ID3D11BlendState* blend = 
		EnableCoverage == true ? m_pAlphaToCoverage : m_pNoAlphaBlend;
	m_pContext11->OMSetBlendState( blend, nullptr, mask );
}