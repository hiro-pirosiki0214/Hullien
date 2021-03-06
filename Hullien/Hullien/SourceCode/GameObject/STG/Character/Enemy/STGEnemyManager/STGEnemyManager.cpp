#include "STGEnemyManager.h"
#include "..\STGEnemyFactory\STGEnemyFactory.h"
#include "..\STGEnemy.h"

STG::CEnemyManager::CEnemyManager()
	: m_pEnemyFactory		( nullptr )
	, m_Enemys				()
	, m_EndEnemySpawnTime	( 0.0f )
{
	m_pEnemyFactory = std::make_unique<STG::CEnemyFactory>();
}

STG::CEnemyManager::~CEnemyManager()
{
}

// 初期化関数.
bool STG::CEnemyManager::Init()
{
	if( m_pEnemyFactory->Create( m_Enemys ) == false ) return false;
	for( auto& e : m_Enemys ) if( e->Init() == false ) return false;
	m_EndEnemySpawnTime = m_Enemys.back()->GetSpawnTime();

	return true;
}

// 更新関数.
void STG::CEnemyManager::Update( std::function<void(STG::CActor*)> updateProc )
{
	for( auto& e : m_Enemys ) updateProc( e.get() );
}

// 描画関数.
void STG::CEnemyManager::Render()
{
	for( auto& e : m_Enemys ) e->Render();
}

// 最後の敵が死亡したか.
bool STG::CEnemyManager::IsEndEnmeyDead() const 
{ 
	return m_Enemys.back()->IsDead();
}