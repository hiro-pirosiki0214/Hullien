#ifndef EVENT_BASE_H
#define EVENT_BASE_H

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

private:
};

#endif	//#ifndef SCENE_EVENT_BASE_H.