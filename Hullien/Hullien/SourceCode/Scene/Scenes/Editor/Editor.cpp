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

	, m_pControllerEdit		( nullptr )
	, m_pSpawnEdit			( nullptr )
	, m_pAlienParamEdit		( nullptr )
	, m_pExplosionEdit		( nullptr ) 
	, m_pItemEdit			( nullptr )
	, m_pPlayerEdit			( nullptr )
	, m_pGirlEdit			( nullptr )
	, m_pMotherShipUFOEdit	( nullptr )
	, m_pInvisibleWallEdit	( nullptr )
	, m_pEditCamera			( nullptr )
	, m_pGroundStage		( nullptr )
	, m_NowEditScene		( EEditScenes::None )
{
	m_pControllerEdit		= std::make_unique<CControllerEdit>();
	m_pSpawnEdit			= std::make_unique<CSpawnEdit>();
	m_pAlienParamEdit		= std::make_unique<CAlienParamEdit>();
	m_pExplosionEdit		= std::make_unique<CExplosionEdit>();
	m_pItemEdit				= std::make_unique<CItemEdit>();
	m_pPlayerEdit			= std::make_unique<CPlayerEdit>();
	m_pGirlEdit				= std::make_unique<CGirlEdit>();
	m_pMotherShipUFOEdit	= std::make_unique<CMotherShipUFOEdit>();
	m_pInvisibleWallEdit	= std::make_unique<CInvisibleWallEdit>();

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
	if( m_pSpawnEdit->Init()			== false ) return false;
	if( m_pAlienParamEdit->Init()		== false ) return false;
	if( m_pExplosionEdit->Init()		== false ) return false;
	if( m_pItemEdit->Init()				== false ) return false;
	if( m_pPlayerEdit->Init()			== false ) return false;
	if( m_pGirlEdit->Init()				== false ) return false;
	if( m_pMotherShipUFOEdit->Init()	== false ) return false;
	if( m_pInvisibleWallEdit->Init()	== false ) return false;
	if( m_pSkyDome->Init()			== false ) return false;
	if( m_pGroundStage->Init()		== false ) return false;

	m_pEditCamera->SetHWND( m_pSceneManager->GethWnd() );
	return true;
}

