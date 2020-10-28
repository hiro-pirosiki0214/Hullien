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

//ｼｪｰﾀﾞﾌｧｲﾙ名(ﾃﾞｨﾚｸﾄﾘも含む).
const char SHADER_NAME[] = "Data\\Shader\\Mesh.hlsl";

//ｺﾝｽﾄﾗｸﾀ.
CDX9StaticMesh::CDX9StaticMesh()
	: m_hWnd(nullptr)
	, m_pDevice9(nullptr)

	, m_pVertexShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pPixelShader(nullptr)
	, m_pCBufferPerMesh(nullptr)
	, m_pCBufferPerMaterial(nullptr)
	, m_pCBufferPerFrame(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_ppIndexBuffer(nullptr)
	, m_pSampleLinear(nullptr)
	, m_pShadowMapSampler(nullptr)

	, m_pMesh(nullptr)
	, m_NumMaterials(0)
	, m_pMaterials(nullptr)
	, m_pToonTexture(nullptr)
	, m_pFogTexture(nullptr)
	, m_NumAttr(0)
	, m_AttrID()
	, m_EnableTexture(false)
	, m_pMeshForRay(nullptr)
	, m_IsShadow(false)
{
}

//ﾃﾞｽﾄﾗｸﾀ.
CDX9StaticMesh::~CDX9StaticMesh()
{
	//解放処理.
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
	if( FAILED(InitPram( pDevice11, pContext11 ))) return E_FAIL;
	if( FAILED(LoadXMesh(fileName))) return E_FAIL; 
	if( FAILED(InitShader())) return E_FAIL;

	return S_OK;
}

//ﾒｯｼｭ読み込み.
HRESULT CDX9StaticMesh::LoadXMesh(const char* fileName)
{
	//ﾏﾃﾘｱﾙﾊﾞｯﾌｧ.
	LPD3DXBUFFER pD3DXMtrlBuffer = nullptr;

	//Xﾌｧｲﾙのﾛｰﾄﾞ.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,	//ﾌｧｲﾙ名.
		D3DXMESH_SYSTEMMEM	//ｼｽﾃﾑﾒﾓﾘに読み込み.
		| D3DXMESH_32BIT,	//32bit.
		m_pDevice9, nullptr,
		&pD3DXMtrlBuffer,	//(out)ﾏﾃﾘｱﾙ情報.
		nullptr,
		&m_NumMaterials,	//(out)ﾏﾃﾘｱﾙ数.
		&m_pMesh)))			//(out)ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.
	{
		_ASSERT_EXPR(false, L"Xﾌｧｲﾙ読込失敗");
		return E_FAIL;
	}

	//Xﾌｧｲﾙのﾛｰﾄﾞ(ﾚｲとの判定用に別設定で読み込む).
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,	//ﾌｧｲﾙ名.
		D3DXMESH_SYSTEMMEM,	//ｼｽﾃﾑﾒﾓﾘに読み込み.
		m_pDevice9, nullptr,
		&pD3DXMtrlBuffer,	//(out)ﾏﾃﾘｱﾙ情報.
		nullptr,
		&m_NumMaterials,	//(out)ﾏﾃﾘｱﾙ数.
		&m_pMeshForRay)))	//(out)ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.
	{
		_ASSERT_EXPR(false, L"Xﾌｧｲﾙ読込失敗");
		return E_FAIL;
	}


	D3D11_BUFFER_DESC		bd;	//Dx11ﾊﾞｯﾌｧ構造体.
	D3D11_SUBRESOURCE_DATA	InitData;//初期化ﾃﾞｰﾀ.
									 //読み込んだ情報から必要な情報を抜き出す.
	D3DXMATERIAL* d3dxMaterials
		= static_cast<D3DXMATERIAL*>(pD3DXMtrlBuffer->GetBufferPointer());
	//ﾏﾃﾘｱﾙ数分の領域を確保.
	m_pMaterials = new MY_MATERIAL[m_NumMaterials]();
	m_ppIndexBuffer = new ID3D11Buffer*[m_NumMaterials]();
	//ﾏﾃﾘｱﾙ数分繰り返し.
	for (DWORD No = 0; No < m_NumMaterials; No++) 
	{
		//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧの初期化.
		m_ppIndexBuffer[No] = nullptr;

		//ﾏﾃﾘｱﾙ情報のｺﾋﾟｰ.
		//ｱﾝﾋﾞｴﾝﾄ.
		m_pMaterials[No].Ambient.x = d3dxMaterials[No].MatD3D.Ambient.r;
		m_pMaterials[No].Ambient.y = d3dxMaterials[No].MatD3D.Ambient.g;
		m_pMaterials[No].Ambient.z = d3dxMaterials[No].MatD3D.Ambient.b;
		m_pMaterials[No].Ambient.w = d3dxMaterials[No].MatD3D.Ambient.a;
		//ﾃﾞｨﾌｭｰｽﾞ.
		m_pMaterials[No].Diffuse.x = d3dxMaterials[No].MatD3D.Diffuse.r;
		m_pMaterials[No].Diffuse.y = d3dxMaterials[No].MatD3D.Diffuse.g;
		m_pMaterials[No].Diffuse.z = d3dxMaterials[No].MatD3D.Diffuse.b;
		m_pMaterials[No].Diffuse.w = d3dxMaterials[No].MatD3D.Diffuse.a;
		//ｽﾍﾟｷｭﾗ.
		m_pMaterials[No].Specular.x = d3dxMaterials[No].MatD3D.Specular.r;
		m_pMaterials[No].Specular.y = d3dxMaterials[No].MatD3D.Specular.g;
		m_pMaterials[No].Specular.z = d3dxMaterials[No].MatD3D.Specular.b;
		m_pMaterials[No].Specular.w = d3dxMaterials[No].MatD3D.Specular.a;

		//(その面に)ﾃｸｽﾁｬが貼られているか？.
		if( d3dxMaterials[No].pTextureFilename != nullptr
			&& lstrlen(d3dxMaterials[No].pTextureFilename) > 0)
		{
			//ﾃｸｽﾁｬありのﾌﾗｸﾞを立てる.
			m_EnableTexture = true;

			char path[128] = "";
			int path_count = lstrlen(fileName);
			for (int k = path_count; k >= 0; k--) {
				if (fileName[k] == '\\') {
					for (int j = 0; j <= k; j++) {
						path[j] = fileName[j];
					}
					path[k + 1] = '\0';
					break;
				}
			}
			//ﾊﾟｽとﾃｸｽﾁｬﾌｧｲﾙ名を連結.
			strcat_s(path, sizeof(path), d3dxMaterials[No].pTextureFilename);

			//ﾃｸｽﾁｬﾌｧｲﾙ名をｺﾋﾟｰ.
			strcpy_s(m_pMaterials[No].szTextureName,
				sizeof(m_pMaterials[No].szTextureName),	path);

			// ﾃｸｽﾁｬ作成.
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(
				m_pDevice11, m_pMaterials[No].szTextureName,//ﾃｸｽﾁｬﾌｧｲﾙ名.
				nullptr, nullptr,
				&m_pMaterials[No].pTexture,//(out)ﾃｸｽﾁｬｵﾌﾞｼﾞｪｸﾄ.
				nullptr)))
			{
				_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
				return E_FAIL;
			}
		}
	}
	// ﾃｸｽﾁｬ作成.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11, "Data\\Mesh\\toon.png",//ﾃｸｽﾁｬﾌｧｲﾙ名.
		nullptr, nullptr,
		&m_pToonTexture,//(out)ﾃｸｽﾁｬｵﾌﾞｼﾞｪｸﾄ.
		nullptr)))
	{
		_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
		return E_FAIL;
	}
	// ﾃｸｽﾁｬ作成.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11, "Data\\Mesh\\Fog.png",//ﾃｸｽﾁｬﾌｧｲﾙ名.
		nullptr, nullptr,
		&m_pFogTexture,//(out)ﾃｸｽﾁｬｵﾌﾞｼﾞｪｸﾄ.
		nullptr)))
	{
		_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
		return E_FAIL;
	}
	//------------------------------------------------
	//	ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ作成.
	//------------------------------------------------
	//ﾒｯｼｭの属性情報を得る.
	//属性情報でｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧから細かいﾏﾃﾘｱﾙごとのｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを分離できる.
	D3DXATTRIBUTERANGE* pAttrTable = nullptr;

	//ﾒｯｼｭの面および頂点の順番変更を制御し、ﾊﾟﾌｫｰﾏﾝｽを最適化する.
	//D3DXMESHOPT_COMPACT : 面の順番を変更し、使用されていない頂点と面を削除する.
	//D3DXMESHOPT_ATTRSORT : ﾊﾟﾌｫｰﾏﾝｽを上げる為、面の順番を変更して最適化を行う.
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
		nullptr, nullptr, nullptr, nullptr);
	//属性ﾃｰﾌﾞﾙの取得.
	m_pMesh->GetAttributeTable(nullptr, &m_NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if (FAILED(m_pMesh->GetAttributeTable(pAttrTable, &m_NumAttr)))
	{
		_ASSERT_EXPR(false, L"属性ﾃｰﾌﾞﾙ取得失敗");
		return E_FAIL;
	}

	//同じくLockしないと取り出せない.
	int* pIndex = nullptr;
	m_pMesh->LockIndexBuffer(
		D3DLOCK_READONLY, (void**)&pIndex);
	//属性ごとのｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを作成.
	for (DWORD No = 0; No < m_NumAttr; No++)
	{
		m_AttrID[No] = pAttrTable[No].AttribId;
		//Dx9のｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧからの情報で、Dx11のｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを作成.
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth
			= sizeof(int)*pAttrTable[No].FaceCount * 3;//面数×3で頂点数.
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		//大きいｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ内のｵﾌｾｯﾄ(×3する).
		InitData.pSysMem = &pIndex[pAttrTable[No].FaceStart*3];

		if (FAILED(m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_ppIndexBuffer[No])))
		{
			_ASSERT_EXPR(false, L"ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ作成失敗");
			return E_FAIL;
		}
		//面の数をｺﾋﾟｰ.
		m_pMaterials[m_AttrID[No]].dwNumFace = pAttrTable[No].FaceCount;
	}
	//属性ﾃｰﾌﾞﾙの削除.
	delete[] pAttrTable;
	//使用済みのｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧの解放.
	m_pMesh->UnlockIndexBuffer();
	//不要になったﾏﾃﾘｱﾙﾊﾞｯﾌｧを解放.
	SAFE_RELEASE(pD3DXMtrlBuffer);

	//------------------------------------------------
	//	頂点ﾊﾞｯﾌｧの作成.
	//------------------------------------------------
	//Dx9の場合、mapではなくLockで頂点ﾊﾞｯﾌｧからﾃﾞｰﾀを取り出す.
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = nullptr;
	VERTEX* pVertex = nullptr;
	if (SUCCEEDED(
		pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertex = (VERTEX*)pVertices;
		//Dx9の頂点ﾊﾞｯﾌｧからの情報で、Dx11頂点ﾊﾞｯﾌｧを作成.
		bd.Usage = D3D11_USAGE_DEFAULT;
		//頂点を格納するのに必要なﾊﾞｲﾄ数.
		bd.ByteWidth = m_pMesh->GetNumBytesPerVertex()*m_pMesh->GetNumVertices();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pVertex;
		if (FAILED(m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_pVertexBuffer)))
		{
			_ASSERT_EXPR(false, L"頂点ﾊﾞｯﾌｧ作成失敗");
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);	//頂点ﾊﾞｯﾌｧ解放.




						//ﾃｸｽﾁｬ用のｻﾝﾌﾟﾗ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//ﾘﾆｱﾌｨﾙﾀ(線形補間).
													 //POINT:高速だが粗い.
	samDesc.AddressU
		= D3D11_TEXTURE_ADDRESS_WRAP;//ﾗｯﾋﾟﾝｸﾞﾓｰﾄﾞ(WRAP:繰り返し).
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//MIRROR: 反転繰り返し.
	//CLAMP : 端の模様を引き伸ばす.
	//BORDER: 別途境界色を決める.
	//ｻﾝﾌﾟﾗ作成.
	if (FAILED(m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear)))//(out)ｻﾝﾌﾟﾗ.
	{
		_ASSERT_EXPR(false, L"ｻﾝﾌﾟﾗ作成失敗");
		return E_FAIL;
	}

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


	return S_OK;
}

