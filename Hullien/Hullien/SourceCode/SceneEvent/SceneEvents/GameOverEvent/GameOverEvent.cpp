#include "GameOverEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"

#include "..\..\..\Common\DebugText\DebugText.h"

/************************************
*	ゲームオーバーイベントクラス.
**/
CGameOverEvent::CGameOverEvent()
{
	m_IsSkip = false;
	m_IsEventEnd = false;
}

CGameOverEvent::~CGameOverEvent()
{
}

// 読込関数.
bool CGameOverEvent::Load()
{
	return true;
}

// 更新関数.
void CGameOverEvent::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		m_IsEventEnd = true;
	}
}

// 描画関数.
void CGameOverEvent::Render()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameOverEvent -");
}

void CGameOverEvent::SpriteRender()
{
}

// 次のシーンに進める.
void CGameOverEvent::NextStep()
{
}

// スキップ.
void CGameOverEvent::Skip()
{
}
