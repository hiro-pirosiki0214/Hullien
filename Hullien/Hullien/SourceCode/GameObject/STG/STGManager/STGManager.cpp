#include "STGManager.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\SkyDome\SkyDome.h"
#include "..\Character\Player\STGPlayer.h"
#include "..\Character\Enemy\STGEnemyManager\STGEnemyManager.h"
#include "..\Bullet\STGBullet.h"

CSTGManager::CSTGManager()
	: m_pCamera			( nullptr )
	, m_pSkyDome		( nullptr )
	, m_pPlayer			( nullptr )
	, m_pEnemyManager	( nullptr )
	, m_STGEndFrame		( 0.0f )
	, m_STGNowFrame		( 0.0f )
	, m_SkyDomeRotX		( 0.0f )
{
	m_pCamera		= std::make_shared<CCamera>();
	m_pSkyDome		= std::make_unique<CSkyDome>();
	m_pPlayer		= std::make_shared<STG::CPlayer>();
	m_pEnemyManager	= std::make_unique<STG::CEnemyManager>();
}

CSTGManager::~CSTGManager()
{
}

// 初期化関数.
bool CSTGManager::Init()
{
	if( m_pSkyDome->Init()		== false ) return false;
	if( m_pPlayer->Init()		== false ) return false;
	if( m_pEnemyManager->Init()	== false ) return false;

	m_pCamera->SetPosition( CAMERA_POSITION );
	m_pCamera->SetLookPosition( CAMERA_LOOK_POSITION );
	CCameraManager::SetCamera( m_pCamera );

	return true;
}

// 更新関数.
void CSTGManager::Update()
{
	m_STGNowFrame++;

	m_SkyDomeRotX += SKY_DOME_SCROLL_SPEED;
	m_pSkyDome->SetRotationX( m_SkyDomeRotX );
	m_pSkyDome->Update();

	m_pPlayer->Controller();// プレイヤーの操作.
	m_pPlayer->Update();	// プレイヤーの更新.

	m_pEnemyManager->Update( [&]( STG::CActor* pEnemy )
	{
		// プレイヤーの座標を設定.
		pEnemy->SetTargetPos( m_pPlayer->GetPosition() );

		// 敵の更新.
		pEnemy->Update();

		// プレイヤー、敵の当たり判定.
		m_pPlayer->Collision( pEnemy );
		pEnemy->Collision( m_pPlayer.get() );

		// 弾同士の当たり判定.
		for( auto& pb : m_pPlayer->GetBullets() ){
			for( auto& eb : pEnemy->GetBullets() ){
				eb->Collision( pb.get() );
			}
		}
	});
}

// 描画関数.
void CSTGManager::Render()
{
	m_pSkyDome->Render();		// 背景の描画.
	m_pPlayer->Render();		// プレイヤーの描画.
	m_pEnemyManager->Render();	// 敵の描画.
}

// シューティングが終了したか.
bool CSTGManager::IsSTGEnd()
{
	return m_pEnemyManager->IsEndEnmeyDead();
}