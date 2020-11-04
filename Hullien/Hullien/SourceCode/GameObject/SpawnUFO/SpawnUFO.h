#ifndef SPAWN_UFO_H
#define SPAWN_UFO_H

#include "..\GameObject.h"
#include "..\Actor\Character\Alien\AlienList.h"
#include "..\Actor\Character\Alien\Alien.h"

#include "SpawnUFOParam.h"

#include <vector>
#include <random>

class CActor;
class CUFOLight;
class CCollisionManager;	// 当たり判定クラス.

class CSpawnUFO : public CGameObject
{
	const char* MODEL_NAME = "ufo_mini";	// モデル名.
	const float COLLISION_RADIUS = 5.0f;
	const float ADD_POS_POWER = 0.1f;
public:
	CSpawnUFO();
	virtual ~CSpawnUFO();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// 宇宙人をスポーンさせる.
	void SpawnAlien( std::vector<std::shared_ptr<CAlien>>& );
	// 宇宙人のパラメータリストを設定する.
	void SetAlienParameterList( std::vector<SAlienParam>* );
	// スポーンパラメータの設定.
	void SetSpawnParameter( const SSpawnUFOParam& param );
	// 連れ去るUFOの座標設定.
	void SetAbductUFOPosition( D3DXVECTOR3* pPos ){ m_pAbductUFOPosition = pPos; }

	// ライトを取り出す.
	void LightDischarge();
	// ライトをしまう.
	void LightCleanUP();
	// 光を完全に放出する.
	void DischargePreparation();
	// 光を片づける.
	void CleanUPPreparation();

	// 描画フラグ設定関数.
	void SetDisp(const bool& disp) { m_IsDisp = disp; }

	// スケール値の設定.
	void SetScale(const D3DXVECTOR3& scale) { m_vScale = scale; }
	// 当たり判定(イベントで使用).
	D3DXVECTOR3 Collision(CActor* pActor);

private:
	// 宇宙人の作成.
	std::shared_ptr<CAlien> AlienFactory();
	// 宇宙人番号の取得.
	int GetAlienNo();
	// 宇宙人番号の作成.
	int CreateAlienNo( const int& min, const int& max, const int& outVal );
	// アイテムを取得する.
	EItemList ProbabilityGetItem( const bool& isAlienD );

	// モデルの取得.
	bool GetModel();
	// 当たり判定の設定.
	bool CollisionSetting();

private:
	std::shared_ptr<CDX9StaticMesh>		m_pStaticMesh;			// メッシュ.
	std::shared_ptr<CUFOLight>			m_pUFOLight;			// UFOライト.
	std::vector<std::shared_ptr<CAlien>>m_AilenList;			// 宇宙人リスト.
	std::shared_ptr<CCollisionManager>	m_pCollManager;			// 当たり判定クラス.
	SSpawnUFOParam						m_SpawnParameter;		// スポーンパラメータ.
	D3DXVECTOR3							m_SpawnPoint;			// スポーンポイント.
	D3DXVECTOR3*						m_pAbductUFOPosition;	// 連れ去るUFOの座標.
	std::vector<SAlienParam>*			m_pAlienParamList;		// 宇宙人パラメータリスト.
	int	m_FrameCount;	// 経過フレームカウント.
	int m_SpawnCount;	// スポーンカウント.
	int m_AlienIndex;	// 宇宙人.
	int	m_AlienSpawnCount;
	bool m_IsDisp;		//描画フラグ.

	std::mt19937 m_RandomSeed;	// ランダムシード.
};

#endif	// #ifndef SPAWN_UFO_H.