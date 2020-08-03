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
	const char* SPRITE_GAGEBACK = "gagesize";	//ゲージ背景.
	const char* SPRITE_GAGE		= "gagesize";	//ゲージ.		

public:
	CLifeGauge();
	virtual ~CLifeGauge();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;
private:
	// スプライト読み込み関数.
	bool SpriteSetting();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	//スプライトクラス.
};

#endif	//#ifndef LIFEGAUGE_H
