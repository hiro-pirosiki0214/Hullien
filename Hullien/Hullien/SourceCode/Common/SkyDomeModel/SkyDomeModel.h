#ifndef SKY_DOME_MODEL_H
#define SKY_DOME_MODEL_H

#include "..\Common.h"

class CSkyDomeModel : public CCommon
{
	// メッシュ単位で渡す情報.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mWVP;	// ワールド,ビュー,プロジェクションの合成変換行列.	
	};
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

	const char* SHADER_NAME = "Data\\Shader\\SkyDome.hlsl";

public:
	CSkyDomeModel();
	virtual ~CSkyDomeModel();

	// 初期化.
	HRESULT Init( const char* fileName );
	// 解放関数.
	void Release();

	// 描画.
	void Render();


private:
	// メッシュ読み込み.
	HRESULT LoadXMesh( const char* fileName );
	// シェーダ作成.
	HRESULT InitShader();
	// サンプラの作成.
	HRESULT InitSampler();

private:
	LPDIRECT3DDEVICE9		m_pDevice9;			//Dx9ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.

	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pCBufferPerMesh;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(ﾒｯｼｭ毎).
	ID3D11Buffer*			m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.
	ID3D11Buffer**			m_ppIndexBuffer;	//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ.
	ID3D11SamplerState*		m_pSampleLinear;	//ｻﾝﾌﾟﾗ:ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.


	LPD3DXMESH				m_pMesh;			//ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.
	DWORD					m_NumMaterials;		//ﾏﾃﾘｱﾙ数.
	MY_MATERIAL*			m_pMaterials;		//ﾏﾃﾘｱﾙ構造体.
	DWORD					m_NumAttr;			//属性数.
	DWORD					m_AttrID[300];		//属性ID ※300属性まで.

};

#endif	// #ifndef SKY_DOME_MODEL_H.