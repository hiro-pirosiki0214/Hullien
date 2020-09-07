#ifndef CLEAR_EVENT_H
#define CLEAR_EVENT_H

#include "..\..\EventBase\EventBase.h"

/************************************
*	クリアイベントクラス.
**/
class CClearEvent : public CEventBase
{
public:
	CClearEvent();
	virtual ~CClearEvent();

	// 読み込み関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
};


#endif	//#ifndef CLEAR_EVENT_H