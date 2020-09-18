#ifndef GAMECLEAR_EVENT_H
#define GAMECLEAR_EVENT_H

#include "..\..\EventBase\EventBase.h"

/************************************
*	クリアイベントクラス.
**/
class CGameClearEvent : public CEventBase
{
public:
	CGameClearEvent();
	virtual ~CGameClearEvent();

	// 読み込み関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
};


#endif	//#ifndef GAMECLEAR_EVENT_H