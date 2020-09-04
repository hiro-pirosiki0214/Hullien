#include "..\SceneList.h"
#include "..\..\..\GameObject\Actor\ActorManager\GameActorManager.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameWidget\GameWidgetManager\GameWidgetManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ContinueWidget\ContinueWidget.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_GameObjManager	( nullptr )
	, m_WidgetManager	( nullptr )
	, m_ContinueWidget	( nullptr )
	, m_ChangeSceneState( EChangeSceneState::Clear )
	, m_IsChangeScene	( false )
{
	m_GameObjManager		= std::make_unique<CGameActorManager>();
	m_WidgetManager			= std::make_unique<CGameWidgetManager>();
	m_ContinueWidget		= std::make_unique<CContinueWidget>();
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

	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	if (CFade::GetIsFade() == true) return;

	//if (m_GameObjManager->IsDanger() == false)
	//{
	//	CSoundManager::ThreadPlayBGM("GameBGM");
	//	CSoundManager::StopBGMThread("DangerBGM");
	//}
	//else
	//{
	//	CSoundManager::ThreadPlayBGM("DangerBGM");
	//	CSoundManager::StopBGMThread("GameBGM");
	//}


	if (m_GameObjManager->IsGameOver() == false)
	{
		m_GameObjManager->Update();
		m_WidgetManager->Update(m_GameObjManager.get());
	}
	else
	{
		UpdateContinue();
	}

#if 0	// 次のシーンへ移動.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		if (CFade::GetIsFade() == true) return;
		SetChangeScene(EChangeSceneState::Clear);
	}
#else 
	if(m_WidgetManager->IsGameFinish() == true )
	{
		if (m_IsChangeScene == false)
		{
			SetChangeScene(EChangeSceneState::Clear);
		}
	}
#endif	// #if 0.

	ChangeScene();

}

//============================.
//	描画関数.
//============================.
void CGame::Render()
{
	m_GameObjManager->Render();
	m_GameObjManager->SpriteRender();
	m_WidgetManager->Render();

	if (m_GameObjManager->IsGameOver() == true)
	{
		//プレイヤーの体力が0になったか取得.
		// コンテニュー.
		m_ContinueWidget->Render();
	}
}

// コンテニュー処理関数.
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

// シーン切り替え関数.
void CGame::ChangeScene()
{
	// フェードイン状態かつフェード中なら処理しない.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if (CFade::GetIsFade() == true) return;

	// どのシーンに遷移するか.
	SelectScene();
}

// シーンの選択.
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

// シーン切り替え設定関数.
void CGame::SetChangeScene( const EChangeSceneState& changeState )
{
	m_ChangeSceneState = changeState;
	CFade::SetFadeIn();
	m_IsChangeScene = true;
}
