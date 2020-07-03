#ifndef ALIEN_B_H
#define ALIEN_B_H

#include "..\Alien.h"

class CAlienB : public CAlien
{
	const char* MODEL_NAME = "GhostB_s";

public:
	CAlienB();
	virtual ~CAlienB();

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
	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor ) override;

private:
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

	// 移動関数.
	virtual void VectorMove( const float& moveSpeed ) override;
	// 攻撃関数.
	void Attack();
	// プレイヤーとの当たり判定.
	void PlayerCollison( CActor* pActor );

	// プレイヤーを狙うか判定.
	void AimPlayerDecision();

	// 当たり判定の設定.
	bool ColliderSetting();

private:
	D3DXVECTOR3 m_vPlayerPos;	// プレイヤーの座標.
	bool	m_HasAimPlayer;		// プレイヤーを狙うかどうか.
	bool	m_OldHasAimPlayer;	// 前回プレイヤーを狙っていたか.
	float	m_RotAccValue;		// 回転加速値.
};

#endif	// #ifndef ALIEN_B_H.