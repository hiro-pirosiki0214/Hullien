#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include "..\GameWidget.h"
class CCharacter;

/*****************************************
*	キャラクタUIクラス.
**/
class CCharacterWidget : public CGameWidget
{
public:
	struct stCharacterParam
	{
		float Life;					//ライフ.
		float SpecialAbilityGauge;	//特殊ゲージ.
	}typedef SCharacterParam;

public:
	CCharacterWidget();
	virtual ~CCharacterWidget();

	// パラメータ設定関数.
	virtual void SetParameter(CCharacter& pChara) = 0;
	// パラメータ設定関数.
	virtual void SetParameter(SCharacterParam& pChara)
	{
		m_Parameter.Life = pChara.Life;
		m_Parameter.SpecialAbilityGauge = pChara.SpecialAbilityGauge;
	}

protected:
	SCharacterParam m_Parameter;	//パラメータ.

private:
};

#endif	//#ifndef CHARACTERWIDGET_H.
