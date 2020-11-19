#include "Editor.h"
#include "..\..\SceneManager\SceneManager.h"
#include "..\..\..\Editor\ControllerEdit\ControllerEdit.h"
#include "..\..\..\Editor\SpawnEdit\SpawnEdit.h"
#include "..\..\..\Editor\AlienParamEdit\AlienParamEdit.h"
#include "..\..\..\Editor\ExplosionEdit\ExplosionEdit.h"
#include "..\..\..\Editor\ItemEdit\ItemEdit.h"
#include "..\..\..\Editor\PlayerEdit\PlayerEdit.h"
#include "..\..\..\Editor\GirlEdit\GirlEdit.h"
#include "..\..\..\Editor\MotherShipUFOEdit\MotherShipUFOEdit.h"
#include "..\..\..\Editor\InvisibleWallEdit\InvisibleWallEdit.h"
#include "..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\Common\Shader\ShadowMap\ShadowMap.h"
#include "..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\GameObject\SkyDome\SkyDome.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\Camera\EditCamera\EditCamera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\Fog\Fog.h"

CEditor::CEditor( CSceneManager* pSceneManager )
	: CSceneBase			( pSceneManager )

	, m_pEditCamera			( nullptr )
	, m_pGroundStage		( nullptr )
	, m_NowEditScene		( EEditScenes_PlayerEdit )
	, m_pEdit				()
{
	m_pEditCamera		= std::make_shared<CEditCamera>();
	m_pSkyDome			= std::make_unique<CSkyDome>();
	m_pGroundStage		= std::make_unique<CGroundStage>();
}

CEditor::~CEditor()
{
}

// 読込関数.
bool CEditor::Load()
{
	if( CreateEditList()			== false ) return false;
	if( m_pSkyDome->Init()			== false ) return false;
	if( m_pGroundStage->Init()		== false ) return false;

	m_pEditCamera->SetHWND( m_pSceneManager->GethWnd() );
	return true;
}

// 更新関数.
void CEditor::Update()
{
	CFog::Update();

	m_pEdit[m_NowEditScene-1]->Update();

	if( m_pEdit[m_NowEditScene-1]->IsSetCamera() == false ){
		m_pEditCamera->Updata();
		CCameraManager::SetCamera( m_pEditCamera );
	}
	if(( GetAsyncKeyState(VK_F6) & 0x8000 ) && ( GetAsyncKeyState(VK_SHIFT) & 0x8000 )){
		m_pSceneManager->NextSceneMove();
	}
}

// 描画関数.
void CEditor::Render()
{
	ModelRender();
	ImGuiRender();
}

// モデルの描画.
void CEditor::ModelRender()
{
	//--------------------------------------------.
	// 描画パス1.
	//--------------------------------------------.
	// 深度テクスチャに影用の深度を書き込む.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Shadow );
	m_pGroundStage->Render();

	m_pEdit[m_NowEditScene-1]->ModelRender();
	m_pEdit[m_NowEditScene-1]->EffectRender();

	//--------------------------------------------.
	// 描画パス2.
	//--------------------------------------------.
	// エフェクトなどの描画.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Trans );
	CSceneTexRenderer::SetTransBuffer();
	m_pSkyDome->Render();
	m_pGroundStage->Render();

	m_pEdit[m_NowEditScene-1]->ModelRender();
	m_pEdit[m_NowEditScene-1]->EffectRender();

	//--------------------------------------------.
	// 描画パス3.
	//--------------------------------------------.
	// G-Bufferにcolor, normal, depthを書き込む.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	m_pSkyDome->Render();
	m_pGroundStage->Render();

	m_pEdit[m_NowEditScene-1]->ModelRender();
	m_pEdit[m_NowEditScene-1]->EffectRender();

	//--------------------------------------------.
	// 最終描画.
	//--------------------------------------------.
	// G-Bufferを使用して、画面に描画する.
	CSceneTexRenderer::Render();
}

// UIの表示.
void CEditor::ImGuiRender()
{
	// ImGuiのフレーム初期化.
	CImGuiManager::SetingNewFrame();

	// コントローラーエディットの描画.
	//	m_pControllerEdit->Render();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WND_W, 70));
	ImGui::Begin("Edit");

	ImGui::RadioButton("PlayerEdit", &m_NowEditScene, EEditScenes_PlayerEdit); 
	ImGui::SameLine();
	ImGui::RadioButton("GirlEdit", &m_NowEditScene, EEditScenes_GirlEdit); 
	ImGui::SameLine();
	ImGui::RadioButton("SpawnEdit", &m_NowEditScene, EEditScenes_SpawnEdit); 
	ImGui::SameLine();
	ImGui::RadioButton("AlienParam", &m_NowEditScene, EEditScenes_AlienParam);
	ImGui::SameLine();
	ImGui::RadioButton("Explosion", &m_NowEditScene, EEditScenes_Explosion);
	ImGui::SameLine();
	ImGui::RadioButton("ItemEdit", &m_NowEditScene, EEditScenes_ItemEdit);
	ImGui::SameLine();
	ImGui::RadioButton("MotherShipUFOEdit", &m_NowEditScene, EEditScenes_MotherShipUFOEdit);
	ImGui::SameLine();
	ImGui::RadioButton("InvisibleWallEdit", &m_NowEditScene, EEditScenes_InvisibleWallEdit);

	ImGui::End();

	m_pEdit[m_NowEditScene-1]->Render();

	// ImGui最終描画.
	CImGuiManager::Render();
}

// エディットリストの作成.
bool CEditor::CreateEditList()
{
	m_pEdit.clear();
	for( int i = EEditScenes_Begin; i < EEditScenes_End; i++ ){
		switch( i+1 )
		{
		case EEditScenes_PlayerEdit:
			m_pEdit.emplace_back( std::make_unique<CPlayerEdit>() );
			break;
		case EEditScenes_GirlEdit:
			m_pEdit.emplace_back( std::make_unique<CGirlEdit>() );
			break;
		case EEditScenes_SpawnEdit:
			m_pEdit.emplace_back( std::make_unique<CSpawnEdit>() );
			break;
		case EEditScenes_AlienParam:
			m_pEdit.emplace_back( std::make_unique<CAlienParamEdit>() );
			break;
		case EEditScenes_Explosion:
			m_pEdit.emplace_back( std::make_unique<CExplosionEdit>() );
			break;
		case EEditScenes_ItemEdit:
			m_pEdit.emplace_back( std::make_unique<CItemEdit>() );
			break;
		case EEditScenes_MotherShipUFOEdit:
			m_pEdit.emplace_back( std::make_unique<CMotherShipUFOEdit>() );
			break;
		case EEditScenes_InvisibleWallEdit:
			m_pEdit.emplace_back( std::make_unique<CInvisibleWallEdit>() );
			break;
		default:
			break;
		}
		if( m_pEdit.back()->Init() == false ) return false;
	}
	return true;
}