#include "CapsuleModel.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\Shader\SimpleShader\SimpleShader.h"
#include "..\..\..\Common\D3DX\D3DX11.h"

CCapsuleModel::CCapsuleModel()
	: m_pSimpleShader	( std::make_unique<CSimpleShader>() )
	, m_pVertexBuffer	( nullptr )
	, m_pIndexBuffer	( nullptr )
	, index_num			( 0 )
{
}

CCapsuleModel::~CCapsuleModel()
{
	SAFE_RELEASE( m_pIndexBuffer );
	SAFE_RELEASE( m_pVertexBuffer );

	m_pSimpleShader->Release();
}

//---------------------.
// 初期化.
//---------------------.
HRESULT CCapsuleModel::Init( const float& fRadius, const float& fHeight )
{
	InitPram( CDirectX11::GetDevice(), CDirectX11::GetContext() );

	// シェーダーの作成.
	if( FAILED( m_pSimpleShader->Init( m_pDevice11, m_pContext11 ))) return E_FAIL;
	// モデルの作成.
	if( FAILED( InitModel( fRadius, fHeight ))) return E_FAIL;

	return S_OK;
}

//---------------------.
// 描画.
//---------------------.
void CCapsuleModel::Render()
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
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ );

	SetRasterizerState( enRS_STATE::Wire );
	m_pContext11->DrawIndexed( index_num*2, 0, 0 );
	SetRasterizerState( enRS_STATE::None );
}

