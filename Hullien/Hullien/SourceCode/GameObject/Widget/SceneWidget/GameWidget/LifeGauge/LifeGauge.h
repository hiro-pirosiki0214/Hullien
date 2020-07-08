#ifndef LIFEGAUGE_H
#define LIFEGAUGE_H

#include "..\GameWidget.h"

/*************************************
*	ライフゲージクラス.
**/
class CLifeGauge : public CGameWidget
{
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
};

#endif	//#ifndef LIFEGAUGE_H
