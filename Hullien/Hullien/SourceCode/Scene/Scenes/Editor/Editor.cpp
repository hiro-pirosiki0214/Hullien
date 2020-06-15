#include "Editor.h"
#include "..\..\SceneManager\SceneManager.h"
#include "..\..\..\Editor\ControllerEdit\ControllerEdit.h"
#include "..\..\..\Editor\SpawnEdit\SpawnEdit.h"
#include "..\..\..\Editor\AlienParamEdit\AlienParamEdit.h"

CEditor::CEditor( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )

	, m_pControllerEdit	( nullptr )
	, m_pSpawnEdit		( nullptr )
	, m_pAlienParamEdit	( nullptr )
{
	m_pControllerEdit = std::make_unique<CControllerEdit>();
	m_pSpawnEdit = std::make_unique<CSpawnEdit>();
	m_pAlienParamEdit = std::make_unique<CAlienParamEdit>();
}

CEditor::~CEditor()
{
}

// 読込関数.
bool CEditor::Load()
{
	if( m_pSpawnEdit->Init() == false ) return false;
	if( m_pAlienParamEdit->Init() == false ) return false;
	return true;
}

// 更新関数.
void CEditor::Update()
{
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
}

// 描画関数.
void CEditor::Render()
{
	// ImGuiのフレーム初期化.
	CImGuiManager::SetingNewFrame();

	// コントローラーエディットの描画.
	m_pControllerEdit->Render();
	m_pSpawnEdit->Render();
	m_pAlienParamEdit->Render();

	// ImGui最終描画.
	CImGuiManager::Render();
}