// 更新関数.
void CEditor::Update()
{
	CFog::Update();
	switch( m_NowEditScene )
	{
	case EEditScenes::PlayerEdit:
		m_pPlayerEdit->Update();
		break;
	case EEditScenes::GirlEdit:
		m_pGirlEdit->Update();
		break;
	case EEditScenes::SpawnEdit:
		m_pSpawnEdit->Update();
		break;
	case EEditScenes::AlienParam:
		break;
	case EEditScenes::Explosion:
		m_pExplosionEdit->Update();
		break;
	case EEditScenes::ItemEdit:
		break;
	case EEditScenes::MotherShipUFOEdit:
		m_pMotherShipUFOEdit->Update();
		break;
	case EEditScenes::InvisibleWallEdit:
		m_pInvisibleWallEdit->Update();
		break;
	default:
		break;
	}
	m_pEditCamera->Updata();
	CCameraManager::SetCamera( m_pEditCamera );
	if( ( GetAsyncKeyState('E') & 0x8000 ) &&
		( GetAsyncKeyState('D') & 0x8000 ) ){
		if( !(GetAsyncKeyState('W') & 0x8000 ) ) return;
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

	switch( m_NowEditScene )
	{
	case EEditScenes::PlayerEdit:
		m_pPlayerEdit->ModelRender();
		break;
	case EEditScenes::GirlEdit:
		m_pGirlEdit->ModelRender();
		break;
	case EEditScenes::SpawnEdit:
		m_pSpawnEdit->ModelRender();
		break;
	case EEditScenes::AlienParam:
		break;
	case EEditScenes::Explosion:
		break;
	case EEditScenes::ItemEdit:
		break;
	case EEditScenes::MotherShipUFOEdit:
		m_pMotherShipUFOEdit->ModelRender();
		break;
	case EEditScenes::InvisibleWallEdit:
		break;
	default:
		break;
	}
	//--------------------------------------------.
	// 描画パス2.
	//--------------------------------------------.
	// エフェクトなどの描画.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Trans );
	CSceneTexRenderer::SetTransBuffer();
	m_pSkyDome->Render();
	m_pGroundStage->Render();

	switch( m_NowEditScene )
	{
	case EEditScenes::PlayerEdit:
		m_pPlayerEdit->ModelRender();
		break;
	case EEditScenes::GirlEdit:
		m_pGirlEdit->ModelRender();
		break;
	case EEditScenes::SpawnEdit:
		m_pSpawnEdit->ModelRender();
		break;
	case EEditScenes::AlienParam:
		break;
	case EEditScenes::Explosion:
		m_pExplosionEdit->ModelRender();
		break;
	case EEditScenes::ItemEdit:
		break;
	case EEditScenes::MotherShipUFOEdit:
		m_pMotherShipUFOEdit->ModelRender();
		break;
	case EEditScenes::InvisibleWallEdit:
		m_pInvisibleWallEdit->ModelRender();
		break;
	default:
		break;
	}


	//--------------------------------------------.
	// 描画パス3.
	//--------------------------------------------.
	// G-Bufferにcolor, normal, depthを書き込む.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	m_pSkyDome->Render();
	m_pGroundStage->Render();

	switch( m_NowEditScene )
	{
	case EEditScenes::PlayerEdit:
		m_pPlayerEdit->ModelRender();
		break;
	case EEditScenes::GirlEdit:
		m_pGirlEdit->ModelRender();
		break;
	case EEditScenes::SpawnEdit:
		m_pSpawnEdit->ModelRender();
		break;
	case EEditScenes::AlienParam:
		break;
	case EEditScenes::Explosion:
		m_pExplosionEdit->ModelRender();
		break;
	case EEditScenes::ItemEdit:
		break;
	case EEditScenes::MotherShipUFOEdit:
		m_pMotherShipUFOEdit->ModelRender();
		break;
	case EEditScenes::InvisibleWallEdit:
		m_pInvisibleWallEdit->ModelRender();
		break;
	default:
		break;
	}
	
	//--------------------------------------------.
	// 最終描画.
	//--------------------------------------------.
	// G-Bufferを使用して、画面に描画する.

	CDirectX11::SetBackBuffer();
	CSceneTexRenderer::Render();
}

// UIの表示.
void CEditor::ImGuiRender()
{
	// ImGuiのフレーム初期化.
	CImGuiManager::SetingNewFrame();

	// コントローラーエディットの描画.
	//	m_pControllerEdit->Render();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.5f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.7f, 0.4f, 0.4f, 0.7f));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WND_W, 70));
	ImGui::Begin("Edit");

	ImGui::RadioButton("PlayerEdit", &m_NowEditScene, EEditScenes::PlayerEdit); 
	ImGui::SameLine();
	ImGui::RadioButton("GirlEdit", &m_NowEditScene, EEditScenes::GirlEdit); 
	ImGui::SameLine();
	ImGui::RadioButton("SpawnEdit", &m_NowEditScene, EEditScenes::SpawnEdit); 
	ImGui::SameLine();
	ImGui::RadioButton("AlienParam", &m_NowEditScene, EEditScenes::AlienParam);
	ImGui::SameLine();
	ImGui::RadioButton("Explosion", &m_NowEditScene, EEditScenes::Explosion);
	ImGui::SameLine();
	ImGui::RadioButton("ItemEdit", &m_NowEditScene, EEditScenes::ItemEdit);
	ImGui::SameLine();
	ImGui::RadioButton("MotherShipUFOEdit", &m_NowEditScene, EEditScenes::MotherShipUFOEdit);
	ImGui::SameLine();
	ImGui::RadioButton("InvisibleWallEdit", &m_NowEditScene, EEditScenes::InvisibleWallEdit);

	ImGui::End();

	switch( m_NowEditScene )
	{
	case EEditScenes::PlayerEdit:
		m_pPlayerEdit->Render();
		break;
	case EEditScenes::GirlEdit:
		m_pGirlEdit->Render();
		break;
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
	case EEditScenes::MotherShipUFOEdit:
		m_pMotherShipUFOEdit->Render();
		break;
	case EEditScenes::InvisibleWallEdit:
		m_pInvisibleWallEdit->Render();
		break;
	default:
		break;
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	// ImGui最終描画.
	CImGuiManager::Render();
}