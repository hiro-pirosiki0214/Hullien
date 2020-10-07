#include "..\SceneList.h"
#include "..\..\..\GameObject\Actor\ActorManager\GameActorManager.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameWidget\GameWidgetManager\GameWidgetManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ContinueWidget\ContinueWidget.h"
#include "..\..\..\GameObject\SkyDome\SkyDome.h"
#include "..\..\..\SceneEvent\EventManager\EventManager.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"

#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\Common\Shader\ShadowMap\ShadowMap.h"
#include "..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\Common\Fog\Fog.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_GameObjManager	( nullptr )
	, m_WidgetManager	( nullptr )
	, m_ContinueWidget	( nullptr )
//	, m_pSkyDome		( nullptr )
	, m_ChangeSceneState( EChangeSceneState::Clear )
	, m_IsChangeScene	( false )
{
	m_GameObjManager		= std::make_unique<CGameActorManager>();
	m_WidgetManager			= std::make_unique<CGameWidgetManager>();
	m_ContinueWidget		= std::make_unique<CContinueWidget>();
//	m_pSkyDome				= std::make_unique<CSkyDome>();
	m_pEventManager			= std::make_unique<CEventManager>();
	CFade::SetFadeOut();
}

CGame::~CGame()
{
}

//============================.
//	読込関数.
//============================.
bool CGame::Load()
{
	if( m_GameObjManager->Init() == false )	return false;
	if( m_WidgetManager->Init() == false )	return false;
	if( m_ContinueWidget->Init() == false )	return false;
//	if( m_pSkyDome->Init() == false )		return false;
	
	CSoundManager::GetInstance()->m_fMaxBGMVolume = 0.5f;
	CSoundManager::SetBGMVolume("GameBGM", CSoundManager::GetInstance()->m_fMaxBGMVolume);
	CSoundManager::SetBGMVolume("DangerBGM", CSoundManager::GetInstance()->m_fMaxBGMVolume);

	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	CFog::Update();	// フォグの更新.

	if (m_pEventManager->GetIsEventEnd() == false)
	{
		m_pEventManager->Update();
	}
	else
	{

		if (m_GameObjManager->IsDanger() == false)
		{
			CSoundManager::ThreadPlayBGM("GameBGM");
			CSoundManager::FadeInBGM("GameBGM");
			CSoundManager::FadeOutBGM("DangerBGM");
		}
		else
		{
			CSoundManager::ThreadPlayBGM("DangerBGM");
			CSoundManager::FadeInBGM("DangerBGM");
			CSoundManager::FadeOutBGM("GameBGM");
		}


		if (m_GameObjManager->IsGameOver() == false)
		{
			m_GameObjManager->Update();
			m_WidgetManager->Update(m_GameObjManager.get());
		}
		else
		{
			if (CFade::GetIsFade() == true) return;
			UpdateContinue();
		}

#if 0	// 次のシーンへ移動.
		if (GetAsyncKeyState(VK_RETURN) & 0x0001
			|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
		{
			if (CFade::GetIsFade() == true) return;
			SetChangeScene(EChangeSceneState::Clear);
		}
#else 
		if (m_WidgetManager->IsGameFinish() == true)
		{
			if (m_IsChangeScene == false)
			{
				m_pEventManager->NextEventMove();
				m_IsChangeScene = true;
			}
			if (m_pEventManager->GetIsEventEnd() == true
				&& m_ChangeSceneState == EChangeSceneState::None)
			{
				SetChangeScene(EChangeSceneState::Clear);
			}

		}
#endif	// #if 0.
	}

	ChangeScene();
}

//============================.
//	描画関数.
//============================.
void CGame::Render()
{
	if (m_pEventManager->GetIsEventEnd() == false) return;

	// モデルの描画.
	ModelRender();

	m_GameObjManager->SpriteRender();
	m_WidgetManager->Render();

	if (m_GameObjManager->IsGameOver() == true)
	{
		//プレイヤーの体力が0になったか取得.
		// コンテニュー.
		m_ContinueWidget->Render();
	}

	CEditRenderer::PushRenderProc( 
		[&]()
		{
			ImGui::Image( CSceneTexRenderer::GetTransBaffer(), ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetShadowBuffer()[1], ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetShadowBuffer()[2], ImVec2(800, 400) );
		});	
}

//============================.
// モデルの描画.
//============================.
void CGame::ModelRender()
{
	//--------------------------------------------.
	// 描画パス1.
	//--------------------------------------------.
	// 深度テクスチャに影用の深度を書き込む.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Shadow );
	m_GameObjManager->Render();

	//--------------------------------------------.
	// 描画パス2.
	//--------------------------------------------.
	// エフェクトなどの描画.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Trans );
	CSceneTexRenderer::SetTransBuffer();
	m_GameObjManager->Render();

	//--------------------------------------------.
	// 描画パス3.
	//--------------------------------------------.
	// G-Bufferにcolor, normal, depthを書き込む.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	m_GameObjManager->Render();

	//--------------------------------------------.
	// 最終描画.
	//--------------------------------------------.
	// G-Bufferを使用して、画面に描画する.

	CDirectX11::SetBackBuffer();
	CSceneTexRenderer::Render();
}

//============================.
// コンテニュー処理関数.
//============================.
void CGame::UpdateContinue()
{
	//プレイヤーの体力が0になったか取得.
	// コンテニュー.
	m_ContinueWidget->Update();

	if (m_ContinueWidget->GetIsDrawing() == true) return;
	switch (m_ContinueWidget->GetSelectState())
	{
	case CContinueWidget::ESelectState::Yes:
		if (GetAsyncKeyState(VK_RETURN) & 0x0001 
			|| CXInput::B_Button() == CXInput::enSEPARATED)
		{
			if (m_IsChangeScene == true) return;
			CSoundManager::PlaySE("DeterminationSE");
			SetChangeScene(EChangeSceneState::Game);
		}
		break;
	case CContinueWidget::ESelectState::No:
		if (GetAsyncKeyState(VK_RETURN) & 0x0001
			|| CXInput::B_Button() == CXInput::enSEPARATED)
		{
			if (m_IsChangeScene == true) return;
			CSoundManager::PlaySE("End");
			SetChangeScene( EChangeSceneState::GameOver );
		}
		break;
	default:
		break;
	}
}

//============================.
// シーン切り替え関数.
//============================.
void CGame::ChangeScene()
{
	// フェードイン状態かつフェード中なら処理しない.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if (CFade::GetIsFade() == true) return;

	// どのシーンに遷移するか.
	SelectScene();
}

//============================.
// シーンの選択.
//============================.
void CGame::SelectScene()
{
	if (m_GameObjManager->IsDanger() == false)
	{
		CSoundManager::StopBGMThread("GameBGM");
	}
	else
	{
		CSoundManager::StopBGMThread("DangerBGM");
	}

	switch (m_ChangeSceneState)
	{
	case EChangeSceneState::Game:
		// リトライ処理.
		m_pSceneManager->RetryGame();
		break;
	case EChangeSceneState::Clear:
		// 次のシーンへ移行.
		m_pSceneManager->NextSceneMove();
		break;
	case EChangeSceneState::GameOver:
		// ゲームオーバーの設定.
		m_pSceneManager->OnGameOver();
		// 次のシーンへ移行.
		m_pSceneManager->NextSceneMove();
		break;
	default:
		break;
	}
}

//============================.
// シーン切り替え設定関数.
//============================.
void CGame::SetChangeScene( const EChangeSceneState& changeState )
{
	m_ChangeSceneState = changeState;
	CFade::SetFadeIn();
	m_IsChangeScene = true;
}
