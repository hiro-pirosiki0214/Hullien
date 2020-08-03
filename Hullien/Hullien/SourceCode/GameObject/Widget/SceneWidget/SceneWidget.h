#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include "..\Widget.h"

class CCursor;
class CReturnTitle;

/*******************************************
*	ƒV[ƒ“UIŒ³ƒNƒ‰ƒX.
**/
class CSceneWidget : public CWidget
{
protected:
	//	•`‰æó‘Ô.
	enum class enDrawingState
	{
		NowDrawing,	// •`‰æ’†.
		Finish,			// •`‰æI—¹.
	} typedef EDrawingState;

public:
	CSceneWidget();
	virtual ~CSceneWidget();

protected:
	EDrawingState m_DrawingState;		// •`‰æó‘Ô.

private:
};

#endif	//#ifndef SCENEWIDGET_H.