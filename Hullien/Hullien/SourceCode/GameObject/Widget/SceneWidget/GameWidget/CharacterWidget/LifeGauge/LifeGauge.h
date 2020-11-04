#ifndef LIFEGAUGE_H
#define LIFEGAUGE_H

#include "..\ChatacterWidget.h"
#include <vector>

/*************************************
*	ライフゲージクラス.
**/
class CLifeGauge : public CCharacterWidget
{
private:
	const char* SPRITE_GAUGE		= "hpGauge";	//ゲージ.
	const char* SPRITE_GAUGEDELAY	= "hpGauge";	//ゲージ遅延.
	const int	GAUGE_NUM			= 1;		//ゲージの配列番号.
	const int	GAUGEDELAY_NUM		= 0;		//ゲージの配列番号.
	const int	ONE					= 1;		//一.

	inline static float LIFE_DELAY_SUB_VALUE = 0.005f;

public:
	CLifeGauge();
	virtual ~CLifeGauge();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;
	// パラメータ設定関数.
//	virtual void SetParameter(CCharacter& pChara) override;

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	//スプライトクラス.
	std::vector<SGaugeState> m_GaugeState;			//ゲージの情報.
	float m_OldLife;
	bool	m_IsOldLifeSet;
};

#endif	//#ifndef LIFEGAUGE_H
