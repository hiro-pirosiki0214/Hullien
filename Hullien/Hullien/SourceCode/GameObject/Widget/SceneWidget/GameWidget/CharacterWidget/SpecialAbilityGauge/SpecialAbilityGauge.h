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
	const char* SPRITE_GAGEBACK = "gagesize";	//ゲージ背景.
	const char* SPRITE_GAGE		= "gagesize";	//ゲージ.		

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
	bool SpriteSetting();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	// スプライトクラス.

};

#endif	//#ifndef SPECIALABILITYGAUGE_H.