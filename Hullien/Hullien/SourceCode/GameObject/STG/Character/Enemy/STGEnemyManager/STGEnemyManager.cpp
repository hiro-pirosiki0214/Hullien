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

// ‰Šú‰»ŠÖ”.
bool STG::CEnemyManager::Init()
{
	for( auto& e : m_Enemys ) if( e->Init() == false ) return false;
	return true;
}

// XVŠÖ”.
void STG::CEnemyManager::Update( std::function<void(STG::CActor*)> updateProc )
{
	for( auto& e : m_Enemys ) updateProc( e.get() );
}

// •`‰æŠÖ”.
void STG::CEnemyManager::Render()
{
	for( auto& e : m_Enemys ) e->Render();
}