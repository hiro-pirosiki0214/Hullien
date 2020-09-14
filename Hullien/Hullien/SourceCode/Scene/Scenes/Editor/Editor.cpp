#include "Editor.h"
#include "..\..\SceneManager\SceneManager.h"
#include "..\..\..\Editor\ControllerEdit\ControllerEdit.h"
#include "..\..\..\Editor\SpawnEdit\SpawnEdit.h"
#include "..\..\..\Editor\AlienParamEdit\AlienParamEdit.h"
#include "..\..\..\Editor\ExplosionEdit\ExplosionEdit.h"
#include "..\..\..\Editor\ItemEdit\ItemEdit.h"

CEditor::CEditor( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )

	, m_pControllerEdit	( nullptr )
	, m_pSpawnEdit		( nullptr )
	, m_pAlienParamEdit	( nullptr )
	, m_pExplosionEdit	( nullptr ) 
	, m_pItemEdit		( nullptr )
	, m_NowEditScene	( EEditScenes::None )
{
	m_pControllerEdit = std::make_unique<CControllerEdit>();
	m_pSpawnEdit = std::make_unique<CSpawnEdit>();
	m_pAlienParamEdit = std::make_unique<CAlienParamEdit>();
	m_pExplosionEdit = std::make_unique<CExplosionEdit>();
	m_pItemEdit = std::make_unique<CItemEdit>();
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
	if( m_pItemEdit->Init() == false ) return false;
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
//	m_pControllerEdit->Render();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.5f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.7f, 0.4f, 0.4f, 0.7f));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WND_W, 70));
	ImGui::Begin("config 1");

	ImGui::RadioButton("SpawnEdit", &m_NowEditScene, EEditScenes::SpawnEdit); 
	ImGui::SameLine();
	ImGui::RadioButton("AlienParam", &m_NowEditScene, EEditScenes::AlienParam);
	ImGui::SameLine();
	ImGui::RadioButton("Explosion", &m_NowEditScene, EEditScenes::Explosion);
	ImGui::SameLine();
	ImGui::RadioButton("ItemEdit", &m_NowEditScene, EEditScenes::ItemEdit);

	ImGui::End();

	switch( m_NowEditScene )
	{
	case EEditScenes::SpawnEdit:
		m_pSpawnEdit->Render();
		break;
	case EEditScenes::AlienParam:
		m_pAlienParamEdit->Render();
		break;
	case EEditScenes::Explosion:
		m_pExplosionEdit->Render();
		break;
	case EEditScenes::ItemEdit:
		m_pItemEdit->Render();
		break;
	default:
		break;
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	// ImGui最終描画.
	CImGuiManager::Render();
}