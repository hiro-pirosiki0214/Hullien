#include "STGEnemyManager.h"
#include "..\STGEnemy.h"

STG::CEnemyManager::CEnemyManager()
	: m_Enemys	()
{
	m_Enemys.resize( 1 );
	for( auto& e : m_Enemys ) e = std::make_shared<STG::CEnemy>();
}

STG::CEnemyManager::~CEnemyManager()
{
}

// 初期化関数.
bool STG::CEnemyManager::Init()
{
	for( auto& e : m_Enemys ) if( e->Init() == false ) return false;
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