#include "..\SceneList.h"
#include "..\..\..\GameObject\Actor\ActorManager\GameActorManager.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameWidget\GameWidgetManager\GameWidgetManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ContinueWidget\ContinueWidget.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_GameObjManager	( nullptr )
	, m_WidgetManager		( nullptr )
	, m_ContinueWidget		( nullptr )
	, m_DispContinue			(0)
{
	m_GameObjManager		= std::make_unique<CGameActorManager>();
	m_WidgetManager			= std::make_unique<CGameWidgetManager>();
	m_ContinueWidget		= std::make_unique<CContinueWidget>();
}

CGame::~CGame()
{
}

//============================.
//	読込関数.
//============================.
bool CGame::Load()
{
	if( m_GameObjManager->Init() == false ) return false;
	if( m_WidgetManager->Init() == false ) return false;
	if( m_ContinueWidget->Init() == false ) return false;

	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	m_GameObjManager->Update();
	m_WidgetManager->Update( m_GameObjManager.get() );

	if (GetAsyncKeyState(VK_F1) & 0x0001)
	{
		m_DispContinue++;
		if (m_DispContinue >= 2)
		{
			m_DispContinue = 0;
		}
	}

	if (m_DispContinue == 1) 
	{
		//プレイヤーの体力が0になったか取得.
		// コンテニュー.
		m_ContinueWidget->Update();

		if ( m_ContinueWidget->GetIsDrawing() == true ) return;
		switch (m_ContinueWidget->GetSelectState())
		{
		case CContinueWidget::ESelectState::Yes:
			if (GetAsyncKeyState(VK_RETURN) & 0x0001)
			{
				// リトライ処理.
				m_pSceneManager->RetryGame();
			}
			break;
		case CContinueWidget::ESelectState::No:
			if (GetAsyncKeyState(VK_RETURN) & 0x0001)
			{
				// ゲームオーバーの設定.
				m_pSceneManager->OnGameOver();
				// 次のシーンへ移行.
				m_pSceneManager->NextSceneMove();
			}
			break;
		default:
			break;
		}
	}

#if 1	// 次のシーンへ移動.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 )
	{
		m_pSceneManager->NextSceneMove();
	}
#else 
	if( m_pLimitTime->IsFinish() == true )
	{
		m_pSceneManager->NextSceneMove();
	}
#endif	// #if 0.
}

//============================.
//	描画関数.
//============================.
void CGame::Render()
{
	m_GameObjManager->Render();
	m_WidgetManager->Render();

	if (m_DispContinue != 1) return;
	//プレイヤーの体力が0になったか取得.
	// コンテニュー.
	m_ContinueWidget->Render();
}