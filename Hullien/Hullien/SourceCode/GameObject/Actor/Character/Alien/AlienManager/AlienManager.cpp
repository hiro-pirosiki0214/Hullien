#include "AlienManager.h"
#include "..\..\..\..\SpawnUFO\SpawnUFO.h"
#include "..\AlienList.h"
#include "..\..\..\Explosion\Explosion.h"

#include "..\..\..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

#include <algorithm>

CAlienManager::CAlienManager()
	: m_AilenList			()
	, m_SpawnUFOList		()
	, m_AlienParamList		()
	, m_AbductUFOPosition	( 10.0f, 0.0f, 0.0f )
	, m_IsAlienAbduct		( false )
	, m_SortCount			( 0 )
{
}

CAlienManager::~CAlienManager()
{
}

// 初期化関数.
bool CAlienManager::Init()
{
	if( SpawnUFOInit() == false ) return false;
	if( ReadAlienParamList() == false ) return false;
	return true;
}

// 更新関数.
void CAlienManager::Update( CActor* pPlayer, CActor* pGirl, std::function<void(CActor*)> collProc )
{
	Spawn();	// スポーン.
	bool isAbduct = false;
	// 宇宙人達の更新.
	for( size_t i = 0; i < m_AilenList.size(); i++ ){
		m_AilenList[i]->SetOtherAbduct( &m_IsAlienAbduct );

		// 座標の設定.
		m_AilenList[i]->SetTargetPos( *pPlayer );
		m_AilenList[i]->SetTargetPos( *pGirl );
		// 更新処理.
		m_AilenList[i]->Update();
		// プレイヤー、女の子の当たり判定.
		collProc( m_AilenList[i].get() );
		// 宇宙人の当たり判定.
		m_AilenList[i]->Collision( pPlayer );
		m_AilenList[i]->Collision( pGirl );

		// 女の子を連れ去っているか確認.
		if( isAbduct == false ){
			if( m_AilenList[i]->IsAbduct() == true ){
				isAbduct = m_IsAlienAbduct = true;
			}
		}

		// 爆発できるか確認.
		ExplosionConfirming( m_AilenList[i] );
		// 爆発と宇宙人の当たり判定.
		for( auto& e : m_ExplosionList ) e.Collision( m_AilenList[i].get() );

		// リストから指定の宇宙人を消すか.
		if( m_AilenList[i]->IsDelete() == false ) continue;
		m_AilenList[i] = m_AilenList.back();
		m_AilenList.pop_back();
		i--;
	}
	if( isAbduct == false ) m_IsAlienAbduct = false;
	// モデルのアルファ値でのソート.
	ModelAlphaSort();
}

// 描画関数.
void CAlienManager::Render()
{
	// 宇宙人達の描画.
	for( auto& a : m_AilenList ) a->Render();
	// スポーンUFOの描画.
	for( auto& s : m_SpawnUFOList ) s.Render();
	// 爆発の描画.
	for( auto& e : m_ExplosionList ) e.Render();
#if _DEBUG
	DebugRender();
#endif	// #if _DEBUG.
}

// スポーン.
void CAlienManager::Spawn()
{
	for( auto& s : m_SpawnUFOList ){
		s.SpawnAlien( m_AilenList );
	}
}

// 爆発できるかどうか確認.
void CAlienManager::ExplosionConfirming( const std::shared_ptr<CAlien>& ailen  )
{
	// 宇宙人Cじゃなければ終了.
	if( ailen->GetObjectTag() != EObjectTag::Alien_C ) return;
	// 爆発しなければ終了.
	if( ailen->IsExplosion() == false ) return;

	// 爆発を追加.
	m_ExplosionList.emplace_back();
	m_ExplosionList.back().Init();
}

// スポーンUFOの初期化.
bool CAlienManager::SpawnUFOInit()
{
	std::vector<SSpawnUFOParam> spawnPramList;
	if( CFileManager::BinaryVectorReading( SPAWN_PARAM_FILE_PATH, spawnPramList ) == false ) return false;
	for( const auto& s : spawnPramList ){
		m_SpawnUFOList.emplace_back();
		m_SpawnUFOList.back().SetSpawnParameter( s );
		m_SpawnUFOList.back().SetAbductUFOPosition( &m_AbductUFOPosition );
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
	}

	return true;
}

// 宇宙人のモデルの値でのソート関数.
void CAlienManager::ModelAlphaSort()
{
	m_SortCount++;
	if( m_SortCount < FPS ) return;

	auto comp = []( auto& a, auto b )
	{ return a->GetModelAplha() > b->GetModelAplha();};

	std::sort( m_AilenList.begin(), m_AilenList.end(), comp );
	m_SortCount = 0;
}

// デバッグ用の描画関数.
void CAlienManager::DebugRender()
{
	const float pos_y = 40.0f;
	const float pos_x = 880.0f;
	CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*-1, 0.0f } );
	CDebugText::Render( "- Alien Parameter -" );

	CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*1, 0.0f } );
	CDebugText::Render( "AlienCount : ", m_AilenList.size() );
	CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*2, 0.0f } );
	CDebugText::Render( "SpawnCount : ", m_SpawnUFOList.size() );

	auto alienA_ADashPramRender = [&]()
	{
		size_t a_index = static_cast<size_t>(EAlienList::A);
		if( m_AlienParamList.size() < a_index ) return;
		CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*4, 0.0f } );
		CDebugText::Render( "-- Alien A Param --" );

		CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*5, 0.0f } );
		CDebugText::Render( "Life : ", m_AlienParamList[a_index-1].Life );
		CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*6, 0.0f } );
		CDebugText::Render( "MoveSpeed : ", m_AlienParamList[a_index-1].MoveSpeed );
		CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*7, 0.0f } );
		CDebugText::Render( "RotationalSpeed : ", m_AlienParamList[a_index-1].RotationalSpeed );
		CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*8, 0.0f } );
		CDebugText::Render( "ModelAlphaAddValue : ", m_AlienParamList[a_index-1].ModelAlphaAddValue );
	};
	alienA_ADashPramRender();
}