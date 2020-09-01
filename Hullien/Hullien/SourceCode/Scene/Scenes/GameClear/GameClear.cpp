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
	m_pFade = std::make_shared<CFade>();
	m_pFade->SetFadeOut();
}

CGameClear::~CGameClear()
{
}

//============================.
//	読込関数.
//============================.
bool CGameClear::Load()
{

	if ( m_pClearWidget->Init() == false ) return false;

	return true;
}

//============================.
//	更新関数.
//============================.
void CGameClear::Update()
{
	m_pFade->Update();

	if (m_pFade->GetFadeState() == CWidget::EFadeState::Out
		&& m_pFade->GetIsFade() == true) return;
	m_pClearWidget->Update();
	ChangeScene();
}

//============================.
//	描画関数.
//============================.
void CGameClear::Render()
{
	if (m_pClearWidget == nullptr) return;
	m_pClearWidget->Render();
}

//============================.
// シーン切り替え関数.
//============================.
void CGameClear::ChangeScene()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001) {
		m_IsChangeScene = true;
		m_pFade->SetFadeIn();
	}
	// フェードイン状態かつフェード中なら処理しない.
	if (m_pFade->GetFadeState() != CWidget::EFadeState::In) return;
	if (m_pFade->GetIsFade() == true) return;
	m_pSceneManager->NextSceneMove();
}
