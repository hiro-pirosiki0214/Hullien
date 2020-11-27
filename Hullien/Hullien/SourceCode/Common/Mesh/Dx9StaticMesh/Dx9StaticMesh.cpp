#include "Dx9StaticMesh.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Light\LightManager\LightManager.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\D3DX\D3DX11.h"
#include "..\..\Shader\ShadowMap\ShadowMap.h"
#include "..\..\Shader\TranslucentShader\TranslucentShader.h"
#include "..\..\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\Fog\Fog.h"
#include <crtdbg.h>	//_ASSERTﾏｸﾛで必要.

// シェーダー名.
const char SHADER_NAME[] = "Data\\Shader\\Mesh.hlsl";

CDX9StaticMesh::CDX9StaticMesh()
	: m_hWnd				( nullptr )
	, m_pDevice9			( nullptr )
	, m_pVertexShader		( nullptr )
	, m_pVertexLayout		( nullptr )
	, m_pPixelShader		( nullptr )
	, m_pCBufferPerMesh		( nullptr )
	, m_pCBufferPerMaterial	( nullptr )
	, m_pCBufferPerFrame	( nullptr )
	, m_pVertexBuffer		( nullptr )
	, m_ppIndexBuffer		( nullptr )
	, m_pSampleLinear		( nullptr )
	, m_pToonSampleLinear	( nullptr )
	, m_pShadowMapSampler	( nullptr )
							  
	, m_pMesh				( nullptr )
	, m_pMeshForRay			( nullptr )
	, m_NumMaterials		( 0 )
	, m_pToonTexture		( nullptr )
	, m_pFogTexture			( nullptr )
	, m_pMaterials			( nullptr )
	, m_NumAttr				( 0 )
	, m_AttrID				()
	, m_EnableTexture		( false )
	, m_IsShadow			( false )
{
}

CDX9StaticMesh::~CDX9StaticMesh()
{
	// 解放処理.
	Release();
}


HRESULT CDX9StaticMesh::Init(
	HWND hWnd, 
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, 
	LPDIRECT3DDEVICE9 pDevice9,
	const char* fileName )
{
	m_hWnd = hWnd;
	m_pDevice9 = pDevice9;
	if( FAILED( InitPram( pDevice11, pContext11 )) )	return E_FAIL;
	if( FAILED( LoadXMesh( fileName )) )				return E_FAIL; 
	if( FAILED( InitShader()) )							return E_FAIL;
	if( FAILED( CreateCBuffer( &m_pCBufferPerMesh,		sizeof(CBUFFER_PER_MESH) ) ))		return E_FAIL;
	if( FAILED( CreateCBuffer( &m_pCBufferPerMaterial,	sizeof(CBUFFER_PER_MATERIAL) ) ))	return E_FAIL;
	if( FAILED( CreateCBuffer( &m_pCBufferPerFrame,		sizeof(CBUFFER_PER_FRAME) ) ))		return E_FAIL;

	return S_OK;
}

