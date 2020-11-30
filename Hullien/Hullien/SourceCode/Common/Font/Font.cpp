#include "Font.h"
#include "..\..\Resource\FontResource\FontResource.h"
#include "..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\Camera\CameraManager\CameraManager.h"

CFont::CFont()
	: m_pShader			( std::make_unique<CSpriteShader>() )
	, m_pVertexBuffer	( nullptr )
	, m_pVertexBufferUI	( nullptr )
	, m_pSampleLinear	( nullptr )
{
}

CFont::~CFont()
{
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pSampleLinear );
}

HRESULT CFont::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitPram( pDevice11, pContext11 ))) return E_FAIL;
	if( FAILED( m_pShader->Init( pDevice11, pContext11 ))) return E_FAIL;
	if( FAILED( CreateVerTexBuffer() )) return E_FAIL;
	if( FAILED( CreateSampleLinear() )) return E_FAIL;

	return S_OK;
}

void CFont::RenderUI( const std::string& text )
{
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ) return;
	m_vScale.y = m_vScale.y*1.2f;
	// 文字数分ループ.
	for( int i = 0; i < static_cast<int>(text.length()); i++ ){
		std::string f = text.substr( i, 1 );
		if( IsDBCSLeadByte( text[i] ) == TRUE ){
			f = text.substr( i++, 2 );	// 全角文字.
		} else {
			f = text.substr( i, 1 );	// 半角文字.
		}
		RenderFontUI( f.c_str() );
		m_vPos.x += (FONT_SIZE*2.1f) * m_vScale.x;
	}
}

void CFont::Render( const std::string& text, const bool& isBillboard )
{
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ) return;
	if( IsDBCSLeadByte( text[0] ) == TRUE ){
		m_vPos.x += m_vScale.x * (static_cast<float>(text.length())*0.5f)-m_vScale.x;
	} else {
		m_vPos.x += m_vScale.x*2.0f * (static_cast<float>(text.length())*0.5f)-m_vScale.x;
	}
	// 文字数分ループ.
	for( int i = 0; i < static_cast<int>(text.length()); i++ ){
		std::string f = text.substr( i, 1 );
		if( IsDBCSLeadByte( text[i] ) == TRUE ){
			f = text.substr( i++, 2 );	// 全角文字.
		} else {
			f = text.substr( i, 1 );	// 半角文字.
		}
		RenderFont( f.c_str(), isBillboard );
		m_vPos.x -= m_vScale.x*2.0f;
	}
}

void CFont::RenderFontUI( const char* c )
{
	// シェーダーに各値を設定.
	m_pShader->SetConstantBufferData( CreateWorldMatrix(), m_vColor, TEXTUR_UV_POS );
	m_pShader->ShaderUISet( m_pVertexBufferUI );

	// プリミティブトポロジー.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// テクスチャをシェーダーに渡す.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	ID3D11ShaderResourceView* pResourceView = CFontResource::GetTexture2D(c);	// リソースビュー.
	m_pContext11->PSSetShaderResources( 0, 1, &pResourceView );

	// 描画.
	SetBlend( true );
	SetRasterizerState( enRS_STATE::Back );
	m_pContext11->Draw( 4, 0 );
	SetRasterizerState( enRS_STATE::None );
	SetBlend( false );
}

void CFont::RenderFont( const char* c, const bool& isBillboard  )
{
	//　ワールド行列.
	D3DXMATRIX mWorld;
	// ワールド行列作成.
	mWorld = CreateWorldMatrix();
	if( isBillboard == true ){
		// ビルボード用.
		D3DXMATRIX CancelRotation = CCameraManager::GetViewMatrix();
		CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0.0f; // xyzを0にする.
		// CancelRotationの逆行列を求める.
		D3DXMatrixInverse(&CancelRotation, nullptr, &CancelRotation);
		mWorld = CancelRotation * mWorld;
	}
	// WVPの作成.
	D3DXMATRIX mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();

	// シェーダーに各値を設定.
	m_pShader->SetConstantBufferData( mWVP, m_vColor, TEXTUR_UV_POS );
	m_pShader->ShaderSet( m_pVertexBuffer );

	// プリミティブトポロジー.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// テクスチャをシェーダーに渡す.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	ID3D11ShaderResourceView* pResourceView = CFontResource::GetTexture2D(c);	// リソースビュー.
	m_pContext11->PSSetShaderResources( 0, 1, &pResourceView );

	// 描画.
	SetRasterizerState( enRS_STATE::Back );
	m_pContext11->Draw( 4, 0 );
	SetRasterizerState( enRS_STATE::None );
}

//---------------------------------.
// バーテックスバッファの作成.
//---------------------------------.
HRESULT CFont::CreateVerTexBuffer()
{
	CSpriteShader::VERTEX verticesUI[] =
	{
		D3DXVECTOR3( 0.0f,			 FONT_SIZE*2.0f,	0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
		D3DXVECTOR3( 0.0f,			 0.0f,			0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
		D3DXVECTOR3( FONT_SIZE*2.0f, FONT_SIZE*2.0f,0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
		D3DXVECTOR3( FONT_SIZE*2.0f, 0.0f,			0.0f ),	D3DXVECTOR2( 1.0f, 0.0f )
	};
	// 最大要素数を算出する.
	UINT uVerMax = sizeof(verticesUI) / sizeof(verticesUI[0]);

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
	InitData.pSysMem = verticesUI;		// 板ポリの頂点をセット.

	// 頂点バッファの作成.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBufferUI ) ) ){
		_ASSERT_EXPR( false, L"頂点ﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}

	CSpriteShader::VERTEX vertices[] =
	{
		D3DXVECTOR3(-1.0,  1.0f, 0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
		D3DXVECTOR3(-1.0, -1.0f, 0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
		D3DXVECTOR3( 1.0,  1.0f, 0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
		D3DXVECTOR3( 1.0, -1.0f, 0.0f ),	D3DXVECTOR2( 1.0f, 0.0f )
	};
	// 最大要素数を算出する.
	uVerMax = sizeof(vertices) / sizeof(vertices[0]);
	bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法.
	bd.ByteWidth			= sizeof(CSpriteShader::VERTEX)*uVerMax;	//頂点のサイズ.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;		// CPUからはアクセスしない.
	bd.MiscFlags			= 0;		// その他のフラグ(未使用).
	bd.StructureByteStride	= 0;		// 構造体のサイズ(未使用).

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
// ワールドマトリックスの作成.
//---------------------------------.
D3DXMATRIX CFont::CreateWorldMatrix()
{
	// ワールド行列, スケール行列, 回転行列, 平行移動行列.
	D3DXMATRIX mScale, mRot, mTran;

	// 拡大縮小行列作成.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, 1.0f );
	// 回転行列を作成.
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.y, m_vRot.x, m_vRot.z );
	// 平行移動行列.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );

	// ワールド行列作成.
	return mScale * mRot * mTran;
}

//---------------------------------.
// WVPの作成.
//---------------------------------.
D3DXMATRIX CFont::CreateWVPMatrix()
{
	// ワールド行列, スケール行列, 回転行列, 平行移動行列.
	D3DXMATRIX mScale, mRot, mTran;
	// 拡大縮小行列作成.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, 1.0f );
	// 回転行列を作成.
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.x, m_vRot.y, m_vRot.z );
	// 平行移動行列.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );

	D3DXMATRIX mWorld = mScale * mRot * mTran;
	// ワールド行列作成.
	return mWorld*CCameraManager::GetViewMatrix()*CCameraManager::GetProjMatrix();
}