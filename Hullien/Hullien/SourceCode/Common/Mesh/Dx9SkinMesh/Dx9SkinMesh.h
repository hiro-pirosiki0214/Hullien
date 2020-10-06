/***************************************************************************************************
*	SkinMeshCode Version 2.00
*	LastUpdate	: 2019/10/09
**/
#ifndef DX9_SKIN_MESH_H
#define DX9_SKIN_MESH_H

#include "Parser/CDX9SkinMeshParser.h"
#include "..\..\Common.h"

//==================================================================================================
//
//	スキンメッシュクラス.
//
//==================================================================================================
class CDX9SkinMesh : public CCommon
{
public:
	//メッシュ単位.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mW;			//ワールド行列.
		D3DXMATRIX	mWVP;		//ワールドから射影までの変換行列.
	};

	//マテリアル単位.
	struct CBUFFER_PER_MATERIAL
	{
		D3DXVECTOR4 vAmbient;	//アンビエント光.
		D3DXVECTOR4 vDiffuse;	//ディフューズ色.
		D3DXVECTOR4 vSpecular;	//鏡面反射.
	};

	//シェーダーに渡す値.
	struct CBUFFER_PER_FRAME
	{
		D3DXVECTOR4	vCamPos;	//ｶﾒﾗ位置(視点位置).
		D3DXVECTOR4	vLightPos;	//ﾗｲﾄ位置.
		D3DXVECTOR4	vLightDir;	//ﾗｲﾄ方向.
		D3DXMATRIX	mLightRot;	//ﾗｲﾄ回転行列.
		D3DXVECTOR4	fIntensity;	//ﾗｲﾄ強度(明るさ). ※xのみ使用する.
		D3DXVECTOR4 vColor;		//色.
		D3DXMATRIX	mLightWVP[4];
		D3DXVECTOR4	SpritPos;
		D3DXVECTOR4 IsShadow;
		D3DXVECTOR4 FogTex;		// フォグのテクスチャ座標.
	};

	//ボーン単位.
	struct CBUFFER_PER_BONES
	{
		D3DXMATRIX mBone[D3DXPARSER::MAX_BONES];
		CBUFFER_PER_BONES()
		{
			for (int i = 0; i < D3DXPARSER::MAX_BONES; i++)
			{
				D3DXMatrixIdentity(&mBone[i]);
			}
		}
	};


	//頂点構造体.
	struct MY_SKINVERTEX
	{
		D3DXVECTOR3	vPos;		//頂点位置.
		D3DXVECTOR3	vNorm;		//頂点法線.
		D3DXVECTOR2	vTex;		//UV座標.
		UINT bBoneIndex[4];		//ボーン 番号.
		float bBoneWeight[4];	//ボーン 重み.
		MY_SKINVERTEX()
			: vPos()
			, vNorm()
			, vTex()
			, bBoneIndex()
			, bBoneWeight()
		{}
	};

	//パーサークラスからアニメーションコントローラーを取得する.
	LPD3DXANIMATIONCONTROLLER GetAnimController()
	{
		return m_pD3dxMesh->m_pAnimController;
	}

	//メソッド.
	CDX9SkinMesh( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName )
		: CDX9SkinMesh()
	{
		Init( hWnd, pDevice11, pContext11, pDevice9, fileName );
	}
	CDX9SkinMesh();
	~CDX9SkinMesh();

	HRESULT Init(
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName );

	//Xファイルからスキンメッシュを作成する.
	HRESULT LoadXMesh( const char* fileName );
	//描画関数.
	void Render( const LPD3DXANIMATIONCONTROLLER pAC=NULL );
	//解放関数.
	HRESULT Release();

	double GetAnimSpeed()				{ return m_dAnimSpeed;		}
	void SetAnimSpeed( double dSpeed )	{ m_dAnimSpeed = dSpeed;	}

	double GetAnimTime()				{ return m_dAnimTime;		}
	void SetAnimTime( double dTime )	{ m_dAnimTime = dTime;		}

	//アニメーションコントローラを取得.
	LPD3DXANIMATIONCONTROLLER GetAnimationController() { return m_pD3dxMesh->m_pAnimController; }

	//アニメーションセットの切り替え.
	void ChangeAnimSet( int index, LPD3DXANIMATIONCONTROLLER pAC=nullptr );
	//アニメーションセットの切り替え(開始フレーム指定可能版).
	void ChangeAnimSet_StartPos( int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC=nullptr );
	// アニメーションをブレンドして切り替え.
	void ChangeAnimBlend( int index, int oldIndex, LPD3DXANIMATIONCONTROLLER pAC = nullptr );

	// ブレンドアニメーションの更新.
	void BlendAnimUpdate();

	//アニメーション停止時間を取得.
	double GetAnimPeriod( int index );
	//アニメーション数を取得.
	int GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC=NULL );

	//指定したボーン情報(座標・行列)を取得する関数.
	bool GetMatrixFromBone(const char* sBoneName, D3DXMATRIX* pOutMat );
	bool GetPosFromBone(const char* sBoneName, D3DXVECTOR3* pOutPos);
	bool SetMatrixFromBone(const char* sBoneName, D3DXMATRIX inMat );
	bool SetPosFromBone(const char* sBoneName, D3DXVECTOR3 inPos );
	bool GetDeviaPosFromBone(const char* sBoneName, D3DXVECTOR3* pOutPos, D3DXVECTOR3 vSpecifiedPos = { 0.0f, 0.0f, 0.0f });

	//ﾒｯｼｭを取得.
	LPD3DXMESH GetMesh() const { return m_pMeshForRay; }

