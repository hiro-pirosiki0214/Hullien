#include "..\SceneList.h"
#include "..\..\..\GameObject\Actor\ActorManager\GameActorManager.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameWidget\GameWidgetManager\GameWidgetManager.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_GameObjManager	( nullptr )
	, m_WidgetManager		( nullptr )
{
	m_GameObjManager		= std::make_unique<CGameActorManager>();
	m_WidgetManager			= std::make_unique<CGameWidgetManager>();
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
	if (m_WidgetManager->Init() == false) return false;

	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	m_GameObjManager->Update();
	m_WidgetManager->Update( m_GameObjManager.get() );

#if 1	// 次のシーンへ移動.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 )
	//if( m_pLimitTime->IsFinish() == true )
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
}