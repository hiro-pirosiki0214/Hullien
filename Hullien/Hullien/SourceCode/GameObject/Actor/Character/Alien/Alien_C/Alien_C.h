#ifndef ALIEN_C_H
#define ALIEN_C_H

#include "..\Alien.h"

class CAlienC : public CAlien
{
	const char* MODEL_NAME = "c_s";	// モデル名.

public:
	CAlienC();
	virtual ~CAlienC();

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

	// 当たり判定の設定.
	bool ColliderSetting();
};

#endif	// #ifndef ALIEN_C_H.