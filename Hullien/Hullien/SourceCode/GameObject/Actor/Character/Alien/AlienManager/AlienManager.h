#ifndef ALIEN_MANAGER_H
#define ALIEN_MANAGER_H

#include "..\Alien.h"
#include "..\..\..\Explosion\Explosion.h"

#include <vector>
#include <memory>
#include <functional>

class CMotherShipUFO;	// マザーシップUFO.
class CSpawnUFO;		// スポーンUFO.
class CExplosion;		// 爆発クラス.

class CAlienManager
{
	const char* SPAWN_PARAM_FILE_PATH = "Data\\GameParam\\Alien\\SpaenParam\\SpaenParam.bin";
	const char* EXPLOSION_PARAM_FILE_PATH = "Data\\GameParam\\Alien\\Explosion\\Explosion.bin";
	const char* ALIEN_PARAM_LIST_FILE_PATH = "Data\\GameParam\\Alien\\AlienParamPathList.txt";
public:
	CAlienManager();
	~CAlienManager();

	// 初期化関数.
	bool Init();
	// 更新関数.
	void Update( std::function<void(CActor*)> updateProc );
	// 描画関数.
	void Render();
	// エフェクト描画関数.
	void EffectRender();
	// スプライト描画関数.
	void SpriteRender();
	// マザーシップの座標を設定.
	void SetMotherShipUFOPos( const D3DXVECTOR3& vPos ){ m_MotherShipUFOPos = vPos; }
	// 落とすアイテムリストの取得.
	DropItemList GetDropItemList(){ return m_DropItemList; }
	// 女の子を連れ去っているか.
	bool IsGirlAbduct();
	// アニメーションを止める.
	void StopAnimation();

private:
	// スポーン.
	void Spawn();

	// 爆発できるかどうか確認.
	void ExplosionConfirming( CAlien* ailen );
	// 落とすアイテムの設定.
	void SetDropItemList( CAlien* ailen );
	// スポーンUFOの初期化.
	bool SpawnUFOInit();
	// 宇宙人パラメータリストの読み込み.
	bool ReadAlienParamList();
	// 爆発パラメータの読み込み.
	bool ReadExplosionParam();

private:
	std::vector<std::shared_ptr<CAlien>>	m_AilenList;			// 宇宙人リスト.
	std::vector<std::shared_ptr<CSpawnUFO>>	m_SpawnUFOList;			// スポーンUFOリスト.
	std::vector<SAlienParam>				m_AlienParamList;		// 宇宙人パラメータリスト.
	std::vector<CExplosion>					m_ExplosionList;		// 爆発リスト.
	CExplosion::SExplosionParam				m_ExplosionParam;		// 爆発パラメーター.
	DropItemList							m_DropItemList;			// 落とすアイテムのリスト.
	D3DXVECTOR3								m_MotherShipUFOPos;		// 連れ去るUFOの座標.
	bool									m_IsAlienAbduct;		// 宇宙人が連れ去っているかどうか.
	int										m_SortCount;			// ソートする時のカウント.
	bool									m_IsRisingMotherShip;	// マザーシップに昇っているか.
};

#endif	// #ifdef ALIEN_MANAGER_H.