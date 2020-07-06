#include "Font.h"
#include "..\..\Resource\FontResource\FontResource.h"

CFont::CFont()
	: m_pShader			( std::make_unique<CSpriteShader>() )
	, m_pVertexBuffer	( nullptr )
	, m_pSampleLinear	( nullptr )
	, m_pResourceView	( nullptr )
{
}

CFont::~CFont()
{
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pResourceView );
}

HRESULT CFont::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitPram( pDevice11, pContext11 ))) return E_FAIL;
	if( FAILED( m_pShader->Init( pDevice11, pContext11 ))) return E_FAIL;
	if( FAILED( CreateVerTexBuffer() )) return E_FAIL;
	if( FAILED( CreateSampleLinear() )) return E_FAIL;

	return S_OK;
}

void CFont::Render( const std::string& text )
{
	// 文字数分ループ.
	for( int i = 0; i < static_cast<int>(text.length()); i++ ){
		std::string f = text.substr( i, 1 );
		if( IsDBCSLeadByte( text[i] ) == TRUE ){
			f = text.substr( i++, 2 );	// 全角文字.
		} else {
			f = text.substr( i, 1 );	// 半角文字.
		}
		RenderFont( CFontResource::GetTexture2D(f) );
		m_vPos.x += (FONT_SIZE*2.1f) * m_vScale.x;
	}
}

void CFont::RenderFont( ID3D11Texture2D* texture )
{
	// リソースビューの作成.
	CreateResourceView( texture );

	// シェーダーに各値を設定.
	m_pShader->SetConstantBufferData( CreateWorldMatrix(), m_vColor, TEXTUR_UV_POS );
	m_pShader->ShaderUISet( m_pVertexBuffer );

	// プリミティブトポロジー.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// テクスチャをシェーダーに渡す.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pResourceView );

	// 描画.
	SetBlend( true );
	SetRasterizerState( enRS_STATE::Back );
	m_pContext11->Draw( 4, 0 );
	SetRasterizerState( enRS_STATE::None );
	SetBlend( false );
}

//---------------------------------.
// バーテックスバッファの作成.
//---------------------------------.
HRESULT CFont::CreateVerTexBuffer()
{
	CSpriteShader::VERTEX vertices[] =
	{
		D3DXVECTOR3(-FONT_SIZE,  FONT_SIZE, 0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
		D3DXVECTOR3(-FONT_SIZE, -FONT_SIZE, 0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
		D3DXVECTOR3( FONT_SIZE,  FONT_SIZE, 0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
		D3DXVECTOR3( FONT_SIZE, -FONT_SIZE, 0.0f ),	D3DXVECTOR2( 1.0f, 0.0f )
	};
	// 最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法.
	bd.ByteWidth			= sizeof(CSpriteShader::VERTEX)*uVerMax;	//頂点のサイズ.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;		// CPUからはアクセスしない.
	bd.MiscFlags			= 0;		// その他のフラグ(未使用).
	bd.StructureByteStride	= 0;		// 構造体のサイズ(未使用).

	// サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;		// 板ポリの頂点をセット.

	// 頂点バッファの作成.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ) ) ){
		_ASSERT_EXPR( false, L"頂点ﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}
	return S_OK;
}

//---------------------------------.
// サンプラーの作成.
//---------------------------------.
HRESULT CFont::CreateSampleLinear()
{
	// シェーダ用にサンプラを作成する.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof(samDesc) );
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_pDevice11->CreateSamplerState( &samDesc, &m_pSampleLinear );

	return S_OK;
}

//---------------------------------.
// リソースビューの作成.
//---------------------------------.
HRESULT CFont::CreateResourceView( ID3D11Texture2D* texture )
{
	// テクスチャ情報を取得する.
	D3D11_TEXTURE2D_DESC texDesc;
	texture->GetDesc( &texDesc );

	// ShaderResourceViewの情報を作成する.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof(srvDesc) );
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	m_pDevice11->CreateShaderResourceView( texture, &srvDesc, &m_pResourceView );

	return S_OK;
}

//---------------------------------.
// ワールドマトリックスの作成.
//---------------------------------.
D3DXMATRIX CFont::CreateWorldMatrix()
{
	// ワールド行列, スケール行列, 回転行列, 平行移動行列.
	D3DXMATRIX mScale, mRot, mTran;

	// 拡大縮小行列作成.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, 1.0f );
	// 回転行列を作成.
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.x, m_vRot.y, m_vRot.z );
	// 平行移動行列.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );

	// ワールド行列作成.
	return mScale * mRot * mTran;
}