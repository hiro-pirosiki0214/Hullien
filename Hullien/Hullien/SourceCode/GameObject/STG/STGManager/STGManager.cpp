#include "STGManager.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\Character\Player\STGPlayer.h"
#include "..\Character\Enemy\STGEnemyManager\STGEnemyManager.h"
#include "..\Bullet\STGBullet.h"

CSTGManager::CSTGManager()
	: m_pCamera			( nullptr )
	, m_pPlayer			( nullptr )
	, m_pEnemyManager	( nullptr )
{
	m_pCamera		= std::make_shared<CCamera>();
	m_pPlayer		= std::make_shared<STG::CPlayer>();
	m_pEnemyManager	= std::make_unique<STG::CEnemyManager>();
}

CSTGManager::~CSTGManager()
{
}

// 初期化関数.
bool CSTGManager::Init()
{
	if( m_pPlayer->Init()		== false ) return false;
	if( m_pEnemyManager->Init()	== false ) return false;
	return true;
}

// 更新関数.
void CSTGManager::Update()
{
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

	m_pCamera->SetPosition( { 0.0f, 150.0f, 10.0f } );
	m_pCamera->SetLookPosition( { 0.0f, 0.0f, 0.0f } );
	CCameraManager::SetCamera( m_pCamera );
}

// 描画関数.
void CSTGManager::Render()
{
	m_pPlayer->Render();		// プレイヤーの描画.
	m_pEnemyManager->Render();	// 敵の描画.
}