#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ClearWidget\ClearWidget.h"

CGameClear::CGameClear( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
{
	m_pClearWidget = std::make_unique<CClearWidget>();
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

	return true;
}

//============================.
//	XVŠÖ”.
//============================.
void CGameClear::Update()
{
	m_pClearWidget->Update();

	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
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
}