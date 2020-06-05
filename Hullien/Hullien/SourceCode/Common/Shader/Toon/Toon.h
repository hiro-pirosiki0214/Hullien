#ifndef TOON_H
#define TOON_H

#include "..\..\Common.h"

// トゥーンシェーダークラス.
class CToon
{
	inline static const char* SHADER_FIEL_PATH		= "Data\\";
	inline static const char* SHADER_ENTRY_POINT	= "Data\\";
	inline static const char* SHADER_MODEL			= "Data\\";
public:
	CToon();
	~CToon();

private:
	HRESULT InitShader();
	HRESULT InitVertexShader();
	HRESULT InitVertexLayout( ID3DBlob* pCompiledShader );

private:
	struct CBUFFER
	{
		D3DXMATRIX	mW;			// ワールド行列.
		D3DXMATRIX	mWVP;		// ワールドから射影までの変換行列.
		D3DXVECTOR4 vLightDir;	// ライト方向.
	};

	ID3D11Device*			m_pDevice11;

	ID3D11VertexShader*		m_pVertexShader;	// 頂点シェーダー.
	ID3D11InputLayout*		m_pVertexLayout;	// 頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;		// ピクセルシェーダー.

	ID3D11ShaderResourceView*	m_pToonTexture;
};

#endif	// #ifndef TOON_H.