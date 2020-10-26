#ifndef SPECIALABILITYGAUGE_H
#define SPECIALABILITYGAUGE_H

#include "..\ChatacterWidget.h"
#include <vector>

/*******************************************
*	特殊能力ゲージクラス.
**/
class CSpecialAbilityGauge : public CCharacterWidget
{
private:
	const char* SPRITE_GAUGEBACK = "SPgage";	// ゲージ背景.
	const char* SPRITE_GAUGE = "SPgage";		// ゲージ.		
	const int     GAUGE_NUM = 1;						// ゲージの配列番号.
	const int     ONE = 1;									// 一.
	const int	 WAITTIME_MAX = 100;				//最大待機時間.
	const float  POS_Y = 50.0f;							// ゲージ描画位置y座標.
	const float  FADE_IN_SPEED = 0.1f;				//フェードイン速度.
	const float  FADE_OUT_SPEED = 0.01f;			//フェードアウト速度.
	const float  SCALE_MAX = 1.0f;						//大きさの最大.


public:
	CSpecialAbilityGauge();
	virtual ~CSpecialAbilityGauge();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	// バーを光らせる処理.
	void ShineBar();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;			// スプライトクラス.
	std::vector<SGaugeState>					m_GaugeState;	//ゲージの情報.
	std::vector<float>								m_Alpha;				//透過値.
	int													m_WaitTime;		//待機時間.
	bool												m_IsOnlyFirst;		//一度だけ処理する.

};

#endif	//#ifndef SPECIALABILITYGAUGE_H.