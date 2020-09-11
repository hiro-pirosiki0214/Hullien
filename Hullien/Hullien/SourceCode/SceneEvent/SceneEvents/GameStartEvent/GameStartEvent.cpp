#include "GameStartEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include ".\..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Event\EventUFO\EventUFO.h"

#include "..\..\..\Common\DebugText\DebugText.h"

/***********************************
*	スタートイベントクラス.
**/
CGameStartEvent::CGameStartEvent()
	: m_pGroundStage	( nullptr )
	, m_pUFO				( nullptr )
{
	m_pGroundStage = std::make_shared<CGroundStage>();
	m_pUFO = std::make_shared<CEventUFO>();
}

CGameStartEvent::~CGameStartEvent()
{
}

// 読込関数.
bool CGameStartEvent::Load()
{
	if ( m_pGroundStage->Init() == false ) return false;	// 地面の初期化.
	if ( m_pUFO->Init() == false ) return false;				// UFOの初期化.
	m_IsEventEnd = false;
	return true;
}

// 更新関数.
void CGameStartEvent::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		m_IsEventEnd = true;
	}
}

// 描画関数.
void CGameStartEvent::Render()
{
	m_pGroundStage->Render();	// ステージの描画.
	m_pUFO->Render();				// UFOの描画.
	DebugRender();
}

void CGameStartEvent::DebugRender()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameStartEvent -");
}

