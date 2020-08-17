#include "GameActorManager.h"
#include "..\Character\Player\Player.h"
#include "..\Character\Girl\Girl.h"
#include "..\Character\Alien\AlienManager\AlienManager.h"
#include "..\Barrier\Barrier.h"
#include "..\Item\ItemManager\ItemManager.h"
#include "..\..\GroundStage\GroundStage.h"
#include "..\..\Widget\SceneWidget\GameWidget\Warning\Warning.h"

CGameActorManager::CGameActorManager()
	: m_pGroundStage	( nullptr )
	, m_pPlayer			( nullptr )
	, m_pGirl			( nullptr )
	, m_pAlienManager	( nullptr )
	, m_pItemManager	( nullptr )
	, m_pBarrier		( nullptr )
	, m_pWarning		(nullptr)
	, m_ObjPositionList	()
	, m_ObjPosListCount	( 0 )
{
	m_pGroundStage	= std::make_shared<CGroundStage>();
	m_pPlayer		= std::make_shared<CPlayer>();
	m_pGirl			= std::make_shared<CGirl>();
	m_pAlienManager	= std::make_shared<CAlienManager>();
	m_pItemManager	= std::make_shared<CItemManager>();
	m_pBarrier		= std::make_shared<CBarrier>();
	m_pWarning = std::make_shared<CWarning>();
}

CGameActorManager::~CGameActorManager()
{
}

// 初期化関数.
bool CGameActorManager::Init()
{
	if( m_pGroundStage->Init()	== false ) return false;	// 地面の初期化.
	if( m_pPlayer->Init()		== false ) return false;	// プレイヤーの初期化.
	if( m_pGirl->Init()			== false ) return false;	// 女の子の初期化.
	if( m_pAlienManager->Init()	== false ) return false;	// 宇宙人管理の初期化.
	if( m_pItemManager->Init()	== false ) return false;	// アイテム管理の初期化.
	if (m_pWarning->Init() == false) return false;

	return true;
}

// 更新関数.
void CGameActorManager::Update()
{
	m_ObjPosListCount = 0;	// カウントの初期化.

	// プレイヤーの更新.
	m_pPlayer->Update();
	SetPositionList( m_pPlayer.get() );	// 座標リストの設定.

	// バリアを使えるか.
	if( m_pPlayer->IsSpecialAbility() == true ) m_pBarrier->Init();	// バリアの初期化.

	// 女の子の更新.
	m_pGirl->Update();
	SetPositionList( m_pGirl.get() );	// 座標リストの設定.
	m_pBarrier->SetTargetPos( *m_pGirl.get() );	// 女の子の座標を取得.

	// 女の子が捕まったか.
//	if ( m_pGirl->IsDanger() == true ) m_pWarning->Init();	// 警告の初期化.

	// バリアの更新.
	m_pBarrier->Update();

	// 宇宙人の更新.
	m_pAlienManager->Update( 
		[&]( CActor* pActor )
		{
			// 座標の設定.
			pActor->SetTargetPos( *m_pPlayer.get() );
			pActor->SetTargetPos( *m_pGirl.get() );

			// 宇宙人の更新処理.
			pActor->Update();
			SetPositionList( pActor );	// 座標リストの設定.

			// プレイヤー、女の子の当たり判定.
			m_pPlayer->Collision( pActor );
			m_pGirl->Collision( pActor );

			// 宇宙人の当たり判定.
			pActor->Collision( m_pPlayer.get() );
			pActor->Collision( m_pGirl.get() );
			pActor->Collision( m_pBarrier.get() );
		} );
	// アイテムリストがあればアイテムを落とす.
	m_pItemManager->Drop( m_pAlienManager->GetDropItemList() );
	// アイテムの更新.
	m_pItemManager->Update( 
		[&]( CActor* pActor )
		{
			pActor->Update();					// アイテムの更新.
			SetPositionList( pActor );			// 座標リストの設定.
			pActor->Collision( m_pGirl.get() );	// アイテムの当たり判定.
		} );

	// 警告の更新.
	m_pWarning->SetGirlState( m_pGirl.get() );		// 女の子の情報設定.
	m_pWarning->Update();							// 女の子の情報設定.
}

// 描画関数.
void CGameActorManager::Render()
{
	m_pGroundStage->Render();	// ステージの描画.
	m_pPlayer->Render();		// プレイヤーの描画.
	m_pGirl->Render();			// 女の子の描画.
	m_pAlienManager->Render();	// 宇宙人達の描画.
	m_pItemManager->Render();	// アイテムの描画.
	m_pBarrier->Render();		// バリアの描画.
	m_pWarning->Render();		//警告の描画.
}

// 座標リストの設定.
void CGameActorManager::SetPositionList( CActor* pActor )
{
	if( pActor == nullptr ) return;
	m_ObjPosListCount++;	// オブジェクト数の加算.
	if( static_cast<int>(m_ObjPositionList.size()) < m_ObjPosListCount ){
		// リスト数を追加.
		m_ObjPositionList.emplace_back( pActor->GetObjectTag(), pActor->GetPosition() );
	} else {
		m_ObjPositionList[m_ObjPosListCount-1] = { pActor->GetObjectTag(), pActor->GetPosition() };
	}
}