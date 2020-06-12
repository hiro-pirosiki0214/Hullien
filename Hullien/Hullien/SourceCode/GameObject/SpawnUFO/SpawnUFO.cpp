#include "SpawnUFO.h"
#include "..\Actor\Character\Alien\AlienList.h"
#include "..\Actor\Character\Alien\Alien_A\Alien_A.h"

CSpawnUFO::CSpawnUFO()
	: m_SpawnParameter		()
	, m_pAbductUFOPosition	( nullptr )
	, m_pAlienParamList		( nullptr )
	, m_FrameCount			( 0 )
	, m_AlienIndex			( 0 )
{
}

CSpawnUFO::~CSpawnUFO()
{
}

// 初期化関数.
bool CSpawnUFO::Init()
{
	return true;
}

// 更新関数.
void CSpawnUFO::Update()
{
	m_FrameCount++;	// カウントの加算.
}

// 描画関数.
void CSpawnUFO::Render()
{
}

// 宇宙人をスポーンさせる.
void CSpawnUFO::SpawnAlien( std::vector<std::shared_ptr<CAlien>>& alienList )
{
	Update();	// 更新関数.

	if( m_FrameCount != m_SpawnParameter.SpawnTime ) return;
	std::shared_ptr<CAlien> tempAlien = AlienFactory();
	if( tempAlien == nullptr ) return;
	if( m_pAbductUFOPosition == nullptr ) return;

	alienList.emplace_back( tempAlien );	// 宇宙人の追加.
	// 宇宙人のパラメータリストが空なら終了.
	if( m_pAlienParamList->empty() == true ) return;
	if( m_pAlienParamList->size() <= static_cast<size_t>(m_AlienIndex) ) return;

	// リストにあったパラメータとスポーン座標を設定し、スポーンさせる.
	alienList.back()->Spawn( m_pAlienParamList->at(m_AlienIndex), m_SpawnParameter.Position );
	// 連れ去るUFOの座標を設定.
	alienList.back()->SetAbductUFOPosition( m_pAbductUFOPosition );
	m_FrameCount = 0;
}

// 宇宙人のパラメータリストを設定する.
void CSpawnUFO::SetAlienParameterList( std::vector<CAlien::SAlienParam>* alienParamList )
{
	m_pAlienParamList = alienParamList;
}

// 敵の作成.
std::shared_ptr<CAlien> CSpawnUFO::AlienFactory()
{
	const EAlienList alienNo = EAlienList::A;
	switch( alienNo )
	{
	case EAlienList::A:
		m_AlienIndex = static_cast<int>(EAlienList::A);
		m_AlienIndex--;
		return std::make_shared<CAlienA>();
	default:
		break;
	}
	return nullptr;
}