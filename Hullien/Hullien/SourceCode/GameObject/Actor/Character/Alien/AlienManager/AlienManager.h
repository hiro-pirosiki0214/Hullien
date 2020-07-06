#ifndef ALIEN_MANAGER_H
#define ALIEN_MANAGER_H

#include "..\Alien.h"
#include "..\..\..\Explosion\Explosion.h"

#include <vector>
#include <memory>
#include <functional>

class CSpawnUFO;
class CExplosion;

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
	void Update( CActor* pPlayer, CActor* pGirl, std::function<void(CActor*)> collProc );
	// 描画関数.
	void Render();

private:
	// スポーン.
	void Spawn();

	// 爆発できるかどうか確認.
	void ExplosionConfirming( const std::shared_ptr<CAlien>& ailen );

	// スポーンUFOの初期化.
	bool SpawnUFOInit();
	// 宇宙人パラメータリストの読み込み.
	bool ReadAlienParamList();
	// 爆発パラメータの読み込み.
	bool ReadExplosionParam();

	// 宇宙人のモデルの値でのソート関数.
	void ModelAlphaSort();

	// デバッグ用の描画関数.
	void DebugRender();

private:
	std::vector<std::shared_ptr<CAlien>>	m_AilenList;		// 宇宙人リスト.
	std::vector<CSpawnUFO>					m_SpawnUFOList;		// スポーンUFOリスト.
	std::vector<CAlien::SAlienParam>		m_AlienParamList;	// 宇宙人パラメータリスト.
	std::vector<CExplosion>		m_ExplosionList;	// 爆発リスト.
	CExplosion::SExplosionParam	m_ExplosionParam;	// 爆発パラメーター.
	D3DXVECTOR3	m_AbductUFOPosition;	// 連れ去るUFOの座標.
	bool		m_IsAlienAbduct;		// 宇宙人が連れ去っているかどうか.
	int			m_SortCount;			// ソートする時のカウント.
};

#endif	// #ifdef ALIEN_MANAGER_H.