// メッシュ読み込み.
HRESULT CDX9StaticMesh::LoadXMesh(const char* fileName)
{
	// マテリアルバッファ.
	LPD3DXBUFFER pD3DXMtrlBuffer = nullptr;

	// Xファイルの読み込み.
	if( FAILED( D3DXLoadMeshFromX(
		fileName,			// ファイル名.
		D3DXMESH_SYSTEMMEM	// システムメモリに読み込み.
		| D3DXMESH_32BIT,	// 32bit.
		m_pDevice9,
		nullptr,
		&pD3DXMtrlBuffer,	// (out)マテリアル情報.
		nullptr,
		&m_NumMaterials,	// (out)マテリアル数.
		&m_pMesh ))){		// (out)メッシュオブジェクト.
		_ASSERT_EXPR(false, L"Xﾌｧｲﾙ読込失敗");
		return E_FAIL;
	}

	//Xﾌｧｲﾙのﾛｰﾄﾞ(ﾚｲとの判定用に別設定で読み込む).
	if( FAILED( D3DXLoadMeshFromX(
		fileName,			// ファイル名.
		D3DXMESH_SYSTEMMEM,	// システムメモリに読み込み.
		m_pDevice9,
		nullptr,
		&pD3DXMtrlBuffer,	// (out)マテリアル情報.
		nullptr,
		&m_NumMaterials,	// (out)マテリアル数.
		&m_pMeshForRay ))){	// (out)メッシュオブジェクト.
		_ASSERT_EXPR(false, L"Xﾌｧｲﾙ読込失敗");
		return E_FAIL;
	}

	D3D11_BUFFER_DESC		bd;			// Dx11バッファ構造体.
	D3D11_SUBRESOURCE_DATA	InitData;	// 初期化データ.
	// 読み込んだ情報から必要な情報を抜き出す.
	D3DXMATERIAL* d3dxMaterials
		= static_cast<D3DXMATERIAL*>(pD3DXMtrlBuffer->GetBufferPointer());
	// マテリアル数分の領域を確保.
	m_pMaterials = new MY_MATERIAL[m_NumMaterials]();
	m_ppIndexBuffer = new ID3D11Buffer*[m_NumMaterials]();
	// マテリアル数分繰り返し.
	for( DWORD No = 0; No < m_NumMaterials; No++ ){
		// インデックスバッファの初期化.
		m_ppIndexBuffer[No] = nullptr;

		// マテリアル情報のコピー.
		// アンビエント.
		m_pMaterials[No].Ambient.x = d3dxMaterials[No].MatD3D.Ambient.r;
		m_pMaterials[No].Ambient.y = d3dxMaterials[No].MatD3D.Ambient.g;
		m_pMaterials[No].Ambient.z = d3dxMaterials[No].MatD3D.Ambient.b;
		m_pMaterials[No].Ambient.w = d3dxMaterials[No].MatD3D.Ambient.a;
		// ディフューズ.
		m_pMaterials[No].Diffuse.x = d3dxMaterials[No].MatD3D.Diffuse.r;
		m_pMaterials[No].Diffuse.y = d3dxMaterials[No].MatD3D.Diffuse.g;
		m_pMaterials[No].Diffuse.z = d3dxMaterials[No].MatD3D.Diffuse.b;
		m_pMaterials[No].Diffuse.w = d3dxMaterials[No].MatD3D.Diffuse.a;
		// スペキュラ.
		m_pMaterials[No].Specular.x = d3dxMaterials[No].MatD3D.Specular.r;
		m_pMaterials[No].Specular.y = d3dxMaterials[No].MatD3D.Specular.g;
		m_pMaterials[No].Specular.z = d3dxMaterials[No].MatD3D.Specular.b;
		m_pMaterials[No].Specular.w = d3dxMaterials[No].MatD3D.Specular.a;

		// (その面に)テクスチャが貼られているか？.
		if( d3dxMaterials[No].pTextureFilename != nullptr &&
			lstrlen(d3dxMaterials[No].pTextureFilename) > 0 ){
			// テクスチャありのフラグを立てる.
			m_EnableTexture = true;

			char path[128] = "";
			int path_count = lstrlen(fileName);
			for( int k = path_count; k >= 0; k-- ){
				if( fileName[k] == '\\' ){
					for( int j = 0; j <= k; j++ ){
						path[j] = fileName[j];
					}
					path[k + 1] = '\0';
					break;
				}
			}
			// パスとテクスチャ名を連結.
			strcat_s( path, sizeof(path), d3dxMaterials[No].pTextureFilename );

			//　テクスチャ名をコピー.
			strcpy_s( m_pMaterials[No].szTextureName, sizeof(m_pMaterials[No].szTextureName), path );

			// テクスチャ作成.
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice11,
				m_pMaterials[No].szTextureName,	// テクスチャファイル名.
				nullptr,
				nullptr,
				&m_pMaterials[No].pTexture,		// (out)テクスチャオブジェクト.
				nullptr ))){
				_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
				return E_FAIL;
			}
		}
	}
	// トゥーンテクスチャ作成.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,
		"Data\\Mesh\\toon.png",	// テクスチャファイル名.
		nullptr,
		nullptr,
		&m_pToonTexture,		// (out)テクスチャオブジェクト.
		nullptr ))){
		_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
		return E_FAIL;
	}
	// フォグテクスチャ作成.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11, 
		"Data\\Mesh\\Fog.png",	// テクスチャファイル名.
		nullptr,
		nullptr,
		&m_pFogTexture,			// (out)テクスチャオブジェクト.
		nullptr ))){
		_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
		return E_FAIL;
	}
	//------------------------------------------------
	//	インデックスバッファ作成.
	//------------------------------------------------
	// メッシュの属性情報を得る.
	//	属性情報でインデックスバッファから細かいマテリアルごとのインデックスバッファを分離できる.
	D3DXATTRIBUTERANGE* pAttrTable = nullptr;

	// メッシュの面および頂点の順番変更を制御し、パフォーマンスを最適化する.
	//	D3DXMESHOPT_COMPACT : 面の順番を変更し、使用されていない頂点と面を削除する.
	//	D3DXMESHOPT_ATTRSORT : パフォーマンスを上げる為、面の順番を変更して最適化を行う.
	m_pMesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
		nullptr, nullptr, nullptr, nullptr );
	// 属性テーブルの取得.
	m_pMesh->GetAttributeTable( nullptr, &m_NumAttr );
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if( FAILED( m_pMesh->GetAttributeTable( pAttrTable, &m_NumAttr )) ){
		_ASSERT_EXPR(false, L"属性ﾃｰﾌﾞﾙ取得失敗");
		return E_FAIL;
	}

	// 同じくLockしないと取り出せない.
	int* pIndex = nullptr;
	m_pMesh->LockIndexBuffer( D3DLOCK_READONLY, (void**)&pIndex );
	// 属性ごとのインデックスバッファを作成.
	for( DWORD No = 0; No < m_NumAttr; No++ ){
		m_AttrID[No] = pAttrTable[No].AttribId;
		// Dx9のインデックスバッファからの情報で、
		//	Dx11のインデックスバッファを作成.
		bd.Usage			= D3D11_USAGE_DEFAULT;
		bd.ByteWidth		= sizeof(int)*pAttrTable[No].FaceCount*3;//面数×3で頂点数.
		bd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags	= 0;
		bd.MiscFlags		= 0;
		// 大きいインデックスバッファ内のオフセット(×3する).
		InitData.pSysMem = &pIndex[pAttrTable[No].FaceStart*3];

		// インデックスバッファの作成.
		if( FAILED( m_pDevice11->CreateBuffer(
			&bd, 
			&InitData,
			&m_ppIndexBuffer[No] ))){
			_ASSERT_EXPR(false, L"ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ作成失敗");
			return E_FAIL;
		}
		// 面の数をコピー.
		m_pMaterials[m_AttrID[No]].dwNumFace = pAttrTable[No].FaceCount;
	}
	// 属性テーブルの削除.
	SAFE_DELETE( pAttrTable );
	// 使用済みのインデックスバッファの解放.
	m_pMesh->UnlockIndexBuffer();
	// 不要になったマテリアルバッファを解放.
	SAFE_RELEASE( pD3DXMtrlBuffer );

	//------------------------------------------------
	//	頂点バッファの作成.
	//------------------------------------------------
	// Dx9の場合、mapではなくLockで頂点バッファからデータを取り出す.
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	m_pMesh->GetVertexBuffer( &pVB );
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = nullptr;
	VERTEX* pVertex = nullptr;
	if( SUCCEEDED( pVB->Lock(0, 0, (VOID**)&pVertices, 0 ))){
		pVertex				= (VERTEX*)pVertices;
		// Dx9の頂点バッファからの情報で、Dx11頂点バッファを作成.
		bd.Usage			= D3D11_USAGE_DEFAULT;
		// 頂点を格納するのに必要なバイト数.
		bd.ByteWidth		= m_pMesh->GetNumBytesPerVertex()*m_pMesh->GetNumVertices();
		bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags	= 0;
		bd.MiscFlags		= 0;
		InitData.pSysMem = pVertex;
		if( FAILED( m_pDevice11->CreateBuffer(
			&bd,
			&InitData,
			&m_pVertexBuffer ))){
			_ASSERT_EXPR(false, L"頂点ﾊﾞｯﾌｧ作成失敗");
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE( pVB );	// 頂点バッファ解放.


	// テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter		= D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// リニアフィルタ(線形補間).
															// POINT:高速だが粗い.
	// ラッピングモード.
	samDesc.AddressU	= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV	= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW	= D3D11_TEXTURE_ADDRESS_WRAP;
	// WRAP		: 繰り返し.
	// MIRROR	: 反転繰り返し.
	// CLAMP	: 端の模様を引き伸ばす.
	// BORDE	: 別途境界色を決める.
	// サンプラ作成.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc,
		&m_pSampleLinear ))){	// (out)サンプラ.
		_ASSERT_EXPR(false, L"ｻﾝﾌﾟﾗ作成失敗");
		return E_FAIL;
	}

	// トゥーンテクスチャ用.
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc,
		&m_pToonSampleLinear))){
		_ASSERT_EXPR(false, L"ｻﾝﾌﾟﾗ作成失敗");
		return E_FAIL;
	}

	// シャドウマップ用.
	samDesc.Filter		= D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU	= D3D11_TEXTURE_ADDRESS_BORDER;
	samDesc.AddressV	= D3D11_TEXTURE_ADDRESS_BORDER;
	samDesc.AddressW	= D3D11_TEXTURE_ADDRESS_BORDER;
	samDesc.BorderColor[0] = 1.0f;
	samDesc.BorderColor[1] = 1.0f;
	samDesc.BorderColor[2] = 1.0f;
	samDesc.BorderColor[3] = 1.0f;
	samDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samDesc.MaxAnisotropy = 1;
	samDesc.MipLODBias = 0.0f;
	samDesc.MinLOD = 0.0f;
	samDesc.MinLOD = D3D11_FLOAT32_MAX;
	// サンプラ作成.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pShadowMapSampler ))){
		_ASSERT_EXPR( false, "サンプラー作成失敗" );
		MessageBox( nullptr, "サンプラー作成失敗", "Warning", MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

//解放関数.
void CDX9StaticMesh::Release()
{
	//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ解放.
	if( m_ppIndexBuffer != nullptr ){
		for( int No = m_NumMaterials-1; No >= 0; No-- ){
			SAFE_RELEASE( m_ppIndexBuffer[No] );
		}
		delete[] m_ppIndexBuffer;
		m_ppIndexBuffer = nullptr;
	}
	SAFE_DELETE_ARRAY(m_pMaterials);
	SAFE_RELEASE( m_pMesh );
	SAFE_RELEASE( m_pShadowMapSampler );
	SAFE_RELEASE( m_pToonSampleLinear );
	SAFE_RELEASE( m_pFogTexture );
	SAFE_RELEASE( m_pToonTexture );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pCBufferPerMaterial );
	SAFE_RELEASE( m_pCBufferPerMesh );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );

	m_pDevice9 = nullptr;
	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_hWnd = nullptr;
}

