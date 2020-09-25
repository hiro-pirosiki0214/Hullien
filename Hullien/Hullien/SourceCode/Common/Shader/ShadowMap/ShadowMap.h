#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "..\ShaderBase.h"

class CShadowMap : public CShaderBase
{
	const char* SHADER_NAME = "Data\\Shader\\ShadowMap.hlsl";	// シェーダーファイル名.
																//※シェーダー内のコンスタントバッファと一致している必要あり.
	struct C_BUFFER
	{
		D3DXMATRIX	mLightWVP;		// ワールド,ビュー,プロジェクションの合成変換行列.
	};
	//頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//頂点座標.
		D3DXVECTOR3	Normal;	//法線(陰影計算に必須).
		D3DXVECTOR2	Tex;	//ﾃｸｽﾁｬ座標.
	};
public:
	CShadowMap();
	virtual ~CShadowMap();

	// インスタンスの取得.
	static CShadowMap* GetInstance();

	// 初期化.
	static HRESULT Init();
	// 解放.
	static HRESULT ReleaseShader();

	// コンスタントバッファにデータを渡す.
	static void SetConstantBufferData( const D3DXMATRIX& mLWVP, const bool isSkin = false );
	// 各種シェーダの設定.
	static void ShaderSet( ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer );

private:
	// 初期化.
	virtual HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	// 解放.
	virtual HRESULT Release() override;
	// シェーダー作成.
	virtual HRESULT InitShader() override;

private:
	ID3D11VertexShader*	m_pVertexSkinShader;	// スキンメッシュ用頂点シェーダー.
	ID3D11InputLayout*	m_pVertexSkinLayout;	// 頂点レイアウト.
};

#endif	// #ifndef SHADOW_MAP_H.