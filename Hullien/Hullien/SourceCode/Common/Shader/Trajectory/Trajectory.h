#ifndef TRAJECTOTY_H
#define TRAJECTOTY_H

#include "..\ShaderBase.h"
#include <vector>

class CTrajectory : public CShaderBase
{
	// シェーダー名.
	const char* SHADER_NAME = "Data\\Shader\\Trajectory.hlsl";
	// テクスチャ名.
	const char* IMAGE_NAME = "Data\\Mesh\\Laser\\laser.png";
	// 頭のビルボード用テクスチャ名.
	const char* HEAD_IMAGE_NAME = "Data\\Mesh\\Laser\\LaserHead.png";
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
	void CreateVertexBuffer( const std::vector<D3DXVECTOR3>& height );

private:
	// シェーダー作成.
	virtual HRESULT InitShader() override;
	// テクスチャの作成.
	HRESULT CreateTexture();
	// サンプラの作成.
	HRESULT CreateSample();
	// ブレンド作成.
	HRESULT InitBlend();
	// アルファカバレージを有効:無効に設定する.
	void SetCoverage( bool EnableCoverage );

private:
	ID3D11ShaderResourceView*	m_pLaserTexture;		// レーザーテクスチャ.
	ID3D11ShaderResourceView*	m_pLaserHeadTexture;	// レーザーの頭テクスチャ.
	ID3D11SamplerState*			m_pSampleLinear;		// サンプラ.
	ID3D11Buffer*				m_pVertexBuffer;		// 頂点バッファ.
	ID3D11Buffer*				m_pHeadVertexbuffer;	// 頭の頂点バッファ.
	ID3D11BlendState*			m_pNoAlphaBlend;		// アルファブレンド無効.
	ID3D11BlendState*			m_pAlphaToCoverage;		// アルファカバレージ有効.
	int							m_VertexCount;			// 頂点数.
	D3DXVECTOR3					m_HeadPosition;			// 頭の座標.
};

#endif	// #ifndef TRAJECTOTY_H.