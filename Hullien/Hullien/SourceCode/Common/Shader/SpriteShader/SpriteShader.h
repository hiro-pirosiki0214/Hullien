#ifndef SPRITE_SHADER_H
#define SPRITE_SHADER_H

#include "..\ShaderBase.h"

class CSpriteShader : public CShaderBase
{
	const char* SHADER_NAME = "Data\\Shader\\Sprite.hlsl";

public:
	// コンスタントバッファのアプリ側の定義.
	//※シェーダー内のコンスタントバッファと一致している必要あり.
	struct C_BUFFER
	{
		D3DXMATRIX	mW;				// ワールド行列.
		D3DXMATRIX	mWVP;
		D3DXVECTOR4 vColor;			// カラー(RGBAの型に合わせる).
		D3DXVECTOR4 vUV_ViewPort;
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR2	Tex;	// テクスチャ座標.
	};

public:
	CSpriteShader();
	virtual ~CSpriteShader();

	// 初期化.
	virtual HRESULT Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11) override;
	// 解放.
	virtual HRESULT Release() override;

	// コンスタントバッファにデータを渡す.
	void SetConstantBufferData(
		const D3DXMATRIX& mWVP,
		const float& fAlpha,
		const D3DXVECTOR2& texPos);

	// 各種シェーダの設定.
	void ShaderSet(ID3D11Buffer* pVertexBuffer);
	// 各種UI用のシェーダの設定.
	void ShaderUISet(ID3D11Buffer* pVertexBuffer);

private:
	// シェーダー作成.
	virtual HRESULT InitShader() override;

private:
	ID3D11VertexShader*		m_pVertexShaderUI;	// 頂点シェーダー UI用に使用.
};

#endif	// #ifndef SPRITE_SHADER_H.