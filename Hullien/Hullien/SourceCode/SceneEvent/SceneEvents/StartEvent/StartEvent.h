#ifndef START_EVENT_H
#define START_EVENT_H

#include "..\..\EventBase\EventBase.h"

/***********************************
*	スタートイベントクラス.
**/
class CStartEvent : public CEventBase
{
public:
	CStartEvent();
	virtual ~CStartEvent();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
};

#endif //#ifndef START_EVENT_H.