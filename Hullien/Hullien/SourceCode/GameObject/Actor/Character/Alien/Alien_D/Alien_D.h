#ifndef ALIEN_D_H
#define ALIEN_D_H

#include "..\Alien.h"

class CLaserBeam;	// レーザービームクラス.

class CAlienD : public CAlien
{
	const char* MODEL_NAME = "GhostB_s";
	const char* SPRITE_NAME = "AttackRange";

	const D3DXVECTOR4 ATTACK_RANGE_COLOR		= { 0.8f, 0.0f, 0.0f, 0.0f };	// 赤色.
	const D3DXVECTOR4 ATTACK_RANGE_DANGER_COLOR	= { 0.8f, 0.8f, 0.0f, 1.0f };	// 黄色.
	const float ATTACK_TIME = 1.0f;
	const float ATTACK_RANGE_SPRITE_POS_Y = 0.002f;
	const float ATTACK_RANGE_SPRITE_SCALE = 10.0f;
	const float ATTACK_RANGE_ADD_VALUE = 0.005f;
	const float ATTACK_RANGE_SUB_VALUE = -0.01f;

	const float CONTROL_POINT_ONE_LENGHT	= -1.5f;
	const float CONTROL_POINT_ONE_Y_LENGHT	= 10.0f;
	const float CONTROL_POINT_TWO_LENGHT	= 1.5f;
	const float CONTROL_POINT_TWO_Y_LENGHT	= 7.0f;

public:
	CAlienD();
	virtual ~CAlienD();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// スポーン.
	virtual bool Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos ) override;

private:
	// モデルの描画.
	void ModelRender();
	// 攻撃範囲のスプライト描画.
	void AttackRangeSpriteRender();
	// スポーン.
	virtual void Spawning() override;
	// 移動.
	virtual void Move() override;
	// 拐う.
	virtual void Abduct() override;
	// 怯み.
	virtual void Fright() override;
	// 死亡.
	virtual void Death() override;
	// 逃げる.
	virtual void Escape() override;
	// 攻撃関数.
	void Attack();
	// 移動関数.
	virtual void VectorMove( const float& moveSpeed ) override;
	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor ) override;

	// 攻撃の当たり判定.
	void AttackCollision( CActor* pActor  );

	// スプライトの取得.
	bool GetSprite( const char* spriteName );

	// 当たり判定の設定.
	bool ColliderSetting();

private:
	std::shared_ptr<CSprite>	m_pAttackRangeSprite;	// 攻撃範囲のスプライト.
	std::unique_ptr<CLaserBeam>	m_pLaserBeam;			// レーザービームクラス.
	std::vector<D3DXVECTOR3>	m_ControlPositions;		// ベジェ曲線で使用する操作座標.
	float m_AttackCount;	// 攻撃カウント.
	bool m_IsAttackStart;	// 攻撃開始したか.
};

#endif	// #ifndef ALIEN_D_H.