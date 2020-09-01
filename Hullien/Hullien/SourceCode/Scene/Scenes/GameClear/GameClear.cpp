#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ClearWidget\ClearWidget.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"

CGameClear::CGameClear( CSceneManager* pSceneManager )
	: CSceneBase		   ( pSceneManager )
	, m_pClearWidget	   ( nullptr )
	, m_pFade			   ( nullptr )
	, m_IsChangeScene ( false )
{
	m_pClearWidget = std::make_unique<CClearWidget>();
	m_pFade = std::make_unique<CFade>();
}

CGameClear::~CGameClear()
{
}

//============================.
//	“ÇžŠÖ”.
//============================.
bool CGameClear::Load()
{

	if ( m_pClearWidget->Init() == false ) return false;
	if ( m_pFade->Init() == false ) return false;
	m_pFade->SetFadeOut(0.01f);

	return true;
}

//============================.
//	XVŠÖ”.
//============================.
void CGameClear::Update()
{
	if (m_pFade->GetFadeState() == CFade::EFadeState::Out
		&& m_pFade->GetIsFade() == false)
	{
		m_pFade->Update();
	}
	else
	{
		m_pClearWidget->Update();

		if (GetAsyncKeyState(VK_RETURN) & 0x0001) {
			m_IsChangeScene = true;
			m_pFade->SetFadeIn(0.01f);
		}

		if (m_IsChangeScene == false) return;
		m_pFade->Update();

		if (m_pFade->GetFadeState() == CFade::EFadeState::In
			&& m_pFade->GetIsFade() == false) return;
		m_pSceneManager->NextSceneMove();
	}
}

//============================.
//	•`‰æŠÖ”.
//============================.
void CGameClear::Render()
{
	if (m_pClearWidget == nullptr) return;
	m_pClearWidget->Render();

	if (m_pFade->GetIsFade() == false) return;
	m_pFade->Render();
}