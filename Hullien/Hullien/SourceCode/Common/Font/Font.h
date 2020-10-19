#ifndef FONT_H
#define FONT_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"

class CFont : public CCommon
{
	const float FONT_SIZE = 25.0f;	// フォントの大きさ.
	const D3DXVECTOR2 TEXTUR_UV_POS = { 1.0f, 1.0f };	// テクスチャUV座標.

public:
	CFont();
	virtual ~CFont();

	HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	void RenderUI( const std::string& text );
	void Render( const std::string& text, const bool& isBillboard = false );

private:
	// テクスチャの描画.
	void RenderFontUI( const char* c );
	// テクスチャの描画.
	void RenderFont( const char* c, const bool& isBillboard  );
	// バーテックスバッファの作成.
	HRESULT CreateVerTexBuffer();
	// サンプラーの作成.
	HRESULT CreateSampleLinear();
	// ワールドマトリックスの作成.
	D3DXMATRIX CreateWorldMatrix();
	// WVPの作成.
	D3DXMATRIX CreateWVPMatrix();

private:
	std::unique_ptr<CSpriteShader>	m_pShader;			// スプライトシェーダー.
	ID3D11Buffer*					m_pVertexBuffer;	// バーテックスバッファ.
	ID3D11Buffer*					m_pVertexBufferUI;	// 頂点バッファ.
	ID3D11SamplerState*				m_pSampleLinear;	// サンプラー.
};

#endif	// #ifndef FONT_H.