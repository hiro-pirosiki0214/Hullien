#ifndef SPAWN_UFO_H
#define SPAWN_UFO_H

#include "..\GameObject.h"
#include "..\Actor\Character\Alien\AlienList.h"
#include "..\Actor\Character\Alien\Alien.h"

#include "SpawnUFOParam.h"

#include <vector>
#include <random>

class CSpawnUFO : public CGameObject
{
public:
	CSpawnUFO();
	virtual ~CSpawnUFO();

	// 初期化関数.
	virtual bool Init() override;
	// 描画関数.
	virtual void Render() override;

	// 宇宙人をスポーンさせる.
	void SpawnAlien( std::vector<std::shared_ptr<CAlien>>& );
	// 宇宙人のパラメータリストを設定する.
	void SetAlienParameterList( std::vector<CAlien::SAlienParam>* );
	// スポーンパラメータの設定.
	void SetSpawnParameter( const SSpawnUFOParam& param ){ m_SpawnParameter = param; }
	// 連れ去るUFOの座標設定.
	void SetAbductUFOPosition( D3DXVECTOR3* pPos ){ m_pAbductUFOPosition = pPos; }

private:
	// 更新関数.外部で使用しないので隠蔽.
	virtual void Update() override;
	// ウェーブカウントの更新.
	void WaveCountUpdate(
		int& waveCount, const int& waveTime, 
		int& intervalcount, const int& intervalTime );

	// 宇宙人の作成.
	std::shared_ptr<CAlien> AlienFactory();
	// 宇宙人番号の取得.
	int GetAlienNo();
	// 宇宙人番号の作成.
	int CreateAlienNo( const int& min, const int& max, const int& outVal );

private:
	SSpawnUFOParam	m_SpawnParameter;		// スポーンパラメータ.
	D3DXVECTOR3*	m_pAbductUFOPosition;	// 連れ去るUFOの座標.
	std::vector<CAlien::SAlienParam>* m_pAlienParamList;	// 宇宙人パラメータリスト.
	int	m_FrameCount;	// 経過フレームカウント.
	int m_SpawnCount;	// スポーンカウント.
	int m_AlienIndex;	// 宇宙人.

	int m_AlienCWaveCount;			// 宇宙人Cのウェーブカウント.
	int m_AlienCWaveIntervalCount;	// 宇宙人Cのウェーブ間隔カウント.
	int m_AlienDWaveCount;			// 宇宙人Dのウェーブカウント.
	int m_AlienDWaveIntervalCount;	// 宇宙人Dのウェーブ間隔カウント,

	std::mt19937 m_RandomSeed;	// ランダムシード.
};

#endif	// #ifndef SPAWN_UFO_H.