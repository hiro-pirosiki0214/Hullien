#ifndef DEBUG_TEXT_SHADER_H
#define DEBUG_TEXT_SHADER_H

#include "..\ShaderBase.h"

class CDebugTextShader : public CShaderBase
{
private: //------ 定数 ------.
	const char* SHADER_NAME = "Data\\Shader\\DebugText.hlsl";	// シェーダーファイル.

public: //------ 構造体 ------.
	// コンスタントバッファのアプリ側の定義.
	//※シェーダー内のコンスタントバッファと一致している必要あり.
	struct C_BUFFER
	{
		D3DXMATRIX	mWVP;		// ワールド,ビュー,プロジェクションの合成変換行列.
		D3DXVECTOR4 vColor;		// カラー(RGBAの型に合わせる).
		D3DXVECTOR4 fAlpha;		// アルファ値(x のみ使用).
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR2	Tex;	// テクスチャ座標.
	};
public: //------ 関数 ------.
	CDebugTextShader();
	virtual ~CDebugTextShader();
	// 初期化.
	virtual HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	// 解放.
	virtual HRESULT Release() override;

	// コンスタントバッファにデータを渡す.
	void SetConstantBufferData( const D3DXMATRIX& mWVP, const D3DXVECTOR4& color );
	// 各種シェーダの設定.
	void ShaderSet( ID3D11Buffer* pVertexBuffer, ID3D11SamplerState* pSampleLinear, ID3D11ShaderResourceView* pAsciiTexture );

private: //------ 関数 ------.
	// シェーダー作成.
	virtual HRESULT InitShader() override;
};

#endif	// #ifndef DEBUG_TEXT_SHADER_H.