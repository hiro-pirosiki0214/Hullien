#include "Editor.h"
#include "..\..\SceneManager\SceneManager.h"
#include "..\..\..\Editor\ControllerEdit\ControllerEdit.h"
#include "..\..\..\Editor\SpawnEdit\SpawnEdit.h"
#include "..\..\..\Editor\AlienParamEdit\AlienParamEdit.h"
#include "..\..\..\Editor\ExplosionEdit\ExplosionEdit.h"

CEditor::CEditor( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )

	, m_pControllerEdit	( nullptr )
	, m_pSpawnEdit		( nullptr )
	, m_pAlienParamEdit	( nullptr )
	, m_pExplosionEdit	( nullptr ) 
{
	m_pControllerEdit = std::make_unique<CControllerEdit>();
	m_pSpawnEdit = std::make_unique<CSpawnEdit>();
	m_pAlienParamEdit = std::make_unique<CAlienParamEdit>();
	m_pExplosionEdit = std::make_unique<CExplosionEdit>();
}

CEditor::~CEditor()
{
}

// 読込関数.
bool CEditor::Load()
{
	if( m_pSpawnEdit->Init() == false ) return false;
	if( m_pAlienParamEdit->Init() == false ) return false;
	if( m_pExplosionEdit->Init() == false ) return false;
	return true;
}

// 更新関数.
void CEditor::Update()
{
	if( ( GetAsyncKeyState('E') & 0x8000 ) &&
		( GetAsyncKeyState('D') & 0x8000 ) ){
		if( !(GetAsyncKeyState('W') & 0x8000 ) ) return;
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
	m_pExplosionEdit->Render();

	// ImGui最終描画.
	CImGuiManager::Render();
}