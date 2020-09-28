#ifndef TRAJECTOTY_H
#define TRAJECTOTY_H

#include "..\ShaderBase.h"
#include <list>

class CTrajectory : public CShaderBase
{
	const char* SHADER_NAME = "Data\\Shader\\Trajectory.hlsl";
	const char* IMAGE_NAME = "Data\\Mesh\\Barrier\\laser.png";
public:
	// コンスタントバッファのアプリ側の定義.
	//※シェーダー内のコンスタントバッファと一致している必要あり.
	struct C_BUFFER
	{
		D3DXMATRIX	mWVP;
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR2	Tex;	// テクスチャ座標.
	};
public:
	CTrajectory();
	virtual ~CTrajectory();

	// 初期化.
	virtual HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	// 解放.
	virtual HRESULT Release() override;

	// 描画.
	void Render();
	// 頂点バッファの作成.
	void CreateVertexBuffer( 
		const std::list<std::pair<D3DXVECTOR3,D3DXVECTOR3>>& height,
		const std::list<std::pair<D3DXVECTOR3,D3DXVECTOR3>>& width );

private:
	// シェーダー作成.
	virtual HRESULT InitShader() override;
	// テクスチャの作成.
	HRESULT CreateTexture();
	// サンプラの作成.
	HRESULT CreateSample();
	// 頂点バッファの作成.
	void CreateVertexBuffer( 
		const std::list<std::pair<D3DXVECTOR3,D3DXVECTOR3>>& vertexPoint,
		ID3D11Buffer** ppHeightVertexBuffer);

private:
	ID3D11ShaderResourceView*	m_pTexture;			// テクスチャ.
	ID3D11SamplerState*			m_pSampleLinear;	// サンプラ.
	ID3D11Buffer*	m_pHeightVertexBuffer;	// 高さ頂点バッファ.
	ID3D11Buffer*	m_pWidthVertexBuffer;	// 幅頂点バッファ.
	int m_VertexCount;					// 頂点数.
};

#endif	// #ifndef TRAJECTOTY_H.