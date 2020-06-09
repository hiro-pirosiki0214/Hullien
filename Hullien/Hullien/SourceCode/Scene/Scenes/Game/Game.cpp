#include "..\SceneList.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Alien\Alien_A\Alien_A.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )

	, m_pGroundStage	( nullptr )
	, m_pPlayer			( nullptr )
	, m_pEnemy			( nullptr )
{
	m_pGroundStage = std::make_shared<CGroundStage>();
	m_pPlayer = std::make_shared<CPlayer>();
	m_pEnemy = std::make_shared<CAlienA>();
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
	if( m_pEnemy->Init() == false ) return false;
	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	m_pPlayer->Update();
	m_pEnemy->SetOpponentPos( *m_pPlayer.get() );
	m_pEnemy->Update();
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
	m_pGroundStage->Render();
	m_pPlayer->Render();
	m_pEnemy->Render();
}