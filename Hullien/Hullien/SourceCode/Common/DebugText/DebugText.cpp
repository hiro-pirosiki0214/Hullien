#include "DebugText.h"
#include "..\Shader\DebugTextShader\DebugTextShader.h"

CDebugText::CDebugText()
	: m_pVertexBuffer	()
	, m_pSampleLinear	( nullptr )
	, m_pAsciiTexture	( nullptr )
	, m_pShader			( std::make_unique<CDebugTextShader>() )
	, m_fKerning		()
	, m_vColor			( 0.0f, 0.0f, 0.0f, 0.0f )
{
}

CDebugText::~CDebugText()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CDebugText* CDebugText::GetInstance()
{
	static std::unique_ptr<CDebugText> pInstance =
		std::make_unique<CDebugText>();	// インスタンスの作成.
	return pInstance.get();
}

//----------------------------.
// 初期化関数.
//----------------------------.
HRESULT CDebugText::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	float Scale, 
	D3DXVECTOR4 vColor )
{
	GetInstance()->InitPram( pDevice11, pContext11 );

	if( FAILED( GetInstance()->m_pShader->Init( pDevice11, pContext11 ))){
		return E_FAIL;
	}
	if( FAILED( GetInstance()->InitModel())) return E_FAIL;
	if( FAILED( GetInstance()->InitSampleLinear())) return E_FAIL;
	if( FAILED( GetInstance()->InitTexture())) return E_FAIL;

	GetInstance()->m_fAlpha = vColor.w;
	GetInstance()->m_vColor = vColor;
	float scale = Scale / 25.0f;
	GetInstance()->m_vScale = { scale, scale, scale };

	return S_OK;
}

//----------------------------.
// 描画関数.
//----------------------------.
void CDebugText::Render( const std::string& stext )
{
	// 文字数分ループ.
	for( int i = 0; i < static_cast<int>(stext.length()); i++ ){
		char font = stext[i];
		int index = font - 32;	// フォントインデックス作成.

		// フォントレンダリング.
		GetInstance()->RenderFont( index, GetInstance()->m_vPos.x, GetInstance()->m_vPos.y );
		GetInstance()->m_vPos.x += GetInstance()->m_fKerning[index] * GetInstance()->m_vScale.x;
	}
}

//----------------------------.
// フォントレンダリング関数.
//----------------------------.
void CDebugText::RenderFont( int FontIndex, float x, float y )
{
	D3DXMATRIX mWorld, mView, mProj;
	D3DXMATRIX mScale, mTran;
	D3DXMatrixIdentity( &mWorld );
	D3DXMatrixIdentity( &mProj );

	// ビュートランスフォーム.
	D3DXVECTOR3 vEye( 0.0f, 0.0f, -1.0f );
	D3DXVECTOR3 vLook( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUp( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &mView, &vEye, &vLook, &vUp );

	mProj._11 =  2.0f / static_cast<float>(WND_W);
	mProj._22 = -2.0f / static_cast<float>(WND_H);
	mProj._41 = -1.0f;
	mProj._42 =  1.0f;

	// 平行移動.
	D3DXMatrixTranslation( &mTran, x,  y, -100.0f );

	// 拡大縮小行列作成.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );
	mWorld = mScale * mTran;

	D3DXMATRIX mWVP = mWorld * mView * mProj;

	m_pShader->SetConstantBufferData( mWVP, m_vColor );
	m_pShader->ShaderSet( m_pVertexBuffer[FontIndex], m_pSampleLinear, m_pAsciiTexture );

	// プリミティブトポロジー.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	//描画.
	SetCoverage( true );
	SetRasterizerState( enRS_STATE::Back );
	m_pContext11->Draw( 4, 0 );
	SetRasterizerState( enRS_STATE::None );
	SetCoverage( false );
}

//----------------------------.
// モデルの作成.
//----------------------------.
HRESULT CDebugText::InitModel()
{
	// 95文字分繰り返し.
	for( int i = 0; i < 95; i++ ){
		m_fKerning[i] = 10.0f;
	}

	float left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
	int cnt = 0;
	// 2重ループで1文字ずつ指定する.
	for( int k = 0; k < 10; k++ ){
		for( int i = 0; i < 10; i++ ){
			left	= (k * WDIMENSION);
			top		= (i * DIMENSION);
			right	= (left + m_fKerning[cnt]);
			bottom	= (top + DIMENSION);

			left	/= TEX_DIMENSION;
			top		/= TEX_DIMENSION;
			right	/= TEX_DIMENSION;
			bottom	/= TEX_DIMENSION;

			CDebugTextShader::VERTEX vertices[] =
			{
				D3DXVECTOR3( 0.0f,       DIMENSION, 0.0f ),	D3DXVECTOR2( left,  bottom ),
				D3DXVECTOR3( 0.0f,       0.0f,      0.0f ),	D3DXVECTOR2( left,  top ),
				D3DXVECTOR3( WDIMENSION, DIMENSION, 0.0f ),	D3DXVECTOR2( right, bottom ),
				D3DXVECTOR3( WDIMENSION, 0.0f,      0.0f ),	D3DXVECTOR2( right, top )
			};
			// 最大要素数を算出する.
			UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

			// バッファ構造体.
			D3D11_BUFFER_DESC bd;
			bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法.
			bd.ByteWidth			= sizeof(CDebugTextShader::VERTEX)*uVerMax;	//頂点のサイズ.
			bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして扱う.
			bd.CPUAccessFlags		= 0;		// CPUからはアクセスしない.
			bd.MiscFlags			= 0;		// その他のフラグ(未使用).
			bd.StructureByteStride	= 0;		// 構造体のサイズ(未使用).

			// サブリソースデータ構造体.
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;		// 板ポリの頂点をセット.

			// 頂点バッファの作成.
			if( FAILED( m_pDevice11->CreateBuffer(
				&bd, &InitData, &m_pVertexBuffer[cnt] ) ) ){
				_ASSERT_EXPR( false, L"頂点ﾊﾞｯﾌｧ作成失敗" );
				return E_FAIL;
			}
			cnt++;
		}
	}
	return S_OK;
}

//----------------------------.
// サンプラの作成.
//----------------------------.
HRESULT CDebugText::InitSampleLinear()
{
	// テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter		= D3D11_FILTER_MIN_MAG_MIP_POINT;	// POINT:高速だが粗い.
	samDesc.AddressU	= D3D11_TEXTURE_ADDRESS_WRAP;		// ラッピング(WRAP:繰り返し).
	samDesc.AddressV	= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW	= D3D11_TEXTURE_ADDRESS_WRAP;

	// サンプラ作成.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear ))){ // (out)サンプラ.
		_ASSERT_EXPR( false, L"ｻﾝﾌﾟﾗ作成失敗" );
		return E_FAIL;
	}
	return S_OK;
}

//----------------------------.
// テクスチャ作成.
//----------------------------.
HRESULT CDebugText::InitTexture()
{
	// テクスチャ作成.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,					// リソースを使用するデバイスのポインタ.
		"Data\\Texture\\_DebugText\\ascii.png",	// ファイル名.
		nullptr, 
		nullptr,
		&m_pAsciiTexture,
		nullptr ))){
		_ASSERT_EXPR( false, L"ﾃｸｽﾁｬ作成失敗" );
		return E_FAIL;
	}
	return S_OK;
}