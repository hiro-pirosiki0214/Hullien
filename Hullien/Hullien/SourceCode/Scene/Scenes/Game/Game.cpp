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
	, m_NowEventScene	( EEventSceneState::GameStart )
	, m_NextSceneState	( ENextSceneState::None )
	, m_IsChangeScene	( false )
{
	m_GameObjManager		= std::make_unique<CGameActorManager>();
	m_WidgetManager			= std::make_unique<CGameWidgetManager>();
	m_ContinueWidget		= std::make_unique<CContinueWidget>();
	m_pEventManager			= std::make_unique<CEventManager>();
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
	
	if (m_pSceneManager->GetRetry() == false)
	{
		m_NowEventScene = EEventSceneState::GameStart;
	}
	else
	{
		m_NowEventScene = EEventSceneState::Game;
		CFade::SetFadeOut();
	}

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

	switch (m_NowEventScene)
	{
	case EEventSceneState::Game:
		GameUpdate();
		break;
	case EEventSceneState::Continue:
		ContinueUpdate();
		break;
	case EEventSceneState::GameStart:
	case EEventSceneState::GameOver_Girl:
	case EEventSceneState::GameOver_Player:
	case EEventSceneState::Clear:
		m_pEventManager->Update();
		break;
	default:
		break;
	}

	// イベントシーンの切り替え.
	ChangeEventScene();


#if 0	// 次のシーンへ移動.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		if (CFade::GetIsFade() == true) return;
		SetChangeScene(EChangeSceneState::Clear);
	}
#else 
	// 次のシーンへ移動.
	NextSceneMove();
#endif	// #if 0.
#ifdef _DEBUG
	if( GetAsyncKeyState(VK_F4) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
#endif	// #ifdef _DEBUG.
}

//============================.
//	描画関数.
//============================.
void CGame::Render()
{
	switch (m_NowEventScene)
	{
	case EEventSceneState::Game:
		ModelRender();
		m_GameObjManager->SpriteRender();
		m_WidgetManager->Render();
		break;
	case EEventSceneState::Continue:
		if (m_GameObjManager->IsGameOver() == false) {
			m_pEventManager->Render();
		}
		else {
			ModelRender();
			m_GameObjManager->SpriteRender();
			m_WidgetManager->Render();
		}
		m_ContinueWidget->Render();
		break;
	case EEventSceneState::GameStart:
	case EEventSceneState::GameOver_Girl:
	case EEventSceneState::Clear:
		m_pEventManager->Render();
		break;
	default:
		break;
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

// ゲーム処理関数.
void CGame::GameUpdate()
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

	m_GameObjManager->Update();
	m_WidgetManager->Update(m_GameObjManager.get());
}

// コンテニュー処理関数.
void CGame::ContinueUpdate()
{
	m_ContinueWidget->Update();

	if (m_ContinueWidget->GetIsDrawing() == true) return;
	switch (m_ContinueWidget->GetSelectState())
	{
	case CContinueWidget::ESelectState::Yes:
		if (GetAsyncKeyState(VK_RETURN) & 0x8000
			|| CXInput::B_Button() == CXInput::enPRESSED_MOMENT)
		{
			CSoundManager::PlaySE("DeterminationSE");
			m_NextSceneState = ENextSceneState::Game;
		}
		break;
	case CContinueWidget::ESelectState::No:
		if (GetAsyncKeyState(VK_RETURN) & 0x8000
			|| CXInput::B_Button() == CXInput::enPRESSED_MOMENT)
		{
			CSoundManager::PlaySE("End");
			m_NextSceneState = ENextSceneState::GameOver;
		}
		break;
	default:
		break;
	}
}

// シーン切り替え関数.
void CGame::ChangeEventScene()
{
	if (m_NowEventScene == EEventSceneState::Game)
	{
		// プレイヤーが死亡した場合.
		if (m_GameObjManager->IsGameOver() == true)
		{
			m_NowEventScene = EEventSceneState::Continue;
			return;
	//		SetNextScene(EEventSceneState::GameOver_Player, true);
		}
		// 女の子がUFOまで連れ去られた場合.
		if (m_GameObjManager->IsReturnAlien() == true)
		{
			SetNextScene(EEventSceneState::GameOver_Girl, true);
		}
		// ゲームクリアの場合.
		if (m_WidgetManager->IsGameFinish() == true)
		{
			SetNextScene(EEventSceneState::Clear);
		}
	}

	// イベントが終了していれば更新.
	if (m_pEventManager->GetIsEventEnd() == false) return;
	switch (m_NowEventScene)
	{
	case EEventSceneState::GameStart:
		m_NowEventScene = EEventSceneState::Game;
		break;
	case EEventSceneState::GameOver_Girl:
		m_NowEventScene = EEventSceneState::Continue;
		break;
	case EEventSceneState::Clear:
		m_NextSceneState = ENextSceneState::Clear;
		break;
	default:
		break;
	}
}

// 次のシーンに移行.
void CGame::NextSceneMove()
{
	switch (m_NextSceneState)
	{
	case ENextSceneState::Game:
		if (m_IsChangeScene == false)
		{
			CFade::SetFadeIn();
			m_IsChangeScene = true;
		}
		CSoundManager::FadeOutBGM("GameOverEvent");
		if(CSoundManager::GetIsPlayBGM("GameBGM") == true) CSoundManager::StopBGMThread("GameBGM");
		if(CSoundManager::GetIsPlayBGM("DangerBGM") == true) CSoundManager::StopBGMThread("DangerBGM");
		if(CFade::GetIsFade() == true) return;
		CSoundManager::StopBGMThread("GameOverEvent");
		m_pSceneManager->RetryGame();
		break;
	case ENextSceneState::Clear:
		m_pEventManager->NextEventMove();
		m_pSceneManager->NextSceneMove();
		break;
	case ENextSceneState::GameOver:
		if (m_IsChangeScene == false)
		{
			CFade::SetFadeIn();
			m_IsChangeScene = true;
		}
		CSoundManager::FadeOutBGM("GameOverEvent");
		if (CFade::GetIsFade() == true) return;
		if (CSoundManager::GetIsPlayBGM("GameBGM") == true) CSoundManager::StopBGMThread("GameBGM");
		if (CSoundManager::GetIsPlayBGM("DangerBGM") == true) CSoundManager::StopBGMThread("DangerBGM");
		CSoundManager::StopBGMThread("GameOverEvent");
		m_pSceneManager->OnGameOver();
		m_pEventManager->NextEventMove();
		m_pSceneManager->NextSceneMove();
		break;
	default:
		break;
	}
}

// 次のシーンを設定.
void CGame::SetNextScene(EEventSceneState state, bool GameOver)
{
	if (m_IsChangeScene == false)
	{
		CFade::SetFadeIn();
		m_IsChangeScene = true;
	}
	if (CFade::GetIsFade() == true) return;
	m_NowEventScene = state;
	CSoundManager::StopBGMThread("GameBGM");
	CSoundManager::StopBGMThread("DangerBGM");
	CSoundManager::StopBGMThread("StartEventBGM");
	if (GameOver == true)
	{
		m_pEventManager->OnGameOver();
	}
	m_pEventManager->NextEventMove();
}

