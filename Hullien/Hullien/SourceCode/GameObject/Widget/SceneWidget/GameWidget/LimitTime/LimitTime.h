#ifndef LIMITTIME_H
#define LIMITTIME_H

#include "..\\GameWidget.h"

/*****************************************
*	制限時間クラス.
**/
class CLimitTime : public CGameWidget
{
public:
	CLimitTime();
	virtual ~CLimitTime();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

private:


};

#endif	//#ifndef LIMITTIME_H.