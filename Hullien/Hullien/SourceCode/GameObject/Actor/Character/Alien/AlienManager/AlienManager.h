#ifndef ALIEN_MANAGER_H
#define ALIEN_MANAGER_H

#include "..\Alien.h"

#include <vector>
#include <memory>
#include <functional>

class CSpawnUFO;

class CAlienManager
{
	const char* SPAWN_PARAM_FILE_PATH = "Data\\GameParam\\Alien\\SpaenParam\\SpaenParam.bin";
	const char* ALIEN_PARAM_LIST_FILE_PATH = "Data\\GameParam\\Alien\\AlienParamPathList.txt";
public:
	CAlienManager();
	~CAlienManager();

	// 初期化関数.
	bool Init();
	// 更新関数.
	void Update( CActor* pActor, std::function<void(CActor*)> collProc );
	// 描画関数.
	void Render();

private:
	// スポーン.
	void Spawn();

	// スポーンUFOの初期化.
	bool SpawnUFOInit();
	// 宇宙人パラメータリストの読み込み.
	bool ReadAlienParamList();
	// 宇宙人のモデルの値でのソート関数.
	void ModelAlphaSort();

	// デバッグ用の描画関数.
	void DebugRender();

private:
	std::vector<std::shared_ptr<CAlien>> m_AilenList;	// 宇宙人リスト.
	std::vector<CSpawnUFO> m_SpawnUFOList;				// スポーンUFOリスト.
	std::vector<CAlien::SAlienParam> m_AlienParamList;	// 宇宙人パラメータリスト.
	D3DXVECTOR3	m_AbductUFOPosition;	// 連れ去るUFOの座標.
	bool		m_IsAlienAbduct;		// 宇宙人が連れ去っているかどうか.
	int			m_SortCount;			// ソートする時のカウント.
};

#endif	// #ifdef ALIEN_MANAGER_H.