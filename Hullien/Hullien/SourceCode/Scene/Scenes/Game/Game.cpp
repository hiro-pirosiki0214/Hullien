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

	, m_pGroundStage	( nullptr )
	, m_pPlayer			( nullptr )
	, m_pAlienManager	( nullptr )
	, m_pLimitTime		( nullptr )
	, m_pMiniMap		( nullptr )
{
	m_pGroundStage = std::make_shared<CGroundStage>();
	m_pPlayer = std::make_shared<CPlayer>();
	m_pAlienManager = std::make_shared<CAlienManager>();
	m_pLimitTime = std::make_unique<CLimitTime>();
	m_pMiniMap = std::make_unique<CMiniMap>();
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
	if( m_pAlienManager->Init() == false ) return false;
	if( m_pLimitTime->Init() == false ) return false;
	if( m_pMiniMap->Init() == false ) return false;

	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	// プレイヤーの当たり判定関数.
	auto playerCollProc = [&]( CActor* pActor )
	{
		m_pPlayer->Collision( pActor );
	};

	m_pPlayer->Update();
	m_pAlienManager->Update( m_pPlayer.get(), playerCollProc );

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
	m_pGroundStage->Render();
	m_pPlayer->Render();
	m_pAlienManager->Render();
	m_pLimitTime->Render();
	m_pMiniMap->Render();

}