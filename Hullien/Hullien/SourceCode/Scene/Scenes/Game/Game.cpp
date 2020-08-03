#include "..\SceneList.h"

#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameWidget\LimitTime\LimitTime.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameWidget\MiniMap\MiniMap.h"

#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )

	, m_pLimitTime		( nullptr )
	, m_pMiniMap		( nullptr )
  , m_GameObjManager	( nullptr )
{
	m_pLimitTime = std::make_unique<CLimitTime>();
	m_pMiniMap = std::make_unique<CMiniMap>();
  m_GameObjManager = std::make_unique<CGameActorManager>();
}

CGame::~CGame()
{
}

//============================.
//	読込関数.
//============================.
bool CGame::Load()
{
	if( m_pLimitTime->Init() == false ) return false;
	if( m_pMiniMap->Init() == false ) return false;

	if( m_GameObjManager->Init() == false ) return false;

	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	m_GameObjManager->Update();

	m_pLimitTime->Update();

#if 1	// 次のシーンへ移動.
	//if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
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
	m_pLimitTime->Render();
	m_pMiniMap->Render();

	m_GameObjManager->Render();

}