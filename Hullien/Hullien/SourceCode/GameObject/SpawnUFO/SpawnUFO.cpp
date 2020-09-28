#include "SpawnUFO.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\Actor\Character\Alien\Alien_A\Alien_A.h"
#include "..\Actor\Character\Alien\Alien_B\Alien_B.h"
#include "..\Actor\Character\Alien\Alien_C\Alien_C.h"
#include "..\Actor\Character\Alien\Alien_D\Alien_D.h"

CSpawnUFO::CSpawnUFO()
	: m_pStaticMesh				( nullptr )
	, m_SpawnParameter			()
	, m_pAbductUFOPosition		( nullptr )
	, m_pAlienParamList			( nullptr )
	, m_FrameCount				( 0 )
	, m_SpawnCount				( 0 )
	, m_AlienIndex				( 0 )
	, m_AlienCWaveCount			( 0 )
	, m_AlienCWaveIntervalCount	( 0 )
	, m_AlienDWaveCount			( 0 )
	, m_AlienDWaveIntervalCount	( 0 )
	, m_RandomSeed				()
{
	// ランダムシードの初期化.
	std::random_device rd;
	m_RandomSeed = std::mt19937( rd() );
}

CSpawnUFO::~CSpawnUFO()
{
}

// 初期化関数.
bool CSpawnUFO::Init()
{
	if( GetModel() == false ) return false;
	return true;
}

// 更新関数.
void CSpawnUFO::Update()
{
	m_FrameCount++;	// カウントの加算.
	m_SpawnCount++;	// スポーンカウントの加算.
	m_AlienCWaveCount++;	// 宇宙人Cのウェーブカウントの加算.
	m_AlienDWaveCount++;	// 宇宙人Dのウェーブカウントの加算.
	WaveCountUpdate(
		m_AlienCWaveCount, m_SpawnParameter.AlienCWaveTime,
		m_AlienCWaveIntervalCount, m_SpawnParameter.AlienCWaveIntervalTime );
	WaveCountUpdate(
		m_AlienDWaveCount, m_SpawnParameter.AlienDWaveTime,
		m_AlienDWaveIntervalCount, m_SpawnParameter.AlienDWaveIntervalTime );
}

// 描画関数.
void CSpawnUFO::Render()
{
	if( m_pStaticMesh == nullptr ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pStaticMesh->Render();
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
}

// 宇宙人をスポーンさせる.
void CSpawnUFO::SpawnAlien( std::vector<std::shared_ptr<CAlien>>& alienList )
{
	// 宇宙人リストのサイズが最大数より多ければ終了.
	if( (int)alienList.size() >= m_SpawnParameter.MaxAlienCount ) return;

	Update();	// 更新関数.

	if( m_SpawnCount != m_SpawnParameter.SpawnTime*FPS ) return;
//	std::shared_ptr<CAlien> tempAlien = AlienFactory();
//	if( tempAlien == nullptr ) return;
	if( m_pAbductUFOPosition == nullptr ) return;

	alienList.emplace_back( AlienFactory() );	// 宇宙人の追加.
	if( alienList.back() == nullptr ){
		alienList.pop_back();
		return;
	}
	// 宇宙人のパラメータリストが空なら終了.
	if( m_pAlienParamList->empty() == true ) return;
	if( m_pAlienParamList->size() <= static_cast<size_t>(m_AlienIndex) ) return;

	// リストにあったパラメータとスポーン座標を設定し、スポーンさせる.
	alienList.back()->Spawn( m_pAlienParamList->at(m_AlienIndex), m_SpawnParameter.Position );
	// 連れ去るUFOの座標を設定.
	alienList.back()->SetAbductUFOPosition( m_pAbductUFOPosition );
//	m_SpawnCount = 0;
}

// 宇宙人のパラメータリストを設定する.
void CSpawnUFO::SetAlienParameterList( std::vector<CAlien::SAlienParam>* alienParamList )
{
	m_pAlienParamList = alienParamList;
}

// スポーンパラメータの設定.
void CSpawnUFO::SetSpawnParameter( const SSpawnUFOParam& param )
{ 
	m_SpawnParameter = param;
	m_vPosition = m_SpawnParameter.Position;
	m_vPosition.y = POS_HEIGHT;
}

// ウェーブカウントの更新.
void CSpawnUFO::WaveCountUpdate( 
	int& waveCount, const int& waveTime, 
	int& intervalcount, const int& intervalTime )
{
	if( waveCount < waveTime*FPS ) return;
	intervalcount++;	// 間隔カウントの加算.

	if( intervalcount < intervalTime*FPS ) return;
	intervalcount = 0;	// 初期化.
	waveCount = 0;		// 初期化.
}

// 宇宙人の作成.
std::shared_ptr<CAlien> CSpawnUFO::AlienFactory()
{
	// 宇宙人番号の作成.
	const EAlienList alienNo = EAlienList::D;//static_cast<EAlienList>(GetAlienNo());

	switch( alienNo )
	{
	case EAlienList::A:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienA>();

	case EAlienList::Ada:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienA>();

	case EAlienList::B:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienB>();

	case EAlienList::Bda:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienB>();

	case EAlienList::C:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienC>();

	case EAlienList::D:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienD>();
	default:
		break;
	}
	return nullptr;
}

// 宇宙人番号の作成.
int CSpawnUFO::GetAlienNo()
{
	if( m_AlienCWaveCount >= m_SpawnParameter.AlienCWaveTime*FPS &&
		m_AlienDWaveCount >= m_SpawnParameter.AlienDWaveTime*FPS ){
		// 宇宙人C、Dをスポーンを含める.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// 最小値.
			static_cast<int>(EAlienList::Last),		// 最大値.
			static_cast<int>(EAlienList::Max) );	// 除外する値.
	} else if( m_AlienCWaveCount >= m_SpawnParameter.AlienCWaveTime*FPS ){
		// 宇宙人Cを含める.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// 最小値.
			static_cast<int>(EAlienList::C),		// 最大値.
			static_cast<int>(EAlienList::D) );		// 除外する値.
	} else if( m_AlienDWaveCount >= m_SpawnParameter.AlienDWaveTime*FPS ){
		// 宇宙人Dを含める.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// 最小値.
			static_cast<int>(EAlienList::D),		// 最大値.
			static_cast<int>(EAlienList::C) );		// 除外する値.
	} else {
		// 宇宙人C、Dを含めない.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// 最小値.
			static_cast<int>(EAlienList::Bda),		// 最大値.
			static_cast<int>(EAlienList::C) );		// 除外する値.
	}
}

int CSpawnUFO::CreateAlienNo( const int& min, const int& max, const int& outVal )
{
	// ランダム値の最大と最小の値を設定.
	std::uniform_int_distribution<int> alienNoMaxMin( min, max );
	int outAlienNo = alienNoMaxMin( m_RandomSeed );
	if( outVal == outAlienNo ){
		// 除外する値になった場合、再度作り直す.
		outAlienNo = CreateAlienNo( min, max, outVal );
	}
	return outAlienNo;
}

// モデルの取得.
bool CSpawnUFO::GetModel()
{
	if( m_pStaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME );
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}