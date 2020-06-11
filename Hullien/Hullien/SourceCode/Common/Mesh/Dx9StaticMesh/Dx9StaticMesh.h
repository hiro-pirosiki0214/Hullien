#ifndef DX9_STATIC_MESH_H
#define DX9_STATIC_MESH_H

#include "..\..\Common.h"

//ﾒｯｼｭﾃﾞｰﾀをﾌｧｲﾙから取り出す為だけにDirectX9を使用する.
//※ﾚﾝﾀﾞﾘﾝｸﾞ(描画)は、DirectX11で行う.
class CDX9StaticMesh : public CCommon
{
public:
	CDX9StaticMesh( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName )
		: CDX9StaticMesh()
	{
		Init( hWnd, pDevice11, pContext11, pDevice9, fileName );
	}

	CDX9StaticMesh();	//ｺﾝｽﾄﾗｸﾀ.
	~CDX9StaticMesh();	//ﾃﾞｽﾄﾗｸﾀ.

	HRESULT Init(
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName );

	//ﾒｯｼｭ読み込み.
	HRESULT LoadXMesh(const char* fileName);
	//解放関数.
	void Release();

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();

	//ﾚﾝﾀﾞﾘﾝｸﾞ用.
	void Render();

	//ﾒｯｼｭを取得.
	LPD3DXMESH GetMesh() const { return m_pMeshForRay; }
	
private:
	//======================================
	//	構造体.
	//======================================
	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義.
	//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
	//ﾒｯｼｭ単位で渡す情報.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mW;			//ﾜｰﾙﾄﾞ行列.
		D3DXMATRIX	mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成変換行列.	
	};
	//ﾏﾃﾘｱﾙ単位で渡す情報.
	struct CBUFFER_PER_MATERIAL
	{
		D3DXVECTOR4 vAmbient;	//ｱﾝﾋﾞｴﾝﾄ色(環境色).
		D3DXVECTOR4	vDiffuse;	//ﾃﾞｨﾌｭｰｽﾞ色(拡散反射色).
		D3DXVECTOR4	vSpecular;	//ｽﾍﾟｷｭﾗ色(鏡面反射色).
	};
	//ﾌﾚｰﾑ単位で渡す情報.ﾗｲﾄ,ﾌｫｸﾞ情報はここに入れる.
	struct CBUFFER_PER_FRAME
	{
		D3DXVECTOR4	vCamPos;	//ｶﾒﾗ位置(視点位置).
		D3DXVECTOR4	vLightPos;	//ﾗｲﾄ位置.
		D3DXVECTOR4	vLightDir;	//ﾗｲﾄ方向.
		D3DXMATRIX	mLightRot;	//ﾗｲﾄ回転行列.
		D3DXVECTOR4	fIntensity;	//ﾗｲﾄ強度(明るさ). ※xのみ使用する.
		D3DXVECTOR4 vColor;		//色.
	};

	//頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//頂点座標.
		D3DXVECTOR3	Normal;	//法線(陰影計算に必須).
		D3DXVECTOR2	Tex;	//ﾃｸｽﾁｬ座標.
	};
	//ﾏﾃﾘｱﾙ構造体.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;	//ｱﾝﾋﾞｴﾝﾄ.
		D3DXVECTOR4	Diffuse;	//ﾃﾞｨﾌｭｰｽﾞ.
		D3DXVECTOR4	Specular;	//ｽﾍﾟｷｭﾗ.
		char szTextureName[64];	//ﾃｸｽﾁｬﾌｧｲﾙ名.
		ID3D11ShaderResourceView* pTexture;//ﾃｸｽﾁｬ.
		DWORD dwNumFace;		//そのﾏﾃﾘｱﾙのﾎﾟﾘｺﾞﾝ数.
		//ｺﾝｽﾄﾗｸﾀ.
		MY_MATERIAL()
			: Ambient(),Diffuse(),Specular(),szTextureName()
			, pTexture(nullptr),dwNumFace(0)
		{
		}
		//ﾃﾞｽﾄﾗｸﾀ.
		~MY_MATERIAL() 
		{
//			SAFE_RELEASE(pTexture);
		}
	};
	
	HWND				m_hWnd;		//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.

	//Dx9.
	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.

	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pCBufferPerMesh;		//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(ﾒｯｼｭ毎).
	ID3D11Buffer*			m_pCBufferPerMaterial;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(ﾏﾃﾘｱﾙ毎).
	ID3D11Buffer*			m_pCBufferPerFrame;		//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(ﾌﾚｰﾑ毎).

	ID3D11Buffer*				m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.
	ID3D11Buffer**				m_ppIndexBuffer;	//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ.
	ID3D11SamplerState*			m_pSampleLinear;	//ｻﾝﾌﾟﾗ:ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.

	LPD3DXMESH			m_pMesh;		//ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.
	LPD3DXMESH			m_pMeshForRay;	//ﾚｲとﾒｯｼｭ用.
	DWORD				m_NumMaterials;	//ﾏﾃﾘｱﾙ数.

	MY_MATERIAL*	m_pMaterials;	//ﾏﾃﾘｱﾙ構造体.
	DWORD			m_NumAttr;		//属性数.
	DWORD			m_AttrID[300];	//属性ID ※300属性まで.

	bool			m_EnableTexture;//ﾃｸｽﾁｬあり.

	//ﾚﾝﾀﾞﾘﾝｸﾞ関数(ｸﾗｽ内でのみ使用する).
	void RenderMesh(
		D3DXMATRIX& mWorld, 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj);
 };

#endif	// #ifndef DX9_STATIC_MESH_H.