#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "..\EventBase\EventBase.h"

// イベントの種類.
enum class enEventNo
{
	None,

	GameStart,				//ゲームスタート.
	GameClear,				//ゲームクリア.
	GameOver,				//ゲームオーバー.

	Max,

	Start = GameStart,	//初めのシーン.
}typedef EEvent;

/************************************
*	イベントシーン管理クラス.
**/
class CEventManager
{
public:
	CEventManager();
	~CEventManager();

	// 更新関数.
	void Update();
	// イベントの設定.
	void NextEvent(const EEvent& EventNo, int TypeNo = 0);
	// イベントの終了を取得.
	bool GetIsEventEnd() const { return m_IsEventEnd; };
	// スキップの設定.
	void OnSkip() { m_IsSkip = true; }


private:
	bool m_IsSkip;			//スキップするか.
	bool m_IsEventEnd;	//イベントが終了したか.
};

#endif	//#ifndef SCENE_EVENT_MANAGER_H
