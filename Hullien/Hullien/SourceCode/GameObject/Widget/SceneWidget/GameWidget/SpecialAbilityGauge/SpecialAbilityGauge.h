#ifndef SPECIALABILITYGAUGE_H
#define SPECIALABILITYGAUGE_H

#include "..\GameWidget.h"

/*******************************************
*	特殊能力ゲージクラス.
**/
class CSpecialAbilityGauge : public CGameWidget
{
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

};

#endif	//#ifndef SPECIALABILITYGAUGE_H.