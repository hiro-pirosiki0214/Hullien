#ifndef DX9_STATIC_MESH_H
#define DX9_STATIC_MESH_H

#include "..\..\Common.h"
#include "..\MeshStruct.h"

// スタティックメッシュクラス.
class CDX9StaticMesh : public CCommon
{
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
		MY_MATERIAL()
			: Ambient		( 0.0f, 0.0f, 0.0f, 0.0f )
			, Diffuse		( 0.0f, 0.0f, 0.0f, 0.0f )
			, Specular		( 0.0f, 0.0f, 0.0f, 0.0f )
			, szTextureName	()
			, pTexture		( nullptr )
			, dwNumFace		( 0 )
		{
		}
		// デストラクタ.
		~MY_MATERIAL() { SAFE_RELEASE(pTexture); }
	};
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

	CDX9StaticMesh();			// コンストラクタ.
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
	inline LPD3DXMESH GetMesh() const { return m_pMeshForRay; }

	// 影を描画するか.
	inline void SetIsShadow( const bool isShadow ){ m_IsShadow = isShadow; }

private:
	// メッシュ読み込み.
	HRESULT LoadXMesh( const char* fileName );
	// シェーダ作成.
	HRESULT InitShader();
	//コンスタントバッファ作成関数.
	HRESULT CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size );

	// レンダリング関数(クラス内でのみ使用する).
	void RenderMesh(
		D3DXMATRIX& mWorld, 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj,
		const bool& isTrans );
	// 影の描画.
	bool ShadowRender( const D3DXMATRIX& mWorld );
	// 半透明の描画,
	bool TranslucentRender( const D3DXMATRIX& mWorld );

private:
	HWND				m_hWnd;		// ウィンドウハンドル.

	LPDIRECT3DDEVICE9	m_pDevice9;	// Dx9デバイスオブジェクト.

	ID3D11VertexShader*		m_pVertexShader;		// 頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;		// 頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;			// ピクセルシェーダー.
	ID3D11Buffer*			m_pCBufferPerMesh;		// コンスタントバッファ(メッシュ毎).
	ID3D11Buffer*			m_pCBufferPerMaterial;	// コンスタントバッファ(マテリアル毎).
	ID3D11Buffer*			m_pCBufferPerFrame;		// コンスタントバッファ(フレーム毎).

	ID3D11Buffer*				m_pVertexBuffer;	// 頂点バッファ.
	ID3D11Buffer**				m_ppIndexBuffer;	// インデックスバッファ.
	ID3D11SamplerState*			m_pSampleLinear;	// サンプラ:テクスチャに各種フィルタをかける.
	ID3D11SamplerState*			m_pToonSampleLinear;// トゥーンマップ用サンプラー.
	ID3D11SamplerState*			m_pShadowMapSampler;// シャドウマップ用サンプラー.

	LPD3DXMESH			m_pMesh;		// メッシュオブジェクト.
	LPD3DXMESH			m_pMeshForRay;	// レイとメッシュ用.
	DWORD				m_NumMaterials;	// マテリアル数.

	ID3D11ShaderResourceView*	m_pToonTexture;	// トゥーンテクスチャ.
	ID3D11ShaderResourceView*	m_pFogTexture;	// フォグテクスチャ.

	MY_MATERIAL*	m_pMaterials;	// マテリアル構造体.
	DWORD			m_NumAttr;		// 属性数.
	DWORD			m_AttrID[300];	// 属性ID ※300属性まで.

	bool			m_EnableTexture;//テクスチャあり.
	bool			m_IsShadow;		// 自分にシャドウをかけるか.
};

#endif	// #ifndef DX9_STATIC_MESH_H.