//===========================================================
//	HLSLﾌｧｲﾙを読み込みｼｪｰﾀﾞを作成する.
//	HLSL: High Level Shading Language の略.
//===========================================================
HRESULT CDX9StaticMesh::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG.

	// 頂点シェーダーのブロブ作成.
	if( m_EnableTexture == true ){
		if( FAILED( D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr,
			nullptr,
			"VS_Main",
			"vs_5_0",
			uCompileFlag,
			0,
			nullptr,
			&pCompiledShader,
			&pErrors,
			nullptr ))){
			_ASSERT_EXPR(false, L"hlsl読み込み失敗");
			return E_FAIL;
		}
	} else {
		if( FAILED( D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr,
			nullptr,
			"VS_NoTex",
			"vs_5_0",
			uCompileFlag,
			0,
			nullptr,
			&pCompiledShader,
			&pErrors, nullptr ))){
			_ASSERT_EXPR(false, L"hlsl読み込み失敗");
			return E_FAIL;
		}
	}
	
	// 上記で作成したブロブから「頂点シェーダー」を作成.
	if( FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader))){	// (out)頂点シェーダー.
		_ASSERT_EXPR(false, L"ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ作成失敗");
		return E_FAIL;
	}

	// 頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	// 頂点インプットの配列要素数を算出.
	UINT numElements = 0;
	if( m_EnableTexture == true ){
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);	// 要素数算出.
		memcpy_s( layout, sizeof(layout), tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements );
	} else {
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);	//要素数算出.
		memcpy_s( layout, sizeof(layout), tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements );
	}

	// 頂点インプットレイアウトを作成.
	if( FAILED( m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout ))){	//(out)頂点インプットレイアウト.
		_ASSERT_EXPR(false, L"頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗");
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// HLSLからピクセルシェーダーのブロブを作成.
	if( m_EnableTexture == true ){
		if( FAILED( D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr,
			nullptr,
			"PS_Main",
			"ps_5_0",
			uCompileFlag,
			0,
			nullptr,
			&pCompiledShader,
			&pErrors,
			nullptr ))){
			_ASSERT_EXPR(false, L"hlsl読み込み失敗");
			return E_FAIL;
		}
	} else {
		if( FAILED( D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr,
			nullptr,
			"PS_NoTex",
			"ps_5_0",
			uCompileFlag,
			0,
			nullptr,
			&pCompiledShader,
			&pErrors,
			nullptr ))){
			_ASSERT_EXPR(false, L"hlsl読み込み失敗");
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);

	// 上記で作成したブロブから「ピクセルシェーダー」を作成.
	if( FAILED( m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader ))){	// (out)ピクセルシェーダー.
		_ASSERT_EXPR(false, L"ﾋﾟｸｾﾙｼｪｰﾀﾞ作成失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	return S_OK;
}

//コンスタントバッファ作成関数.
HRESULT CDX9StaticMesh::CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size )
{
	D3D11_BUFFER_DESC cb;

	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = size;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if( FAILED( m_pDevice11->CreateBuffer(
		&cb, 
		nullptr,
		pConstantBuffer ))){
		return E_FAIL;
	}
	return S_OK;
}

