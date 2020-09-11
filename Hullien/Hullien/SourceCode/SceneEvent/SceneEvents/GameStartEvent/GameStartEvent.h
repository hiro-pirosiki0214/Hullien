#ifndef GAMESTART_EVENT_H
#define GAMESTART_EVENT_H

#include "..\..\EventBase\EventBase.h"

class CGroundStage;		// 地面クラス.
class CEventUFO;

/***********************************
*	スタートイベントクラス.
**/
class CGameStartEvent : public CEventBase
{
public:
	CGameStartEvent();
	virtual ~CGameStartEvent();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	void DebugRender();

private:
	std::shared_ptr<CGroundStage>		m_pGroundStage;
	std::shared_ptr<CEventUFO>			m_pUFO;
};

#endif //#ifndef START_EVENT_H.