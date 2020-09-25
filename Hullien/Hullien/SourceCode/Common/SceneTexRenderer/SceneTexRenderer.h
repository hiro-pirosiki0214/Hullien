#ifndef PERA_RENDERER_H
#define PERA_RENDERER_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"

#include <vector>

class CSceneTexRenderer : public CCommon
{
	const char* SHADER_NAME = "Data\\Shader\\SceneTexRenderer.hlsl";
public:
	//======================================
	//	構造体.
	//======================================
	// ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義.
	// ※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
	struct C_BUFFER
	{
		ALIGN16 D3DXMATRIX	mW;				// ワールド行列.
		ALIGN16 D3DXVECTOR2	vViewPort;		// UV座標.
	};
	//頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//頂点座標.
		D3DXVECTOR2 Tex;
	};

public:
	CSceneTexRenderer();
	virtual ~CSceneTexRenderer();

	// 初期化.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// 解放.
	void Release();

	// 描画関数.
	void Render( std::vector<ID3D11ShaderResourceView*> gbuffers );

private:
	// シェーダ作成.
	HRESULT CreateShader();
	// サンプラの作成.
	HRESULT InitSample();
	// モデル作成.
	HRESULT CreateModel();

private:
	ID3D11VertexShader*		m_pVertexShader;	// 頂点シェーダー.
	ID3D11PixelShader*		m_pPixelShader;		// ピクセルシェーダー.
	ID3D11InputLayout*		m_pVertexLayout;	// 頂点レイアウト.
	ID3D11Buffer*			m_pConstantBuffer;	// コンスタントバッファ.
	ID3D11Buffer*			m_pVertexBuffer;	// 頂点バッファ.
	ID3D11SamplerState*		m_pSampleLinear;	// サンプラ:テクスチャに各種フィルタをかける.
};

#endif	// #ifndef PERA_RENDERER_H.