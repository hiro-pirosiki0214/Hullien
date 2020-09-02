#ifndef ATTACK_UP_H
#define ATTACK_UP_H

#include "..\..\ItemBase.h"

class CAttackUpItem : public CItemBase
{
	const char* MODEL_NAME = "item attack";
	const float* pATTACK_UP_POWER;	// 攻撃力(外部からポインタで取得する).
	const float* pATTACK_UP_TIME;	// 攻撃力(外部からポインタで取得する).

public:
	CAttackUpItem();
	CAttackUpItem(
		const float* pAttackUpPower,
		const float* pAttackUpTime,
		const CItemBase::SParameter* pParam );
	virtual ~CAttackUpItem();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& pActor ) override;

private:
	// 当たった際の効果を与える.
	virtual void GiveEffect( CActor* pActor ) override;
};

#endif	// #ifndef ATTACK_UP_H.