//---------------------.
// WVP Matrixの作成.
//---------------------.
D3DXMATRIX CCapsuleModel::CreateWVPMatrix( D3DXMATRIX& mWVP )
{
	// ワールド行列.
	D3DXMATRIX mWorld;
	D3DXMATRIX mRot, mTran, mScale;

	// 回転.
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.y, m_vRot.x, m_vRot.z );
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
HRESULT CCapsuleModel::InitModel( const float& fRadius, const float& fHeight )
{
	int divideH = 20, divideV = 20;
	divideH = divideH < 4 ? 4 : divideH;
	divideV = divideV < 4 ? 4 : divideV;
	float radius = fRadius;
	float height = fHeight;

	// 偶数のみ有効.
	if( divideV % 2 != 0 ) divideV++;

	int cnt = 0;

	// =============================
	// 頂点座標作成
	// =============================
	int vertCount = (divideH * divideV + 2);
	D3DXVECTOR3* vertices = new D3DXVECTOR3[vertCount];

	// 中心角
	float centerEulerRadianH = 2.0f * 3.14f / (float) divideH;
	float centerEulerRadianV = 2.0f * 3.14f / (float) divideV;

	float offsetHeight = height * 0.5f;

	// 天面
	vertices[cnt++] = D3DXVECTOR3( 0, radius+offsetHeight, 0 );

	// カプセル上部
	for( int vv = 0; vv < divideV / 2; vv++ ){
		float vRadian = (float)(vv + 1) * centerEulerRadianV / 2.0f;
		// 1辺の長さ
		float tmpLen = fabsf( sinf(vRadian) * radius );
		float y = cosf(vRadian) * radius;

		for( int vh = 0; vh < divideH; vh++ ){
			D3DXVECTOR3 pos = 
			{
				tmpLen * sinf( (float)vh * centerEulerRadianH ),
				y + offsetHeight,
				tmpLen * cosf( (float) vh * centerEulerRadianH )
			};
			// サイズ反映
			vertices[cnt++] = pos;
		}
	}

	// カプセル下部
	int offset = divideV / 2;
	for( int vv = 0; vv < divideV / 2; vv++ ){
		float yRadian = (float)(vv + offset) * centerEulerRadianV / 2.0f;
		// 1辺の長さ
		float tmpLen = fabsf( sinf(yRadian) * radius );
		float y = cosf(yRadian) * radius;

		for( int vh = 0; vh < divideH; vh++ ){

			D3DXVECTOR3 pos = 
			{
				tmpLen * sinf( (float)vh * centerEulerRadianH ),
				y - offsetHeight,
				tmpLen * cosf( (float)vh * centerEulerRadianH )
			};
			// サイズ反映
			vertices[cnt++] = pos;
		}
	}
	// 底面
	vertices[cnt] = D3DXVECTOR3( 0, -radius-offsetHeight, 0 );

	// =============================
	// 頂点インデックス作成
	// =============================
	int topAndBottomTriCount = divideH * 2;
	// 側面三角形の数
	int aspectTriCount = divideH * (divideV - 2 + 1) * 2;

	int* indices = new int[(topAndBottomTriCount + aspectTriCount) * 3];

	//天面
	int offsetIndex = 0;
	cnt = 0;
	for (int i = 0; i < divideH * 3; i++)
	{
		if( i % 3 == 0 ){
			indices[cnt++] = 0;
		} else if ( i % 3 == 1 ){
			indices[cnt++] = 1 + offsetIndex;
		} else if ( i % 3 == 2 ){
			int index = 2 + offsetIndex++;
			// 蓋をする
			index = index > divideH ? indices[1] : index;
			indices[cnt++] = index;
		}
	}

	// 側面Index
	/* 頂点を繋ぐイメージ
	* 1 - 2
	* |   |
	* 0 - 3
	* 
	* 0, 1, 2
	* 0, 2, 3
	*
	* 注意 : 1周した時にClampするのを忘れないように。
	*/
	// 開始Index番号
	int startIndex = indices[1];

	// 天面、底面を除いたカプセルIndex要素数
	int sideIndexLen = aspectTriCount * 3;

	int lap1stIndex = 0;
	int lap2ndIndex = 0;

	// 一周したときのindex数
	int lapDiv = divideH * 2 * 3;

	int createSquareFaceCount = 0;

	for( int i = 0; i < sideIndexLen; i++ ){
		// 一周の頂点数を超えたら更新(初回も含む)
		if( i % lapDiv == 0 ){
			lap1stIndex = startIndex;
			lap2ndIndex = startIndex + divideH;
			createSquareFaceCount++;
		}

		if( i % 6 == 0 || i % 6 == 3 ){
			indices[cnt++] = startIndex;
		} else if ( i % 6 == 1 ){
			indices[cnt++] = startIndex + divideH;
		} else if ( i % 6 == 2 || i % 6 == 4 ){
			if( i > 0 &&
				( i % (lapDiv * createSquareFaceCount - 2) == 0 || 
					i % (lapDiv * createSquareFaceCount - 4) == 0 )){
				// 1周したときのClamp処理
				// 周回ポリゴンの最後から2番目のIndex
				indices[cnt++] = lap2ndIndex;
			} else {
				indices[cnt++] = startIndex + divideH + 1;
			}
		} else if ( i % 6 == 5 ){
			if(( i > 0 && i % (lapDiv * createSquareFaceCount - 1 )) == 0 ){
				// 1周したときのClamp処理
				// 周回ポリゴンの最後のIndex
				indices[cnt++] = lap1stIndex;
			} else {
				indices[cnt++] = startIndex + 1;
			}

			// 開始Indexの更新
			startIndex++;
		} else {
		}
	}

	// 底面Index
	offsetIndex = vertCount - 1 - divideH;
	lap1stIndex = offsetIndex;
	int finalIndex = vertCount - 1;
	int len = divideH * 3;

	for( int i = len - 1; i >= 0; i-- ){
		if( i % 3 == 0 ){
			// 底面の先頂点
			indices[cnt++] = finalIndex;
			offsetIndex++;
		} else if ( i % 3 == 1 ){
			indices[cnt++] = offsetIndex;
		} else if ( i % 3 == 2 ){
			int value = 1 + offsetIndex;
			if( value >= vertCount - 1 ){
				value = lap1stIndex;
			}
			indices[cnt++] = value;
		}
	}
	
	// 最大要素数を算出する.
	int vertex_num = vertCount;
	index_num = (topAndBottomTriCount + aspectTriCount) * 3;

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
	InitData.pSysMem = vertices;

	//頂点ﾊﾞｯﾌｧの作成.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ) ) ){
		_ASSERT_EXPR( false, L"頂点ﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}

	D3D11_BUFFER_DESC id;
	id.Usage				= D3D11_USAGE_DEFAULT;	   // 使用方法(デフォルト).
	id.ByteWidth			= sizeof(WORD)*index_num*2;  // 頂点のサイズ.
	id.BindFlags			= D3D11_BIND_INDEX_BUFFER; // 頂点バッファとして扱う.
	id.CPUAccessFlags		= 0;					   // CPUからはアクセスしない.
	id.MiscFlags			= 0;					   // その他のフラグ(未使用).
	id.StructureByteStride	= 0;					   // 構造体のサイズ(未使用).

	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem			= indices;
	IndexData.SysMemPitch		= 0;
	IndexData.SysMemSlicePitch	= 0;

	if( FAILED( m_pDevice11->CreateBuffer(
		&id, &IndexData, &m_pIndexBuffer ))){
		_ASSERT_EXPR( false, L"インデックスﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}

	return S_OK;
}