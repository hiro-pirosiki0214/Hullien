#include "GameActorManager.h"
#include "..\Character\Player\Player.h"
#include "..\Character\Girl\Girl.h"
#include "..\Character\Alien\AlienManager\AlienManager.h"
#include "..\Barrier\Barrier.h"
#include "..\Item\ItemManager\ItemManager.h"
#include "..\..\GroundStage\GroundStage.h"
#include "..\..\SkyDome\SkyDome.h"
#include "..\..\MotherShipUFO\MotherShipUFO.h"
#include "..\..\Widget\SceneWidget\GameWidget\Warning\Warning.h"
#include "..\..\InvisibleWall\InvisibleWall.h"

CGameActorManager::CGameActorManager()
	: m_pSkyDome			( nullptr )
	, m_pGroundStage		( nullptr )
	, m_pPlayer				( nullptr )
	, m_pGirl				( nullptr )
	, m_pMotherShipUFO		( nullptr )
	, m_pAlienManager		( nullptr )
	, m_pItemManager		( nullptr )
	, m_pBarrier			( nullptr )
	, m_pInvisibleWall		( nullptr )
	, m_ObjPositionList		()
	, m_ObjPosListCount		( 0 )
	, m_IsOllAnimationStop	( false )
{
	m_pSkyDome			= std::make_unique<CSkyDome>();
	m_pGroundStage		= std::make_shared<CGroundStage>();
	m_pPlayer			= std::make_shared<CPlayer>();
	m_pGirl				= std::make_shared<CGirl>();
	m_pMotherShipUFO	= std::make_unique<CMotherShipUFO>();
	m_pAlienManager		= std::make_shared<CAlienManager>();
	m_pItemManager		= std::make_shared<CItemManager>();
	m_pBarrier			= std::make_shared<CBarrier>();
	m_pInvisibleWall	= std::make_unique<CInvisibleWall>();
}

CGameActorManager::~CGameActorManager()
{
}

// 初期化関数.
bool CGameActorManager::Init()
{
	if( m_pSkyDome->Init()			== false ) return false;	// 背景の初期化.
	if( m_pGroundStage->Init()		== false ) return false;	// 地面の初期化.
	if( m_pInvisibleWall->Init()	== false ) return false;	// 見えない壁の初期化.
	if( m_pPlayer->Init()			== false ) return false;	// プレイヤーの初期化.
	if( m_pGirl->Init()				== false ) return false;	// 女の子の初期化.
	if( m_pMotherShipUFO->Init()	== false ) return false;	// マザーシップの初期化.
	if( m_pAlienManager->Init()		== false ) return false;	// 宇宙人管理の初期化.
	if( m_pItemManager->Init()		== false ) return false;	// アイテム管理の初期化.
	// マザーシップの座標取取得.
	m_pAlienManager->SetMotherShipUFOPos( m_pMotherShipUFO->GetPosition() );

	m_pPlayer->SetBoxWall( m_pInvisibleWall->GetBoxWall() );
	m_pGirl->SetBoxWall( m_pInvisibleWall->GetBoxWall() );

	return true;
}

// 更新関数.
void CGameActorManager::Update()
{
	m_ObjPosListCount = 0;		// カウントの初期化.
	m_ObjPositionList.clear();	// リストの初期化.

	m_pSkyDome->SetPosition( m_pPlayer->GetPosition() );

	// プレイヤーの更新.
	m_pPlayer->SetTargetPos( *m_pGirl.get() );	// 女の子の座標を取得.
	m_pPlayer->Update();						// 更新.
	SetPositionList( m_pPlayer.get() );	// 座標リストの設定.

	// バリアを使えるか.
	if( m_pPlayer->IsSpecialAbility() == true ) m_pBarrier->Init();	// バリアの初期化.

	// 女の子の更新.
	m_pGirl->Update();
	m_pGirl->SetCameraRadianX(m_pPlayer->GetCameraRadianX());	//カメラのラジアン値取得.
	SetPositionList( m_pGirl.get() );	// 座標リストの設定.
	m_pBarrier->SetTargetPos( *m_pGirl.get() );	// 女の子の座標を取得.

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

			// マザーシップの当たり判定.
			m_pMotherShipUFO->Collision( pActor );
		} );

	// マザーシップの座標を設定.
	SetPositionList( m_pMotherShipUFO.get() );

	// アイテムリストがあればアイテムを落とす.
	m_pItemManager->Drop( m_pAlienManager->GetDropItemList() );
	// アイテムの更新.
	m_pItemManager->Update( 
		[&]( CActor* pActor )
		{
			pActor->Update();						// アイテムの更新.
			SetPositionList( pActor );				// 座標リストの設定.
			pActor->Collision( m_pPlayer.get() );	// アイテムの当たり判定.
		} );
}

// 描画関数.
void CGameActorManager::Render()
{
	m_pSkyDome->Render();
	m_pGroundStage->Render();	// ステージの描画.
	m_pPlayer->Render();		// プレイヤーの描画.
	m_pGirl->Render();			// 女の子の描画.
	m_pAlienManager->Render();	// 宇宙人達の描画.
	m_pMotherShipUFO->Render();	// マザーシップの描画.
	m_pItemManager->Render();	// アイテムの描画.
//	m_pBarrier->Render();		// バリアの描画.
	m_pInvisibleWall->Render();	// 見えない壁の描画.

	// エフェクトの描画.
	m_pPlayer->EffectRender();			// プレイヤーのエフェクト描画.
	m_pAlienManager->EffectRender();	// 宇宙人のエフェクト描画.
	m_pBarrier->EffectRender();			// バリアエフェクト描画.
	// 仮　後で移動.
	m_pAlienManager->SpriteRender();	// スプライトの描画.
}

// スプライトの描画.
void CGameActorManager::SpriteRender()
{
	m_pGirl->SpriteRender();	//女の子のスプライト描画.
	m_pPlayer->SpriteRender();	//プレイヤーのスプライト描画.
}

// ゲームオーバーかどうか.
bool CGameActorManager::IsGameOver()
{
	if( m_pPlayer->IsDead() == false ) return false;
	AnimationStop();	// アニメーションを止める.
	return true;
}

// 女の子を連れ去っているか.
bool CGameActorManager::IsGirlAbduct()
{
	return m_pAlienManager->IsGirlAbduct();
}

// 女の子が危険な状態か.
bool CGameActorManager::IsDanger()
{
	return m_pGirl->IsDanger();
}

// 宇宙人が帰還したか.
bool CGameActorManager::IsReturnAlien()
{
	if(m_pGirl->GetPosition().y < GIRL_LIMIT_POS_Y) return false;
	return m_pMotherShipUFO->IsReturnAlien();
}

// 座標リストの設定.
void CGameActorManager::SetPositionList( CGameObject* pObj )
{
	if( pObj == nullptr ) return;
	m_ObjPosListCount++;	// オブジェクト数の加算.
	m_ObjPositionList.emplace_back( pObj->GetObjectTag(), pObj->GetPosition() );
}

// アニメーションを止める.
void CGameActorManager::AnimationStop()
{
	if( m_IsOllAnimationStop == true ) return;

	m_pPlayer->StopAnimation();
	m_pGirl->StopAnimation();
	m_pAlienManager->StopAnimation();

	m_IsOllAnimationStop = true;
}