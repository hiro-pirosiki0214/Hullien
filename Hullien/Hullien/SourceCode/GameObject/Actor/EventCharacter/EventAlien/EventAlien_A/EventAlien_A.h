#ifndef EVENTALIEN_A_H
#define EVENTALIEN_A_H

#include "..\EventAlien.h"

/*************************************
*	イベント用宇宙人A.
**/
class CEventAlienA : public CEventAlien
{
	const char* MODEL_NAME = "GhostB_s";
	const char* MODEL_TEMP_NAME = "a";	// 仮モデル名.

public:
	CEventAlienA();
	virtual ~CEventAlienA();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision(CActor* pActor) override;
	// スポーン.
	virtual bool Spawn(const D3DXVECTOR3& spawnPos) override;
	// 宇宙人の状態取得関数.
	CEventCharacter::SCharacterParam GetParameter() const { return m_Parameter; }

private:
	// スポーン.
	virtual void Spawning() override;
	// 移動.
	virtual void Move() override;
	// 拐う.
	virtual void Abduct() override;
	// 逃げる.
	virtual void Escape() override;
	// 吹き飛ぶ.
	virtual void BlowAway() override;

	// 当たり判定の設定.
	bool ColliderSetting();

};

#endif	//#ifndef EVENTALIEN_A_H.