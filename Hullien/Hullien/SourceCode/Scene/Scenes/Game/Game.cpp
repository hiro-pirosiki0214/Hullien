#include "..\SceneList.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Girl\Girl.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"
#include "..\..\..\GameObject\Actor\Item\ItemManager\ItemManager.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )

	, m_pGroundStage	( nullptr )
	, m_pPlayer			( nullptr )
	, m_pGirl			( nullptr )
	, m_pAlienManager	( nullptr )
	, m_pItemManager	( nullptr )
{
	m_pGroundStage = std::make_shared<CGroundStage>();
	m_pPlayer = std::make_shared<CPlayer>();
	m_pGirl = std::make_shared<CGirl>();
	m_pAlienManager = std::make_shared<CAlienManager>();
	m_pItemManager = std::make_shared<CItemManager>();
}

CGame::~CGame()
{
}

//============================.
//	読込関数.
//============================.
bool CGame::Load()
{
	if( m_pGroundStage->Init() == false ) return false;
	if( m_pPlayer->Init() == false ) return false;
	if( m_pGirl->Init() == false ) return false;
	if( m_pAlienManager->Init() == false ) return false;
	if( m_pItemManager->Init() == false ) return false;
	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	m_pPlayer->Update();	// プレイヤーの更新.
	m_pGirl->Update();		// 女の子の更新.

	// 宇宙人の更新.
	m_pAlienManager->Update( 
		m_pPlayer.get(),
		m_pGirl.get(),
		[&]( CActor* pActor )
		{
			// プレイヤー、女の子の当たり判定.
			m_pPlayer->Collision( pActor );
			m_pGirl->Collision( pActor );
		} );
	// アイテムリストがあればアイテムを落とす.
	m_pItemManager->Drop( m_pAlienManager->GetDropItemList() );
	// アイテムの更新.
	m_pItemManager->Update( m_pPlayer.get() );

#if 0	// 次のシーンへ移動.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
#endif	// #if 0.
}

//============================.
//	描画関数.
//============================.
void CGame::Render()
{

	m_pGroundStage->Render();	// ステージの描画.
	m_pPlayer->Render();		// プレイヤーの描画.
	m_pGirl->Render();			// 女の子の描画.
	m_pAlienManager->Render();	// 宇宙人達の描画.
	m_pItemManager->Render();	// アイテムの描画.
}