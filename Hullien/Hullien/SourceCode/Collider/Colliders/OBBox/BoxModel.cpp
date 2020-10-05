#include "BoxModel.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\Shader\SimpleShader\SimpleShader.h"
#include "..\..\..\Common\D3DX\D3DX11.h"

CBoxModel::CBoxModel()
	: m_pSimpleShader	( std::make_unique<CSimpleShader>() )
	, m_pVertexBuffer	( nullptr )
	, m_pIndexBuffer	( nullptr )
{
}

CBoxModel::~CBoxModel()
{
	SAFE_RELEASE( m_pIndexBuffer );
	SAFE_RELEASE( m_pVertexBuffer );

	m_pSimpleShader->Release();
}

//---------------------.
// 初期化.
//---------------------.
HRESULT CBoxModel::Init( const D3DXVECTOR3& vMaxPoint, const D3DXVECTOR3& vMinPoint )
{
	InitPram( CDirectX11::GetDevice(), CDirectX11::GetContext() );

	// シェーダーの作成.
	if( FAILED( m_pSimpleShader->Init( m_pDevice11, m_pContext11 ))) return E_FAIL;
	// モデルの作成.
	if( FAILED( InitModel( vMaxPoint, vMinPoint ))) return E_FAIL;
	 
	return S_OK;
}

//---------------------.
// 描画.
//---------------------.
void CBoxModel::Render()
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
	m_pContext11->DrawIndexed( 36, 0, 0 );
	SetRasterizerState( enRS_STATE::None );
}

//---------------------.
// WVP Matrixの作成.
//---------------------.
D3DXMATRIX CBoxModel::CreateWVPMatrix( D3DXMATRIX& mWVP )
{
	// ワールド行列.
	D3DXMATRIX mWorld;
	D3DXMATRIX mRot, mTran, mScale;

	// 回転.
	D3DXMATRIX mYaw, mPitch, mRoll;
	D3DXMatrixRotationX( &mPitch, m_vRot.x );
	D3DXMatrixRotationY( &mYaw, m_vRot.y );
	D3DXMatrixRotationZ( &mRoll, m_vRot.z );
	mRot = mYaw * mPitch * mRoll;
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
HRESULT CBoxModel::InitModel( const D3DXVECTOR3& vMaxPoint, const D3DXVECTOR3& vMinPoint )
{
	//板ﾎﾟﾘ(四角形)の頂点を作成.
	CSimpleShader::VERTEX vertices[]=
	{
		//頂点座標(x,y,z)				 
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点１(左上奥).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点２(右上奥).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点３(左下奥).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y, vMaxPoint.z ), //頂点４(右下奥).

		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y, vMaxPoint.z ), //頂点１(左上手前).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点２(左下手前).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点３(右上手前).
		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y, vMaxPoint.z ), //頂点４(右下手前).

		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y, vMaxPoint.z ), //頂点１(左上手前).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点２(左上奥).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点３(左下手前).
		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点４(左下奥).

		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点１(右上手前).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点２(右下手前).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点３(右上奥).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点４(右下奥).

		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点１(左下).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点２(左上).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点３(右下).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点４(右上).

		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点１(左下).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y, vMaxPoint.z ), //頂点２(左上).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点３(右下).
		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y, vMaxPoint.z )  //頂点４(右上).
	};
	WORD indexList[]{
		0,  1,  2,     1,  0,  3,
		4,  5,  6,     5,  4,  7,
		8,  9,  10,    9,  8,  11,
		12, 13, 14,    13, 12, 15,
		16, 17, 18,    17, 16, 19,
		20, 21, 22,    21, 20, 23,
	};

	// 最大要素数を算出する.
	UINT uVerMax = sizeof(vertices)/sizeof(vertices[0]);
	UINT uIndexMax = sizeof(indexList)/sizeof(indexList[0]);

	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法(デフォルト).
	bd.ByteWidth			= sizeof(CSimpleShader::VERTEX)*uVerMax;// 頂点のサイズ.
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
	id.ByteWidth			= sizeof(WORD)*uIndexMax;  // 頂点のサイズ.
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