#ifndef BACK_GAUGE_H
#define BACK_GAUGE_H

#include "..\ChatacterWidget.h"

/*************************************
*	ライフゲージクラス.
**/
class CBackGauge : public CCharacterWidget
{
private:
	const char* SPRITE_GAUGEBACK	= "gaugeWaku";	//ゲージ背景.

public:
	CBackGauge();
	virtual ~CBackGauge();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
};

#endif	// #ifndef BACK_GAUGE_H.