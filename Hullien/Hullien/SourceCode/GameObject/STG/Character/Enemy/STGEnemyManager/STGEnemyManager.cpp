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

// �������֐�.
bool STG::CEnemyManager::Init()
{
	if( m_pEnemyFactory->Create( m_Enemys ) == false ) return false;
	for( auto& e : m_Enemys ) if( e->Init() == false ) return false;
	m_EndEnemySpawnTime = m_Enemys.back()->GetSpawnTime();

	return true;
}

// �X�V�֐�.
void STG::CEnemyManager::Update( std::function<void(STG::CActor*)> updateProc )
{
	for( auto& e : m_Enemys ) updateProc( e.get() );
}

// �`��֐�.
void STG::CEnemyManager::Render()
{
	for( auto& e : m_Enemys ) e->Render();
}

// �Ō�̓G�����S������.
bool STG::CEnemyManager::IsEndEnmeyDead() const 
{ 
	return m_Enemys.back()->IsDead();
}