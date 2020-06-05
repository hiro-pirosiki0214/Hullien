#include "SphereModel.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Common\Shader\SimpleShader\SimpleShader.h"
#include "..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"

CSphereModel::CSphereModel()
	: m_pSimpleShader	( std::make_unique<CSimpleShader>() )
	, m_pVertexBuffer	( nullptr )
	, m_pIndexBuffer	( nullptr )
{
}

CSphereModel::~CSphereModel()
{
	SAFE_RELEASE( m_pIndexBuffer );
	SAFE_RELEASE( m_pVertexBuffer );

	m_pSimpleShader->Release();
}

//---------------------.
// 初期化.
//---------------------.
HRESULT CSphereModel::Init( const float& radius )
{
	InitPram( CDirectX11::GetDevice(), CDirectX11::GetContext() );

	// シェーダーの作成.
	if( FAILED( m_pSimpleShader->Init( m_pDevice11, m_pContext11 ))) return E_FAIL;
	// モデルの作成.
	if( FAILED( InitModel( radius ))) return E_FAIL;

	return S_OK;
}

//---------------------.
// 描画.
//---------------------.
void CSphereModel::Render()
{
	D3DXMATRIX mWVP;
	// WVP Matrixの作成.
	CreateWVPMatrix( mWVP );

	// 現在の色を設定.
	D3DXVECTOR4 color = m_IsColorChanged == false ? MODEL_COLOR : MODEL_CHANGE_COLOR;

	// コンスタントバッファにデータを渡す.
	m_pSimpleShader->SetConstantBufferData( mWVP, color );

	// 各種シェーダの設定.
	m_pSimpleShader->ShaderSet( m_pVertexBuffer, m_pIndexBuffer );

	// プリミティブトポロジーをセット.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	SetRasterizerState( enRS_STATE::Wire );
	m_pContext11->DrawIndexed( 930, 0, 0 );
	SetRasterizerState( enRS_STATE::None );
}

//---------------------.
// WVP Matrixの作成.
//---------------------.
D3DXMATRIX CSphereModel::CreateWVPMatrix( D3DXMATRIX& mWVP )
{
	// ワールド行列.
	D3DXMATRIX mWorld;
	D3DXMATRIX mRot, mTran, mScale;

	// 回転.
	D3DXMatrixRotationYawPitchRoll( &mRot, 0.0f, 0.0f, 0.0f );
	// 平行移動.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );
	// 拡大縮小行列作成.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );
	// ワールド行列
	mWorld = mScale * mRot * mTran;
	
	mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();

	return mWVP;
}

//---------------------.
// モデルの作成.
//---------------------.
HRESULT CSphereModel::InitModel( const float& fRadius )
{
	const int U_MAX = 30;
	const int V_MAX = 15;

	// 頂点作成.
	CSimpleShader::VERTEX vertices[480];
	WORD vertex_num = U_MAX * ( V_MAX + 1 );
	for( int v = 0; v <= V_MAX; v++ ){
		for( int u = 0; u < U_MAX; u++ ){
			float theta	= static_cast<float>(D3DXToRadian( 180.0 * v / V_MAX ));
			float phi	= static_cast<float>(D3DXToRadian( 360.0f * u / U_MAX ));
			float x = sin( theta ) * cos( phi ) * fRadius;
			float y = cos( theta ) * fRadius;
			float z = sin( theta ) * sin( phi ) * fRadius;
			vertices[U_MAX * v + u].Pos = D3DXVECTOR3( x, y, z );
		}
	}

	// インデックス作成.
	WORD indexList[930];
	WORD index_num = 2 * V_MAX * ( U_MAX + 1 );
	int i = 0;
	for( int v = 0; v < V_MAX; v++ ){
		for( int u = 0; u <= U_MAX; u++ ){
			if( u == U_MAX ){
				indexList[i++] = v * U_MAX;
				indexList[i++] = ( v + 1 ) * U_MAX;
			} else{
				indexList[i++] = ( v * U_MAX ) + u;
				indexList[i++] = indexList[i - 1] + U_MAX;
			}
		}
	}

	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法(デフォルト).
	bd.ByteWidth			= sizeof(CSimpleShader::VERTEX)*vertex_num;// 頂点のサイズ.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;						// CPUからはアクセスしない.
	bd.MiscFlags			= 0;						// その他のフラグ(未使用).
	bd.StructureByteStride	= 0;						// 構造体のサイズ(未使用).

	//ｻﾌﾞﾘｿｰｽﾃﾞｰﾀ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	//板ﾎﾟﾘの頂点をｾｯﾄ.

	//頂点ﾊﾞｯﾌｧの作成.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ) ) ){
		_ASSERT_EXPR( false, L"頂点ﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}

	D3D11_BUFFER_DESC id;
	id.Usage				= D3D11_USAGE_DEFAULT;	   // 使用方法(デフォルト).
	id.ByteWidth			= sizeof(WORD)*index_num;  // 頂点のサイズ.
	id.BindFlags			= D3D11_BIND_INDEX_BUFFER; // 頂点バッファとして扱う.
	id.CPUAccessFlags		= 0;					   // CPUからはアクセスしない.
	id.MiscFlags			= 0;					   // その他のフラグ(未使用).
	id.StructureByteStride	= 0;					   // 構造体のサイズ(未使用).

	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem			= indexList;
	IndexData.SysMemPitch		= 0;
	IndexData.SysMemSlicePitch	= 0;

	if( FAILED( m_pDevice11->CreateBuffer(
		&id, &IndexData, &m_pIndexBuffer ))){
		_ASSERT_EXPR( false, L"インデックスﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}

	return S_OK;
}