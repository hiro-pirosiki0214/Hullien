#ifndef SLIDER_H
#define SLIDER_H

#include "..\Widget.h"
#include <vector>

class CSlinder : public CWidget
{
	const char* SPRITE_BACK_BAR_NAME	= "BackBar";
	const char* SPRITE_BAR_NAME			= "Bar";
	const char* SPRITE_ICON_NAME		= "Icon";

	const float DEFALUT_VALUE_MAX = 1.0f;
	const float DEFALUT_VALUE_MIN = 0.0f;
	const float VALUE_MAX;
	const float VALUE_MIN;

	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_BackBar,	// 後ろのバー.
		ESpriteNo_Bar,		// バー.
		ESpriteNo_Icon,		// アイコン.

		ESpriteNo_Max,
	} typedef ESpriteNo;
public:
	CSlinder();
	CSlinder( const float& valueMax, const float& valueMin );
	virtual ~CSlinder();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// 値を増やす.
	void AddValue( const float& value );
	// 値を減らす.
	void SubValue( const float& value );
	// 値の取得.
	float GetValue() const { return m_Value; }

	// 座標設定関数.
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;
	// アイコンの座標の取得.
	D3DXVECTOR3 GetIconPosition() const { return m_IconPosition; }

private:
	// アイコンの座標の設定.
	void SetIconPosition();

	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;	// スプライトクラス.
	float		m_Value;				// 値.
	D3DXVECTOR3	m_IconPosition;			// アイコンの座標.
	float		m_BackBarImageWidthSize;// バーの画像サイズ幅.
	D3DXVECTOR3	m_BarImageWidthSize;	// バーの画像サイズ幅.
};

#endif	// #ifndef SLIDER_H.
