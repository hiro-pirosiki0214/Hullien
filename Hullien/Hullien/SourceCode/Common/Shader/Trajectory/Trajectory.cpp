#include "Trajectory.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\D3DX\D3DX11.h"

#include <vector>

CTrajectory::CTrajectory()
	: m_pTexture			( nullptr )
	, m_pSampleLinear		( nullptr )
	, m_pHeightVertexBuffer	( nullptr )
	, m_pWidthVertexBuffer	( nullptr )
	, m_VertexCount			( 0 )
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
	return S_OK;
}

// 解放.
HRESULT CTrajectory::Release()
{
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pWidthVertexBuffer );
	SAFE_RELEASE( m_pHeightVertexBuffer );
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
	// ワールド行列.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );	// 行列の初期化.
	// ビルボードで描画.
	D3DXMATRIX CancelRotation = CCameraManager::GetViewMatrix();
	CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0.0f; // xyzを0にする.
	// CancelRotationの逆行列を求める.
	D3DXMatrixInverse( &CancelRotation, nullptr, &CancelRotation );
//	mWorld = CancelRotation * mWorld;
	// WVPの作成.
	D3DXMATRIX mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();

	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// コンスタントバッファ.
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData))) {

		// ワールド、ビュー、プロジェクション行列を渡す.
		cb.mWVP = mWVP;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);// 行列を転置する.

		memcpy_s(
			pData.pData,
			pData.RowPitch,
			(void*)(&cb),
			sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}
	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.

	// このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ピクセルシェーダー.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pTexture );


	UINT stride = sizeof(VERTEX); // データの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pHeightVertexBuffer, &stride, &offset );
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	m_pContext11->Draw( m_VertexCount, 0 );

	//stride = sizeof(VERTEX); // データの間隔.
	//offset = 0;
	//m_pContext11->IASetVertexBuffers( 0, 1, &m_pWidthVertexBuffer, &stride, &offset );
	//m_pContext11->IASetInputLayout( m_pVertexLayout );
	//m_pContext11->Draw( m_VertexCount, 0 );
}

// 頂点バッファの取得.
void CTrajectory::CreateVertexBuffer( 
	const std::list<std::pair<D3DXVECTOR3,D3DXVECTOR3>>& height,
	const std::list<std::pair<D3DXVECTOR3,D3DXVECTOR3>>& width )
{
//	CreateVertexBuffer( height, &m_pHeightVertexBuffer, true );	// 高さ.
//	CreateVertexBuffer( width, &m_pWidthVertexBuffer, false );	// 幅.

	//
	// ビールボードビームはテクスチャが不安定なので放置.
	//
	std::vector<D3DXVECTOR3> pos;
	for( const auto& p : height ){
		pos.emplace_back( D3DXVECTOR3( 
			p.first.x, 
			p.first.y-1.0f,
			p.first.z ) );
	}
	std::vector<VERTEX> vertex(pos.size()*2);
	m_VertexCount = pos.size()*2;
	float x = 0.0f;
	float cal = 1.0f;
	for( int i = 0; i < pos.size(); i++ ){
		// 点からカメラへの単位ベクトルを求める.
		D3DXVECTOR3 z = CCameraManager::GetPosition() - pos[i];
		// 点の単位接線ベクトルを求める
		D3DXVECTOR3		t;
		if( i == 0 ){
			t = pos[i + 1] - pos[i];
		} else if( i == pos.size() - 1 ){
			t = pos[i] - pos[i - 1];
		} else {
			t = pos[i + 1] - pos[i - 1];
		}
		// ストリップ頂点を求める
		D3DXVECTOR3		cross;
		D3DXVec3Cross( &cross, &t, &z );
		D3DXVec3Normalize( &cross, &cross );
		vertex[i * 2].Pos = pos[i] - cross * cal;
		vertex[i * 2 + 1].Pos = pos[i] + cross * cal;
		cal = static_cast<float>(m_VertexCount/2)*0.1f;
		vertex[i * 2].Tex.x = x / static_cast<float>(m_VertexCount/2);
		vertex[i * 2].Tex.y = 1.0f;
		vertex[i * 2 + 1].Tex.x = x / static_cast<float>(m_VertexCount/2);
		vertex[i * 2 + 1].Tex.y = 0.0f;
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
		&m_pHeightVertexBuffer ))){
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
			&m_pTexture,
			nullptr ))){
		_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
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

// 頂点バッファの作成.
void CTrajectory::CreateVertexBuffer( 
	const std::list<std::pair<D3DXVECTOR3,D3DXVECTOR3>>& vertexPoint,
	ID3D11Buffer** ppHeightVertexBuffer,
	const bool& isHeight )
{
	std::vector<VERTEX> vertex;
	float x = 0.0f;
	float size = static_cast<float>(vertexPoint.size());
	m_VertexCount = 0;	// 頂点初期化.
	for( auto& v : vertexPoint ){
		vertex.emplace_back();
		vertex.back().Pos = v.second;
		vertex.back().Tex.x = x / size;
		vertex.back().Tex.y = 1.0f;
		m_VertexCount++;	// 頂点加算.

		vertex.emplace_back();
		vertex.back().Pos = v.first;
		vertex.back().Tex.x = x / size;
		vertex.back().Tex.y = isHeight == true ? 0.0f : 1.0f;
		m_VertexCount++;	// 頂点加算.
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
		ppHeightVertexBuffer ))){
		ERROR_MESSAGE( "頂点ﾊﾞｯﾌｧ作成失敗" );
		return;
	}
}
