#include "ItemManager.h"
#include "..\Items\LifeRecovery\LifeRecovery.h"
#include "..\Items\SPEffectTime\SPEffectTime.h"
#include "..\Items\AttackUp\AttackUp.h"
#include "..\Items\MoveSpeedUp\MoveSpeedUp.h"

#include "..\..\..\..\Utility\FileManager\FileManager.h"

CItemManager::CItemManager()
	: m_ItemList		()
	, m_ItemParam		()
	, m_EachItemEffect	()
{
}

CItemManager::~CItemManager()
{
}

// 初期化.
bool CItemManager::Init()
{
	// 各パラメータの読み込み.
	if( CFileManager::BinaryReading( ITEM_PARAM_FILE_PATH, m_ItemParam ) == false ) return false;
	if( CFileManager::BinaryReading( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect ) == false ) return false;
	return true;
}

// ドロップ.
void CItemManager::Drop( const DropItemList& dropItemList )
{
	for( const auto& d : dropItemList ){
		switch( d.first )
		{
		default:
			continue;
		case EItemList::LifeRecovery:
			m_ItemList.emplace_back( std::make_shared<CLifeRecoveryItem>(
				// 回復力とパラメーターの設定.
				&m_EachItemEffect.LifeRecoveryValue,
				&m_ItemParam ) );
			break;
		case EItemList::SPEffectTime:
			m_ItemList.emplace_back( std::make_shared<CSPEffectTimeItem>(
				// 特殊能力回復力と特殊能力回復時間とパラメーターの設定.
				&m_EachItemEffect.SPRecoveryValue,
				&m_EachItemEffect.SPRecoveryTime,
				&m_ItemParam ) );
			break;
		case EItemList::AttackUp:
			m_ItemList.emplace_back( std::make_shared<CAttackUpItem>(
				// 攻撃力と攻撃力UP時間とパラメーターの設定.
				&m_EachItemEffect.AttackPower,
				&m_EachItemEffect.AttackUpTime,
				&m_ItemParam ) );
			break;
		case EItemList::MoveSpeedUp:
			m_ItemList.emplace_back( std::make_shared<CMoveSpeedUpItem>(
				// 移動速度と移動速度UP時間とパラメーターの設定.
				&m_EachItemEffect.MovePower,
				&m_EachItemEffect.MoveUpTime,
				&m_ItemParam ) );
			break;
		}
		// アイテムの座標を設定して、落とす.
		m_ItemList.back()->Drop( d.second );
	}
}

// 更新関数.
void CItemManager::Update( std::function<void(CActor*)> updateProc )
{
	// 各アイテムの更新、当たり判定.
	for( auto& i : m_ItemList ) updateProc( i.get() );
}

// 描画関数.
void CItemManager::Render()
{
	// 各アイテムの描画.
	for( auto& i : m_ItemList ) i->Render();
}