// レンダリング用.
void CDX9StaticMesh::Render( const bool& isTrans )
{
	// ワールド行列、スケール行列、回転行列、平行移動行列.
	D3DXMATRIX mWorld, mScale, mRot, mTran;
	D3DXMATRIX mYaw, mPitch, mRoll;

	// 拡大縮小行列作成.
	D3DXMatrixScaling(
		&mScale,
		m_vScale.x, m_vScale.y, m_vScale.z );

	// Y軸回転行列作成.
	D3DXMatrixRotationY( &mYaw, m_vRot.y );
	// X軸回転行列作成.
	D3DXMatrixRotationX( &mPitch, m_vRot.x );
	// Z軸回転行列作成.
	D3DXMatrixRotationZ( &mRoll, m_vRot.z );
	// 回転行列を作成.
	mRot = mYaw * mPitch * mRoll;

	// 平行移動行列作成.
	D3DXMatrixTranslation(
		&mTran,
		m_vPos.x, m_vPos.y, m_vPos.z );

	// ワールド行列作成.
	// 拡縮×回転×移動 ※順番がとても大切！！.
	mWorld = mScale * mRot * mTran;

	// 影を描画したら終了.
	if( ShadowRender( mWorld ) == true ) return;
	if( isTrans == false ){
		if( TranslucentRender( mWorld ) == true ) return;
	}

	// 使用するシェーダーのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダー.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// ピクセルシェーダー.
	// サンプラのセット.
	m_pContext11->PSSetSamplers( 1, 1, &m_pToonSampleLinear );
	m_pContext11->PSSetSamplers( 2, 1, &m_pShadowMapSampler );

	// コンスタントバッファ.
	CBUFFER_PER_FRAME cb;

	// カスケードの数だけループ.
	for( int i = 0; i < CSceneTexRenderer::MAX_CASCADE; i++ ){
		const float f = isTrans == false ? 1.0f : 0.0f;
		// ライトの行列を渡す.
		cb.mLightWVP[i] = (mWorld * CLightManager::GetShadowVP()[i]) * f;
		D3DXMatrixTranspose( &cb.mLightWVP[i], &cb.mLightWVP[i] );
		// 影テクスチャを渡す.
		m_pContext11->PSSetShaderResources( i+1, 1, &CSceneTexRenderer::GetShadowBuffer()[i] );
	}
	// シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	// バッファ内のデータの書き換え開始時にMap.
	if( SUCCEEDED( m_pContext11->Map(
		m_pCBufferPerFrame, 0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData))){

		// 色のセット.
		cb.vColor = m_vColor;

		// カメラ位置.
		cb.vCamPos = { CCameraManager::GetPosition(), 0.0f };

		// ライト位置.
		cb.vCamPos = { CLightManager::GetPosition(), 0.0f };
		// ライト方向.
		cb.vLightDir = { CLightManager::GetDirection(), 0.0f };
		D3DXVec4Normalize(&cb.vLightDir, &cb.vLightDir);
		// ライト回転行列.
		cb.mLightRot = CLightManager::GetRorarionMatrix();

		// カスケードの間隔幅を渡す.
		cb.SpritPos.x = CLightManager::GetSpritPos()[0];
		cb.SpritPos.y = CLightManager::GetSpritPos()[1];
		cb.SpritPos.z = CLightManager::GetSpritPos()[2];
		cb.SpritPos.w = CLightManager::GetSpritPos()[3];

		// ライト強度(明るさ).
		cb.fIntensity.x = CLightManager::GetIntensity();

		// 自分自身に影を落とすか.
		cb.IsShadow.x = m_IsShadow;

		// フォグのテクスチャ座標.
		cb.Fog = CFog::GetFog();

		memcpy_s(
			pData.pData,	// コピー先のバファ.
			pData.RowPitch,	// コピー先のバファサイズ.
			(void*)(&cb),	// コピー元のバファ.
			sizeof(cb));	// コピー元のバファサイズ.

		// バッファ内のデータの書き換え終了時にUnmap.
		m_pContext11->Unmap( m_pCBufferPerFrame, 0 );
	}

	// コンスタントバッファのセット.
	m_pContext11->VSSetConstantBuffers( 2, 1, &m_pCBufferPerFrame );
	m_pContext11->PSSetConstantBuffers( 2, 1, &m_pCBufferPerFrame );

	// トゥーンマップテクスチャを渡す.
	m_pContext11->PSSetShaderResources( 5, 1, &m_pToonTexture );
	// フォグテクスチャを渡す.
	m_pContext11->PSSetShaderResources( 6, 1, &m_pFogTexture );
	// メッシュのレンダリング.
	RenderMesh( mWorld, CCameraManager::GetViewMatrix(), CCameraManager::GetProjMatrix(), isTrans );
}