private:
	HWND m_hWnd;

	//Dx9.
	LPDIRECT3DDEVICE9		m_pDevice9;

	//Dx11.
	ID3D11SamplerState*		m_pSampleLinear;
	ID3D11SamplerState*		m_pShadowMapSampler;	// シャドウマップ用サンプラー.
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pVertexLayout;
	ID3D11Buffer*			m_pCBufferPerMesh;		//コンスタントバッファ(メッシュ毎).
	ID3D11Buffer*			m_pCBufferPerMaterial;	//コンスタントバッファ(マテリアル毎).
	ID3D11Buffer*			m_pCBufferPerFrame;		//コンスタントバッファ(フレーム毎).
	ID3D11Buffer*			m_pCBufferPerBone;		//コンスタントバッファ(ボーン毎).

	ID3D11ShaderResourceView*	m_pToonTexture;	// トゥーンテクスチャ.
	ID3D11ShaderResourceView*	m_pFogTexture;	// フォグテクスチャ.

	D3DXMATRIX		m_mWorld;
	D3DXMATRIX		m_mRotation;

	D3DXMATRIX		m_mView;
	D3DXMATRIX		m_mProj;

	D3DXVECTOR3		m_CameraPos;
	D3DXVECTOR3		m_CameraLookPos;

	//アニメーション速度.
	double m_dAnimSpeed;
	double m_dAnimTime;

	bool m_IsChangeAnim;

	//解放処理用に.
	SKIN_PARTS_MESH* m_pReleaseMaterial;

	//メッシュ.
	D3DXPARSER* m_pD3dxMesh;
	LPD3DXMESH	m_pMeshForRay;	//ﾚｲとﾒｯｼｭ用.

								//XFileのパス.
	CHAR	m_FilePath[256];

	//アニメーションフレーム.
	int		m_iFrame;

	bool m_IsShadow;	// 影を描画するか.

	HRESULT InitShader();
	HRESULT CreateIndexBuffer( DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer );
	void RecursiveSetNewPoseMatrices( BONE* pBone,D3DXMATRIX* pmParent );

	//全てのメッシュを作成する.
	void BuildAllMesh( D3DXFRAME* pFrame );

	//メッシュを作成する.
	HRESULT CreateAppMeshFromD3DXMesh( LPD3DXFRAME pFrame );

	//Xファイルからスキン関連の情報を読み出す関数.
	HRESULT ReadSkinInfo( MYMESHCONTAINER* pContainer, MY_SKINVERTEX* pvVB, SKIN_PARTS_MESH* pParts );

	//ボーンを次のポーズ位置にセットする関数.
	void SetNewPoseMatrices( SKIN_PARTS_MESH* pParts, int frame, MYMESHCONTAINER* pContainer );
	//次の(現在の)ポーズ行列を返す関数.
	D3DXMATRIX GetCurrentPoseMatrix( SKIN_PARTS_MESH* pParts, int index );

	//フレーム描画.
	void DrawFrame( LPD3DXFRAME pFrame );
	//パーツ描画.
	void DrawPartsMesh( SKIN_PARTS_MESH* p, D3DXMATRIX World, MYMESHCONTAINER* pContainer );
	void DrawPartsMeshStatic( SKIN_PARTS_MESH* pMesh, D3DXMATRIX World, MYMESHCONTAINER* pContainer );

	// 影の描画.
	bool ShadowRender( SKIN_PARTS_MESH* pMesh, const D3DXMATRIX& mWorld );
	// 半透明の描画,
	bool TranslucentRender( SKIN_PARTS_MESH* pMesh, const D3DXMATRIX& mWorld );

	//全てのメッシュを削除.
	void DestroyAllMesh( D3DXFRAME* pFrame );
	HRESULT DestroyAppMeshFromD3DXMesh( LPD3DXFRAME p );

	//コンスタントバッファ作成関数.
	HRESULT CreateCBuffer(ID3D11Buffer** pConstantBuffer, UINT size);
	//サンプラー作成関数.
	HRESULT CreateLinearSampler(ID3D11SamplerState** pSampler);
};

#endif//#ifndef DX9_SKIN_MESH_H.