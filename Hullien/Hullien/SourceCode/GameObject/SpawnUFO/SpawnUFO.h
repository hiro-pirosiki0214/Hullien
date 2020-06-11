#ifndef SPAWN_UFO_H
#define SPAWN_UFO_H

#include "..\GameObject.h"
#include "..\Actor\Character\Alien\Alien.h"

#include "SpawnUFOParam.h"

#include <vector>

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

private:
	// 更新関数.外部で使用しないので隠蔽.
	virtual void Update() override;

	// 敵の作成.
	std::shared_ptr<CAlien> AlienFactory();

private:
	SSpawnUFOParam m_SpawnParameter;	// スポーンパラメータ.
	std::vector<CAlien::SAlienParam>* m_pAlienParamList;	// 宇宙人パラメータリスト.
	int	m_FrameCount;	// 経過フレームカウント.
	int m_AlienIndex;	// 宇宙人.
};

#endif	// #ifndef SPAWN_UFO_H.