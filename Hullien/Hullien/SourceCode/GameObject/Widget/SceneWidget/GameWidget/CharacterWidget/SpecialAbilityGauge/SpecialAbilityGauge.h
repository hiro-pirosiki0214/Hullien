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
	const char* SPRITE_GAUGEBACK = "gage";	//ゲージ背景.
	const char* SPRITE_GAUGE = "gage";			//ゲージ.		
	const int     GAUGE_NUM = 1;						//ゲージの配列番号.
	const int     ONE = 1;									//一.

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

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	// スプライトクラス.
	std::vector<SGaugeState> m_GaugeState;			//ゲージの情報.

};

#endif	//#ifndef SPECIALABILITYGAUGE_H.