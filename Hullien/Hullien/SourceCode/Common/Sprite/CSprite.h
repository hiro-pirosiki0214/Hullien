#ifndef CSPRITE_UI_H
#define CSPRITE_UI_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"

class CSprite : public CCommon
{
	// 幅高さ構造体(float型).
	struct WHDIZE_FLOAT
	{
		float w;	// 幅.
		float h;	// 高さ.
	};
	enum  class enLocalPosition
	{
		None,

		LeftUp = 0,	// 左上.
		Left,		// 左.
		LeftDown,	// 左下.
		Down,		// 下.
		RightDown,	// 右下.
		Right,		// 右.
		RightUp,	// 右上.
		Up,			// 上.
		Center,		// 中央.

		Max,
	};
public:
	// スプライト構造体.
	struct SPRITE_STATE
	{
		int				LocalPosNum;// ローカル座標の
		WHDIZE_FLOAT	Disp;		// 表示幅,高さ.
		WHDIZE_FLOAT	Base;		// 元画像の幅,高さ.
		WHDIZE_FLOAT	Stride;		// 1コマ当たりの幅,高さ.
		D3DXVECTOR3		vPos;		// 座標.
		int				AnimNum;	// アニメーション数.

		enum enSPRITE_STATE
		{
			None,

			enLocalPosNum = 0,
			enDisp_w,
			enDisp_h,
			enBase_w,
			enBase_h,
			enStride_w,
			enStride_h,
			envPos_x,
			envPos_y,
			envPos_z,
			enAnimNum,

			MAX = enAnimNum,
		};
	};

	CSprite();
	CSprite(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SPRITE_STATE& pSs);

	~CSprite();

	// 初期化.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SPRITE_STATE& pSs);

	// 解放.
	void Release();

	// レンダリング.
	// 3D空間に配置.
	void Render(const bool& isBillboard = false);
	// UIとして配置.
	void RenderUI();
	// アニメーション番号の設定.
	void SetAnimNumber(const int& animNumber);
	// フレームタイムの設定(何フレームでアニメーションするか).
	void SetFrameTime(const int& frameTime) { m_FrameTime = frameTime; }
	// アニメーションを再生する.
	void PlayAnimation() { m_IsAnimation = true; }
	// 表示画像サイズの取得.
	D3DXVECTOR2 GetSpriteSize() { return { m_SState.Disp.w, m_SState.Disp.h }; }
	// 描画座標の取得.
	D3DXVECTOR3 GetRenderPos() { return m_SState.vPos; }

private:
	// モデル作成.
	HRESULT InitModel();
	// テクスチャ作成.
	HRESULT CreateTexture(const char* fileName, ID3D11ShaderResourceView** pTexture);
	HRESULT InitSample();

	D3DXMATRIX CreateWorldMatrix();
	// UV座標に変換.
	D3DXVECTOR2 ConvertIntoUV();
	// アニメーションの更新.
	void AnimUpdate();

	void CreateVERTEX(
		const float& w, const float& h,
		const float& u, const float& v);


	// テクスチャの比率を取得.
	int myGcd(int t, int t2)
	{
		if (t2 == 0) return t;
		return myGcd(t2, t % t2);
	}
private:
	std::unique_ptr<CSpriteShader> m_pSpriteShader;	// スプライトシェーダー.
	ID3D11InputLayout*		m_pVertexLayout;		// 頂点レイアウト.
	ID3D11Buffer*			m_pVertexBuffer;		// 頂点バッファ.
	ID3D11Buffer*			m_pVertexBufferUI;		// 頂点バッファ.

	ID3D11SamplerState*		m_pSampleLinear;		// サンプラ:テクスチャに各種フィルタをかける.
	ID3D11ShaderResourceView*	m_pTexture;			// テクスチャ.

	SPRITE_STATE	m_SState;			// スプライト情報.
	POINT			m_PatternNo;		// パターン番号.
	POINT			m_PatternMax;		// パターン最大数.
	D3DXVECTOR2		m_UV;				// テクスチャUV座標.
	int				m_AnimNumber;		// アニメーションナンバー.
	int				m_FrameCount;		// フレームカウント.
	int				m_FrameCountSpeed;	// フレームカウント速度.
	int				m_FrameTime;		// フレームタイム.
	bool			m_IsAnimation;		// アニメーションするか.
	enLocalPosition m_enLocalPosition;	// ローカル座標の種類.
	CSpriteShader::VERTEX m_VerticesUI[4];
};

#endif	// #ifndef CSPRITE_UI_H.