//解放関数.
void CDX9StaticMesh::Release()
{
	//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ解放.	←バグるので調査必要.
	if (m_ppIndexBuffer != nullptr) {
		for (int No = m_NumMaterials-1; No >= 0; No--) {
			if (m_ppIndexBuffer[No] != nullptr) {
				SAFE_RELEASE(m_ppIndexBuffer[No]);
			}
		}
		delete[] m_ppIndexBuffer;
		m_ppIndexBuffer = nullptr;
	}
	//ﾏﾃﾘｱﾙ解放.
	if (m_pMaterials != nullptr) {
		delete[] m_pMaterials;
		m_pMaterials = nullptr;
	}
	//ﾒｯｼｭﾃﾞｰﾀの解放.
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShadowMapSampler);
	SAFE_RELEASE(m_pFogTexture);
	SAFE_RELEASE(m_pToonTexture);
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pCBufferPerMaterial);
	SAFE_RELEASE(m_pCBufferPerMesh);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);

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
#endif//#ifdef _DEBUG

	//HLSLからﾊﾞｰﾃｯｸｽｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (m_EnableTexture == true){
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, nullptr, nullptr, "VS_Main", "vs_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
		{
			_ASSERT_EXPR(false, L"hlsl読み込み失敗");
			return E_FAIL;
		}
	}
	else {
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, nullptr, nullptr, "VS_NoTex", "vs_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
		{
			_ASSERT_EXPR(false, L"hlsl読み込み失敗");
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ.
	{
		_ASSERT_EXPR(false, L"ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ作成失敗");
		return E_FAIL;
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの配列要素数を算出.
	UINT numElements = 0;
	if (m_EnableTexture == true)
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);	//要素数算出.
		memcpy_s( layout, sizeof(layout),
			tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);	//要素数算出.
		memcpy_s(layout, sizeof(layout),
			tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ.
	{
		_ASSERT_EXPR(false, L"頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSLからﾋﾟｸｾﾙｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (m_EnableTexture == true){
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, nullptr, nullptr, "PS_Main", "ps_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
		{
			_ASSERT_EXPR(false, L"hlsl読み込み失敗");
			return E_FAIL;
		}
	}
	else {
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, nullptr, nullptr, "PS_NoTex", "ps_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
		{
			_ASSERT_EXPR(false, L"hlsl読み込み失敗");
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾋﾟｸｾﾙｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	{
		_ASSERT_EXPR(false, L"ﾋﾟｸｾﾙｼｪｰﾀﾞ作成失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//ｺﾝｽﾀﾝﾄ(定数)ﾊﾞｯﾌｧ作成.
	//ｼｪｰﾀﾞに特定の数値を送るﾊﾞｯﾌｧ.
	//ここでは変換行列渡し用.
	//ｼｪｰﾀﾞに World, View, Projection 行列を渡す.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを指定.
	cb.ByteWidth = sizeof(CBUFFER_PER_MESH);	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//書き込みでｱｸｾｽ.
	cb.MiscFlags = 0;	//その他のﾌﾗｸﾞ(未使用).
	cb.StructureByteStride = 0;	//構造体のｻｲｽﾞ(未使用).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

									//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧの作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(&cb,nullptr,&m_pCBufferPerMesh)))
	{
		_ASSERT_EXPR(false, L"ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗");
		return E_FAIL;
	}

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ ﾏﾃﾘｱﾙ用.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを指定.
	cb.ByteWidth = sizeof(CBUFFER_PER_MATERIAL);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//書き込みでｱｸｾｽ.
	cb.MiscFlags = 0;	//その他のﾌﾗｸﾞ(未使用).
	cb.StructureByteStride = 0;	//構造体のｻｲｽﾞ(未使用).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

									//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧの作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(&cb,nullptr,&m_pCBufferPerMaterial)))
	{
		_ASSERT_EXPR(false, L"ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗");
		return E_FAIL;
	}

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ ﾌﾚｰﾑ用.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを指定.
	cb.ByteWidth = sizeof(CBUFFER_PER_FRAME);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//書き込みでｱｸｾｽ.
	cb.MiscFlags = 0;	//その他のﾌﾗｸﾞ(未使用).
	cb.StructureByteStride = 0;	//構造体のｻｲｽﾞ(未使用).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

									//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧの作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(&cb, nullptr, &m_pCBufferPerFrame)))
	{
		_ASSERT_EXPR(false, L"ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗");
		return E_FAIL;
	}

	return S_OK;
}


//ﾚﾝﾀﾞﾘﾝｸﾞ用.
//※DirectX内のﾚﾝﾀﾞﾘﾝｸﾞ関数.
//  最終的に画面に出力するのはMainｸﾗｽのﾚﾝﾀﾞﾘﾝｸﾞ関数がやる.
void CDX9StaticMesh::Render( const bool& isTrans )
{
	//ﾜｰﾙﾄﾞ行列、ｽｹｰﾙ行列、回転行列、平行移動行列.
	D3DXMATRIX mWorld, mScale, mRot, mTran;
	D3DXMATRIX mYaw, mPitch, mRoll;

	//拡大縮小行列作成.
	D3DXMatrixScaling(
		&mScale,	//(out)計算結果.
		m_vScale.x, m_vScale.y, m_vScale.z);	//x,y,zそれぞれの拡縮値.
												//Y軸回転行列作成.
	D3DXMatrixRotationY( &mYaw, m_vRot.y);
	//X軸回転行列作成.
	D3DXMatrixRotationX( &mPitch, m_vRot.x);
	//Z軸回転行列作成.
	D3DXMatrixRotationZ( &mRoll, m_vRot.z);
	//平行移動行列作成.
	D3DXMatrixTranslation(
		&mTran,	//(out)計算結果.
		m_vPos.x, m_vPos.y, m_vPos.z);	//x,y,z座標.

										//回転行列を作成.
	mRot = mYaw * mPitch * mRoll;

	//ﾜｰﾙﾄﾞ行列作成.
	//拡縮×回転×移動 ※順番がとても大切！！.
	mWorld = mScale * mRot * mTran;

	// 影を描画したら終了.
	if( ShadowRender( mWorld ) == true ) return;
	if( isTrans == false ){
		if( TranslucentRender( mWorld ) == true ) return;
	}

	//使用するｼｪｰﾀﾞのｾｯﾄ.
	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);//頂点ｼｪｰﾀﾞ.
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);//ﾋﾟｸｾﾙｼｪｰﾀﾞ.

	// カスケードの数だけループ.
	for( int i = 0; i < CSceneTexRenderer::MAX_CASCADE; i++ ){
		m_pContext11->PSSetShaderResources( i+1, 1, &CSceneTexRenderer::GetShadowBuffer()[i] );
	}
	m_pContext11->PSSetSamplers( 1, 1, &m_pShadowMapSampler );

	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き換え開始時にMap.
	if (SUCCEEDED(m_pContext11->Map(
		m_pCBufferPerFrame, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData)))
	{
		CBUFFER_PER_FRAME cb;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

		cb.vColor = m_vColor;

		//ｶﾒﾗ位置.
		D3DXVECTOR3 camPos = CCameraManager::GetPosition();
		cb.vCamPos = D3DXVECTOR4( camPos.x, camPos.y, camPos.z, 0.0f );

		//----- ﾗｲﾄ情報 -----.
		//ライト位置.
		D3DXVECTOR3 lightPos = CLightManager::GetPosition();
		cb.vCamPos = { lightPos.x, lightPos.y, lightPos.z, 0.0f };
		//ﾗｲﾄ方向.
		D3DXVECTOR3 lightDir = CLightManager::GetDirection();
		cb.vLightDir = { lightDir.x, lightDir.y, lightDir.z, 0.0f };
		//ﾗｲﾄ回転行列.
		cb.mLightRot = CLightManager::GetRorarionMatrix();

		// ライトの行列を渡す.
		for( int i = 0; i < CSceneTexRenderer::MAX_CASCADE; i++ ){
			float f = isTrans == false ? 1.0f : 0.0f;
			cb.mLightWVP[i] = (mWorld * CLightManager::GetShadowVP()[i]) * f;
			D3DXMatrixTranspose( &cb.mLightWVP[i], &cb.mLightWVP[i] );
		}
		// カスケードの間隔幅を渡す.
		cb.SpritPos.x = CLightManager::GetSpritPos()[0];
		cb.SpritPos.y = CLightManager::GetSpritPos()[1];
		cb.SpritPos.z = CLightManager::GetSpritPos()[2];
		cb.SpritPos.w = CLightManager::GetSpritPos()[3];

		//ﾗｲﾄ強度(明るさ).
		cb.fIntensity.x = CLightManager::GetIntensity();
		//ﾗｲﾄ方向の正規化(ﾉｰﾏﾗｲｽﾞ).
		// ※ﾓﾃﾞﾙからﾗｲﾄへ向かう方向. ﾃﾞｨﾚｸｼｮﾅﾙﾗｲﾄで重要な要素.
		D3DXVec4Normalize(&cb.vLightDir, &cb.vLightDir);

		cb.IsShadow.x = m_IsShadow;

		// フォグのテクスチャ座標.
		cb.Fog = CFog::GetFog();

		memcpy_s(
			pData.pData,	//ｺﾋﾟｰ先のﾊﾞｯﾌｧ.
			pData.RowPitch,	//ｺﾋﾟｰ先のﾊﾞｯﾌｧｻｲｽﾞ.
			(void*)(&cb),	//ｺﾋﾟｰ元のﾊﾞｯﾌｧ.
			sizeof(cb));	//ｺﾋﾟｰ元のﾊﾞｯﾌｧｻｲｽﾞ.

							//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き換え終了時にUnmap.
		m_pContext11->Unmap(m_pCBufferPerFrame, 0);
	}

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使用するか？.
	m_pContext11->VSSetConstantBuffers(
		2, 1, &m_pCBufferPerFrame);	//頂点ｼｪｰﾀﾞ.
	m_pContext11->PSSetConstantBuffers(
		2, 1, &m_pCBufferPerFrame);	//ﾋﾟｸｾﾙｼｪｰﾀﾞ.

									// トゥーンマップテクスチャを渡す.
	m_pContext11->PSSetShaderResources( 5, 1, &m_pToonTexture );
	// フォグテクスチャを渡す.
	m_pContext11->PSSetShaderResources( 6, 1, &m_pFogTexture );
	//ﾒｯｼｭのﾚﾝﾀﾞﾘﾝｸﾞ.
	D3DXMATRIX mView = CCameraManager::GetViewMatrix();
	D3DXMATRIX mProj = CCameraManager::GetProjMatrix();
	RenderMesh( mWorld, mView, mProj, isTrans );
}

//ﾚﾝﾀﾞﾘﾝｸﾞ関数(ｸﾗｽ内でのみ使用する).
void CDX9StaticMesh::RenderMesh(
	D3DXMATRIX& mWorld, 
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj,
	const bool& isTrans )
{
	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き換え開始時にMap.
	if (SUCCEEDED(m_pContext11->Map(
		m_pCBufferPerMesh, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData)))
	{
		CBUFFER_PER_MESH cb;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

		//ﾜｰﾙﾄﾞ行列を渡す.
		float f = isTrans == false ? 1.0f : 0.0f;
		cb.mW = mWorld * f;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝ行列を渡す.
		D3DXMATRIX mWVP = mWorld * mView * mProj;
		D3DXMatrixTranspose(&mWVP, &mWVP);//行列を転置する.
										  //※行列の計算方法がDirectXとGPUで異なるため転置が必要.
		cb.mWVP = mWVP;

		memcpy_s(
			pData.pData,	//ｺﾋﾟｰ先のﾊﾞｯﾌｧ.
			pData.RowPitch,	//ｺﾋﾟｰ先のﾊﾞｯﾌｧｻｲｽﾞ.
			(void*)(&cb),	//ｺﾋﾟｰ元のﾊﾞｯﾌｧ.
			sizeof(cb));	//ｺﾋﾟｰ元のﾊﾞｯﾌｧｻｲｽﾞ.

							//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き換え終了時にUnmap.
		m_pContext11->Unmap(m_pCBufferPerMesh, 0);
	}

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使用するか？.
	m_pContext11->VSSetConstantBuffers(
		0, 1, &m_pCBufferPerMesh);	//頂点ｼｪｰﾀﾞ.
	m_pContext11->PSSetConstantBuffers(
		0, 1, &m_pCBufferPerMesh);	//ﾋﾟｸｾﾙｼｪｰﾀﾞ.

									//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	m_pContext11->IASetInputLayout(m_pVertexLayout);

	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	//属性の数だけ、それぞれの属性のｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを描画.
	for (DWORD No = 0; No < m_NumAttr; No++)
	{
		//使用されていないﾏﾃﾘｱﾙ対策.
		if (m_pMaterials[m_AttrID[No]].dwNumFace == 0) {
			continue;
		}
		//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧをｾｯﾄ.
		m_pContext11->IASetIndexBuffer(
			m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0);
		//ﾏﾃﾘｱﾙの各要素をｼｪｰﾀﾞに渡す.
		D3D11_MAPPED_SUBRESOURCE pDataMat;
		if (SUCCEEDED(
			m_pContext11->Map(m_pCBufferPerMaterial,
				0, D3D11_MAP_WRITE_DISCARD, 0, &pDataMat)))
		{
			CBUFFER_PER_MATERIAL cb;
			//ｱﾝﾋﾞｴﾝﾄ,ﾃﾞｨﾌｭｰｽﾞ,ｽﾍﾟｷｭﾗをｼｪｰﾀﾞに渡す.
			cb.vAmbient = m_pMaterials[m_AttrID[No]].Ambient;
			cb.vDiffuse = m_pMaterials[m_AttrID[No]].Diffuse;
			cb.vSpecular = m_pMaterials[m_AttrID[No]].Specular;

			memcpy_s(pDataMat.pData, pDataMat.RowPitch,
				(void*)&cb, sizeof(cb));

			m_pContext11->Unmap(m_pCBufferPerMaterial, 0);
		}

		//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うか？.
		m_pContext11->VSSetConstantBuffers(1, 1, &m_pCBufferPerMaterial);
		m_pContext11->PSSetConstantBuffers(1, 1, &m_pCBufferPerMaterial);

		//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
		if (m_pMaterials[m_AttrID[No]].pTexture != nullptr) {
			//ﾃｸｽﾁｬがあるとき.
			m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
			m_pContext11->PSSetShaderResources(
				0, 1, &m_pMaterials[m_AttrID[No]].pTexture);
		}
		else {
			//ﾃｸｽﾁｬがないとき.
			ID3D11ShaderResourceView* pNothing[1] = { 0 };
			m_pContext11->PSSetShaderResources(0, 1, pNothing);
		}

		//ﾌﾟﾘﾐﾃｨﾌﾞ(ﾎﾟﾘｺﾞﾝ)をﾚﾝﾀﾞﾘﾝｸﾞ.
		m_pContext11->DrawIndexed(
			m_pMaterials[m_AttrID[No]].dwNumFace * 3, 0, 0);
	}
}

// 影の描画.
bool CDX9StaticMesh::ShadowRender( const D3DXMATRIX& mWorld )
{
	if( CSceneTexRenderer::GetRenderPass() != CSceneTexRenderer::ERenderPass::Shadow ) return false;
	for( int i = 0; i < CSceneTexRenderer::MAX_CASCADE; i++ ){
		CSceneTexRenderer::SetShadowBuffer( i );
		CShadowMap::SetConstantBufferData( mWorld*CLightManager::GetShadowVP()[i] );
		//頂点ﾊﾞｯﾌｧをｾｯﾄ.
		UINT stride = m_pMesh->GetNumBytesPerVertex();
		UINT offset = 0;
		m_pContext11->IASetVertexBuffers(
			0, 1, &m_pVertexBuffer, &stride, &offset);
		//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
		m_pContext11->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//属性の数だけ、それぞれの属性のｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを描画.
		for (DWORD No = 0; No < m_NumAttr; No++)
		{
			//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧをｾｯﾄ.
			m_pContext11->IASetIndexBuffer(
				m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0);
			//ﾌﾟﾘﾐﾃｨﾌﾞ(ﾎﾟﾘｺﾞﾝ)をﾚﾝﾀﾞﾘﾝｸﾞ.
			m_pContext11->DrawIndexed(
				m_pMaterials[m_AttrID[No]].dwNumFace * 3, 0, 0);
		}
	}

	return true;
}

// 半透明の描画,
bool CDX9StaticMesh::TranslucentRender( const D3DXMATRIX& mWorld )
{
	if( CSceneTexRenderer::GetRenderPass() != CSceneTexRenderer::ERenderPass::Trans ) return false;
	CTranslucentShader::SetConstantBufferData( mWorld*CCameraManager::GetViewMatrix()*CCameraManager::GetProjMatrix() );
	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);
	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//属性の数だけ、それぞれの属性のｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを描画.
	for (DWORD No = 0; No < m_NumAttr; No++)
	{
		//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧをｾｯﾄ.
		m_pContext11->IASetIndexBuffer(
			m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0);
		//ﾌﾟﾘﾐﾃｨﾌﾞ(ﾎﾟﾘｺﾞﾝ)をﾚﾝﾀﾞﾘﾝｸﾞ.
		m_pContext11->DrawIndexed(
			m_pMaterials[m_AttrID[No]].dwNumFace * 3, 0, 0);
	}
	return true;
}