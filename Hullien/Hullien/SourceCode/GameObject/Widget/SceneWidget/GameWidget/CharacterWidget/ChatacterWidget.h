#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include "..\GameWidget.h"

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
	void SetParameter(const CCharacterWidget::SCharacterParam& param) { m_Parameter = param; }

protected:
	SCharacterParam m_Parameter;	//パラメータ.

private:
};

#endif	//#ifndef CHARACTERWIDGET_H.
