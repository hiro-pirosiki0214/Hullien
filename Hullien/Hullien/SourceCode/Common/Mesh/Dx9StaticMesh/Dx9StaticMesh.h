#ifndef DX9_STATIC_MESH_H
#define DX9_STATIC_MESH_H

#include "..\..\Common.h"
#include "..\MeshStruct.h"
#include "..\..\Fog\Fog.h"

// スタティックメッシュクラス.
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

	CDX9StaticMesh();	// コンストラクタ.
	virtual ~CDX9StaticMesh();	// デストラクタ.

						// 初期化.
	HRESULT Init(
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName );

	// 解放関数.
	void Release();

	// レンダリング用.
	void Render( const bool& isTrans = false );

	// メッシュを取得.
	LPD3DXMESH GetMesh() const { return m_pMeshForRay; }

	// 影を描画するか.
	void SetIsShadow( const bool isShadow ){ m_IsShadow = isShadow; }

private:
	// メッシュ読み込み.
	HRESULT LoadXMesh(const char* fileName);

	// シェーダ作成.
	HRESULT InitShader();

private:
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR3	Normal;	// 法線(陰影計算に必須).
		D3DXVECTOR2	Tex;	// テクスチャ座標.
	};
	// マテリアル構造体.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;	// アンビエント.
		D3DXVECTOR4	Diffuse;	// ディヒューズ.
		D3DXVECTOR4	Specular;	// スペキュラ.
		char szTextureName[64];	// テクスチャファイル名.
		ID3D11ShaderResourceView* pTexture;	// テクスチャ.
		DWORD dwNumFace;		// そのマテリアルのポリゴン数.
								// コンストラクタ.
		MY_MATERIAL()
			: Ambient(),Diffuse(),Specular(),szTextureName()
			, pTexture(nullptr),dwNumFace(0)
		{
		}
		// デストラクタ.
		~MY_MATERIAL() 
		{
			SAFE_RELEASE(pTexture);
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
	ID3D11SamplerState*			m_pToonSampleLinear;
	ID3D11SamplerState*			m_pShadowMapSampler;// シャドウマップ用サンプラー.

	LPD3DXMESH			m_pMesh;		//ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.
	LPD3DXMESH			m_pMeshForRay;	//ﾚｲとﾒｯｼｭ用.
	DWORD				m_NumMaterials;	//ﾏﾃﾘｱﾙ数.

	ID3D11ShaderResourceView*	m_pToonTexture;	// トゥーンテクスチャ.
	ID3D11ShaderResourceView*	m_pFogTexture;	// フォグテクスチャ.

	MY_MATERIAL*	m_pMaterials;	//ﾏﾃﾘｱﾙ構造体.
	DWORD			m_NumAttr;		//属性数.
	DWORD			m_AttrID[300];	//属性ID ※300属性まで.

	bool			m_EnableTexture;//ﾃｸｽﾁｬあり.
	bool			m_IsShadow;

	//ﾚﾝﾀﾞﾘﾝｸﾞ関数(ｸﾗｽ内でのみ使用する).
	void RenderMesh(
		D3DXMATRIX& mWorld, 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj,
		const bool& isTrans );
	// 影の描画.
	bool ShadowRender( const D3DXMATRIX& mWorld );
	// 半透明の描画,
	bool TranslucentRender( const D3DXMATRIX& mWorld );
};

#endif	// #ifndef DX9_STATIC_MESH_H.