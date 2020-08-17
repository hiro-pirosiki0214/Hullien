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
	// キャラクタのパラメータ.
	struct stCharacterParam
	{
		float Life;					//ライフ.
		float SpecialAbilityGauge;	//特殊ゲージ.
	}typedef SCharacterParam;


	// ゲージの情報.
	struct stGaugeState
	{
		D3DXVECTOR3 vPosition;	//位置.
		D3DXVECTOR3	vScale;		//大きさ.
		int						AnimNum;	//アニメーション番号.

		stGaugeState()
			: vPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			, vScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
			, AnimNum(0)
		{}
	}typedef SGaugeState;

public:
	CCharacterWidget();
	virtual ~CCharacterWidget();

	// パラメータ設定関数.
//	virtual void SetParameter(CCharacter& pChara) = 0;
	// パラメータ設定関数.
	virtual void SetParameter(SCharacterParam& pChara)
	{
		m_Parameter.Life = pChara.Life;
		m_Parameter.SpecialAbilityGauge = pChara.SpecialAbilityGauge;
	}

protected:
	SCharacterParam m_Parameter;							//パラメータ.

private:
};

#endif	//#ifndef CHARACTERWIDGET_H.
