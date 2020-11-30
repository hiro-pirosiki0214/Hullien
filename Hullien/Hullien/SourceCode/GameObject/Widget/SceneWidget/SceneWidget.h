#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include "..\Widget.h"

class CCursor;
class CReturnTitle;

/*******************************************
*	シーンUI元クラス.
**/
class CSceneWidget : public CWidget
{
protected:
	inline static const float INPUT_WAIT_TIME_MAX = 30.0f;	// 入力時の待機フレーム.
	//	描画状態.
	enum class enDrawingState
	{
		NowDrawing,	// 描画中.
		Finish,		// 描画終了.
	} typedef EDrawingState;

public:
	CSceneWidget();
	virtual ~CSceneWidget();

protected:
	EDrawingState	m_DrawingState;		// 描画状態.
	float			m_InputWaitTime;	// 入力した際の待機時間.

private:
};

#endif	//#ifndef SCENEWIDGET_H.