// レンダリング関数(ｸﾗｽ内でのみ使用する).
void CDX9StaticMesh::RenderMesh(
	D3DXMATRIX& mWorld, 
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj,
	const bool& isTrans )
{
	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	// バッファ内のデータの書き換え開始時にMap.
	if( SUCCEEDED( m_pContext11->Map(
		m_pCBufferPerMesh,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData ))){
		CBUFFER_PER_MESH cb;	// コンスタントバッファ.

		// ワールド行列を渡す.
		const float f = isTrans == false ? 1.0f : 0.0f;
		cb.mW = mWorld * f;
		D3DXMatrixTranspose( &cb.mW, &cb.mW );

		// ワールド, ビュー, プロジェクション行列を渡す.
		D3DXMATRIX mWVP = mWorld * mView * mProj;
		D3DXMatrixTranspose( &mWVP, &mWVP );	// 行列を転置する.
		// ※行列の計算方法がDirectXとGPUで異なるため転置が必要.
		cb.mWVP = mWVP;

		memcpy_s(
			pData.pData,	// コピー先のバッファ.
			pData.RowPitch,	// コピー先のバッファサイズ.
			(void*)(&cb),	// コピー元のバッファ.
			sizeof(cb));	// コピー元のバッファサイズ.

		// バッファ内のデータの書き換え終了時にUnmap.
		m_pContext11->Unmap(m_pCBufferPerMesh, 0);
	}

	// コンスタントバッファの設定.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pCBufferPerMesh );
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pCBufferPerMesh );

	// 頂点インプットレイアウトをセット.
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	// プリミティブ・トポロジーをセット.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// 頂点バッファをセット.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	// 属性の数だけ、それぞれの属性のインデックスバッファを描画.
	for( DWORD No = 0; No < m_NumAttr; No++ ){
		// 使用されていないマテリアル対策.
		if( m_pMaterials[m_AttrID[No]].dwNumFace == 0 ) continue;

		// インデックスバッファをセット.
		m_pContext11->IASetIndexBuffer( m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0 );
		// マテリアルの各要素をシェーダーに渡す.
		D3D11_MAPPED_SUBRESOURCE pDataMat;
		if( SUCCEEDED( m_pContext11->Map(
			m_pCBufferPerMaterial,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&pDataMat ))){
			CBUFFER_PER_MATERIAL cb;
			// アンビエント, ディヒューズ, スペキュラをシェーダーに渡す.
			cb.vAmbient		= m_pMaterials[m_AttrID[No]].Ambient;
			cb.vDiffuse		= m_pMaterials[m_AttrID[No]].Diffuse;
			cb.vSpecular	= m_pMaterials[m_AttrID[No]].Specular;

			memcpy_s( pDataMat.pData, pDataMat.RowPitch, (void*)&cb, sizeof(cb) );
			m_pContext11->Unmap( m_pCBufferPerMaterial, 0 );
		}

		// コンスタントバッファのセット.
		m_pContext11->VSSetConstantBuffers(1, 1, &m_pCBufferPerMaterial);
		m_pContext11->PSSetConstantBuffers(1, 1, &m_pCBufferPerMaterial);

		// テクスチャをシェーダーに渡す.
		if( m_pMaterials[m_AttrID[No]].pTexture != nullptr ){
			// テクスチャがあるとき.
			m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
			m_pContext11->PSSetShaderResources( 0, 1, &m_pMaterials[m_AttrID[No]].pTexture );
		} else {
			// テクスチャがないとき.
			ID3D11ShaderResourceView* pNothing[1] = { 0 };
			m_pContext11->PSSetShaderResources( 0, 1, pNothing );
		}

		// ポリゴンをレンダイｒング.
		m_pContext11->DrawIndexed( m_pMaterials[m_AttrID[No]].dwNumFace*3, 0, 0);
	}
}

