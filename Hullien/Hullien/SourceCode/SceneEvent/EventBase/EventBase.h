#ifndef EVENT_BASE_H
#define EVENT_BASE_H

#include <Windows.h>
#include <string>
#include <stack>
#include <memory>

#include "..\..\Global.h"

/***********************************
*	イベントシーン元クラス.
**/
class CEventBase
{
public:
	CEventBase();
	virtual ~CEventBase();

	// 読込関数.
	virtual bool Load() = 0;
	// 更新関数.
	virtual void Update() = 0;
	// 描画関数.
	virtual void Render() = 0;
	// イベントの終了を取得.
	bool GetIsEventEnd() const { return m_IsEventEnd; };

protected:
	bool	m_IsEventEnd;	// イベントが終了したか.

private:
};

#endif	//#ifndef SCENE_EVENT_BASE_H.