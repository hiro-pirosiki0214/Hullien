#include "AlienManager.h"
#include "..\..\..\..\MotherShipUFO\MotherShipUFO.h"
#include "..\..\..\..\SpawnUFO\SpawnUFO.h"
#include "..\AlienList.h"
#include "..\..\..\Explosion\Explosion.h"

#include "..\..\..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

#include <algorithm>

CAlienManager::CAlienManager()
	: m_AilenList			()
	, m_SpawnUFOList		()
	, m_AlienParamList		()
	, m_DropItemList		()
	, m_MotherShipUFOPos	( 0.0f, 0.0f, 0.0f )
	, m_IsAlienAbduct		( false )
	, m_SortCount			( 0 )
	, m_IsRisingMotherShip	( false )
{
}

CAlienManager::~CAlienManager()
{
}

// 初期化関数.
bool CAlienManager::Init()
{
	
	if( SpawnUFOInit() == false )			return false;
	if( ReadAlienParamList() == false )		return false;
	if( ReadExplosionParam() == false )		return false;

	return true;
}

// 更新関数.
void CAlienManager::Update( std::function<void(CActor*)> updateProc )
{
	Spawn();	// スポーン.
	bool isAbduct = false;
	m_DropItemList.clear();

	// 宇宙人達の更新.
	for( size_t i = 0; i < m_AilenList.size(); i++ ){
		m_AilenList[i]->SetOtherAbduct( &m_IsAlienAbduct );

		// 更新関数.
		updateProc( m_AilenList[i].get() );

		// 女の子を連れ去っているか確認.
		if( isAbduct == false ){
			if( m_AilenList[i]->IsAbduct() == true ){
				isAbduct = m_IsAlienAbduct = true;
			}
		}

		// 爆発できるか確認.
		ExplosionConfirming( m_AilenList[i].get() );
		// 爆発と宇宙人の当たり判定.
		for( auto& e : m_ExplosionList ){
			if( e.IsStop() == true ) continue;
			e.Collision( m_AilenList[i].get() );
		}

		// マザーシップに昇っているか確認.
		if( m_AilenList[i]->IsRisingMotherShip() == true ){
			m_IsRisingMotherShip = true;
		}

		// リストから指定の宇宙人を消すか.
		if( m_AilenList[i]->IsDelete() == false ) continue;

		// アイテムリストの設定.
		SetDropItemList( m_AilenList[i].get() );

		m_AilenList[i] = m_AilenList.back();
		m_AilenList.pop_back();
		i--;
	}
	if( isAbduct == false ) m_IsAlienAbduct = false;
}

// 描画関数.
void CAlienManager::Render()
{
	// 宇宙人達の描画.
	for( auto& a : m_AilenList ) a->Render();
	// スポーンUFOの描画.
	for( auto& s : m_SpawnUFOList ) s.Render();
}

// エフェクト描画関数.
void CAlienManager::EffectRender()
{
	for( auto& a : m_AilenList ) a->EffectRender();
	// 爆発の描画.
	for( auto& e : m_ExplosionList ) e.Render();
}

// スプライト描画関数.
void CAlienManager::SpriteRender()
{
	// 宇宙人達の描画.
	for( auto& a : m_AilenList ) a->SpriteRender();
}

// 女の子を連れ去っているか.
bool CAlienManager::IsGirlAbduct()
{
	return m_IsRisingMotherShip;
}

// アニメーションを止める.
void CAlienManager::StopAnimation()
{
	for( auto& a : m_AilenList ) a->StopAnimation();
}

// スポーン.
void CAlienManager::Spawn()
{
	for( auto& s : m_SpawnUFOList ){
		s.SpawnAlien( m_AilenList );
	}
}

// 爆発できるかどうか確認.
void CAlienManager::ExplosionConfirming( CAlien* ailen )
{
	// 宇宙人Cじゃなければ終了.
	if( ailen->GetObjectTag() != EObjectTag::Alien_C ) return;
	// 爆発しなければ終了.
	if( ailen->IsExplosion() == false ) return;
	// 爆発と宇宙人の当たり判定.
	for( auto& e : m_ExplosionList ){
		if( e.IsStop() == false ) continue;
		// 爆発が終了していれば使いまわす.
		e.Init();
		e.SetTargetPos( *ailen );
		return;
	}
	// 終了した爆発がなければ.
	// 爆発を追加.
	m_ExplosionList.emplace_back();
	m_ExplosionList.back().Init();
	m_ExplosionList.back().SetExplosionParam( m_ExplosionParam );
	m_ExplosionList.back().SetTargetPos( *ailen );
}

// 落とすアイテムの設定.
void CAlienManager::SetDropItemList( CAlien* ailen )
{
	if( ailen->GetAnyItem() == EItemList::None ) return;
	if( ailen->GetAnyItem() == EItemList::Max ) return;

	// 落とすアイテムの設定.
	D3DXVECTOR3 dropPos = ailen->GetPosition();
	dropPos.y += 4.0f;	// 高めに調整.
	m_DropItemList[ailen->GetAnyItem()] = dropPos;
}

// スポーンUFOの初期化.
bool CAlienManager::SpawnUFOInit()
{
	std::vector<SSpawnUFOParam> spawnPramList;
	if( CFileManager::BinaryVectorReading( SPAWN_PARAM_FILE_PATH, spawnPramList ) == false ) return false;
	for( const auto& s : spawnPramList ){
		m_SpawnUFOList.emplace_back();
		m_SpawnUFOList.back().Init();
		m_SpawnUFOList.back().SetSpawnParameter( s );
		m_SpawnUFOList.back().SetAbductUFOPosition( &m_MotherShipUFOPos );
	}
	return true;
}

// 宇宙人パラメータリストの読み込み.
bool CAlienManager::ReadAlienParamList()
{
	// 各宇宙人のパラメータパスリストの取得.
	std::vector<std::string> readList = 
		CFileManager::TextLoading( ALIEN_PARAM_LIST_FILE_PATH );

	// テキストが読み込めてなかったら終了.
	if( readList.empty() == true ) return false;

	for( const auto& l : readList ){
		// 各宇宙人のパラメータ読み込み.
		CAlien::SAlienParam param = {};
		CFileManager::BinaryReading( l.c_str(), param );
		m_AlienParamList.emplace_back( param );
	}
	// スポーンUFOに各宇宙人のパラメータ設定.
	for( auto& s : m_SpawnUFOList ){
		s.SetAlienParameterList( &m_AlienParamList );
		s.LightDischarge();
	}

	return true;
}

// 爆発パラメータの読み込み.
bool CAlienManager::ReadExplosionParam()
{
	if( CFileManager::BinaryReading( EXPLOSION_PARAM_FILE_PATH, m_ExplosionParam ) == false ) return false;
	return true;
}