#include "GameClearEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"

#include "..\..\..\Common\DebugText\DebugText.h"

/************************************
*	クリアイベントクラス.
**/
CGameClearEvent::CGameClearEvent()
{
	m_IsEventEnd = false;
}

CGameClearEvent::~CGameClearEvent()
{
}

// 読み込み関数.
bool CGameClearEvent::Load()
{
	return true;
}

// 更新関数.
void CGameClearEvent::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		m_IsEventEnd = true;
	}
}

// 描画関数.
void CGameClearEvent::Render()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameClearEvent -");
}
