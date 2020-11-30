#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include "..\Widget.h"

class CCursor;
class CReturnTitle;

/*******************************************
*	�V�[��UI���N���X.
**/
class CSceneWidget : public CWidget
{
protected:
	inline static const float INPUT_WAIT_TIME_MAX = 30.0f;	// ���͎��̑ҋ@�t���[��.
	//	�`����.
	enum class enDrawingState
	{
		NowDrawing,	// �`�撆.
		Finish,		// �`��I��.
	} typedef EDrawingState;

public:
	CSceneWidget();
	virtual ~CSceneWidget();

protected:
	EDrawingState	m_DrawingState;		// �`����.
	float			m_InputWaitTime;	// ���͂����ۂ̑ҋ@����.

private:
};

#endif	//#ifndef SCENEWIDGET_H.