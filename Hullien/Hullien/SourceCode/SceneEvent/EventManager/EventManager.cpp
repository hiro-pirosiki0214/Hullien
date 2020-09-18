#include "..\EventList.h"

/************************************
*	イベントシーン管理クラス.
**/
CEventManager::CEventManager()
	: m_pEventBase	( nullptr )
	, m_NowEvent		( EEvent::Start )
	, m_NextEvent		( EEvent::Start )
	, m_IsLoadEnd		( false )
	, m_IsSkip			( false )
	, m_IsGameOver	( false )
	, m_IsEventEnd	( false ) 
{
	NextEventMove();
}

CEventManager::~CEventManager()
{
}

// 更新関数.
void CEventManager::Update()
{
	if( m_pEventBase == nullptr ) return;

	if (m_IsLoadEnd == false)
	{
		// 読み込みが終了していなければ、読み込みを行う.
		m_IsLoadEnd = m_pEventBase->Load();
	}
	else
	{
		// 読み込みが終了したら.
		m_IsEventEnd = m_pEventBase->GetIsEventEnd();
		m_pEventBase->Update();		//イベントの更新.
		// イベントが終了していなければ描画する.
		if (m_IsEventEnd == false)
		{
			m_pEventBase->Render();	// イベントの描画.
		}
	}
}

// 次のイベントに移動.
void CEventManager::NextEventMove()
{
	m_IsLoadEnd = false;
	switch (m_NextEvent)
	{
	case EEvent::GameStart:
		m_pEventBase		= std::make_shared<CGameStartEvent>();
		m_IsGameOver	= false;
		m_IsEventEnd		= false;
		m_NowEvent		= m_NextEvent;
		m_NextEvent		= EEvent::GameClear;
		break;
	case EEvent::GameClear:
		m_pEventBase = std::make_shared<CGameClearEvent>();
		m_IsEventEnd = false;
		m_NowEvent = m_NextEvent;
		m_NextEvent = EEvent::GameStart;
		break;
	case EEvent::GameOver:
		m_pEventBase = std::make_shared<CGameOverEvent>();
		m_IsEventEnd = false;
		m_NowEvent = m_NextEvent;
		m_NextEvent = EEvent::GameStart;
		break;
	default:
		break;
	}
}
