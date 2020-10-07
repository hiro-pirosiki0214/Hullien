#ifndef GAMEOVER_EVENT_H
#define GAMEOVER_EVENT_H

#include "..\..\EventBase\EventBase.h"

/************************************
*	ゲームオーバーイベントクラス.
**/
class CGameOverEvent : public CEventBase
{
public:
	CGameOverEvent();
	virtual ~CGameOverEvent();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// スプライト描画関数.
	virtual void SpriteRender() override;

private:
	// 次のシーンに進める.
	virtual void NextStep() override;
	// スキップ.
	virtual void Skip() override;

};

#endif	//#ifndef GAMEOVER_EVENT_H.