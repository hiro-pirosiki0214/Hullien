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
	void Render( const std::string& text );

private:
	// テクスチャの描画.
	void RenderFont( ID3D11Texture2D* texture );
	// バーテックスバッファの作成.
	HRESULT CreateVerTexBuffer();
	// サンプラーの作成.
	HRESULT CreateSampleLinear();
	// リソースビューの作成.
	HRESULT CreateResourceView( ID3D11Texture2D* texture );
	// ワールドマトリックスの作成.
	D3DXMATRIX CreateWorldMatrix();

private:
	std::unique_ptr<CSpriteShader>	m_pShader;			// スプライトシェーダー.
	ID3D11Buffer*					m_pVertexBuffer;	// バーテックスバッファ.
	ID3D11SamplerState*				m_pSampleLinear;	// サンプラー.
	ID3D11ShaderResourceView*		m_pResourceView;	// リソースビュー.
};

#endif	// #ifndef FONT_H.