// 影の描画.
bool CDX9StaticMesh::ShadowRender( const D3DXMATRIX& mWorld )
{
	if( CSceneTexRenderer::GetRenderPass() != CSceneTexRenderer::ERenderPass::Shadow ) return false;

	// カスケード分ループ.
	for( int i = 0; i < CSceneTexRenderer::MAX_CASCADE; i++ ){
		CSceneTexRenderer::SetShadowBuffer( i );
		CShadowMap::SetConstantBufferData( mWorld*CLightManager::GetShadowVP()[i] );
		// 頂点バッファをセット.
		UINT stride = m_pMesh->GetNumBytesPerVertex();
		UINT offset = 0;
		m_pContext11->IASetVertexBuffers(
			0, 1, &m_pVertexBuffer, &stride, &offset);
		// プリミティブ・トポロジーをセット.
		m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// 属性の数だけ、それぞれの属性のインデックスバッファを描画.
		for( DWORD No = 0; No < m_NumAttr; No++ ){
			// インデックスバッファをセット.
			m_pContext11->IASetIndexBuffer( m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0 );
			// ポリゴンをレンダリング.
			m_pContext11->DrawIndexed( m_pMaterials[m_AttrID[No]].dwNumFace*3, 0, 0 );
		}
	}

	return true;
}

// 半透明の描画,
bool CDX9StaticMesh::TranslucentRender( const D3DXMATRIX& mWorld )
{
	if( CSceneTexRenderer::GetRenderPass() != CSceneTexRenderer::ERenderPass::Trans ) return false;

	CTranslucentShader::SetConstantBufferData( mWorld*CCameraManager::GetViewMatrix()*CCameraManager::GetProjMatrix() );
	// 頂点バッファをセット.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	// プリミティブ・トポロジーをセット.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	// 属性の数だけ、それぞれの属性のインデックスバッファを描画.
	for( DWORD No = 0; No < m_NumAttr; No++ ){
		// インデックスバッファをセット.
		m_pContext11->IASetIndexBuffer( m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0 );
		// ポリゴンをレンダリング.
		m_pContext11->DrawIndexed( m_pMaterials[m_AttrID[No]].dwNumFace*3, 0, 0 );
	}
	return true;
}