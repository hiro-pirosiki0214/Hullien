#ifndef SP_EFFECT_TIME_H
#define SP_EFFECT_TIME_H

#include "..\..\ItemBase.h"

class CSPEffectTimeItem : public CItemBase
{
	const char* MODEL_NAME = "item ability";
	const float* pSP_RECOVERY_VALUE;	// 特殊能力回復力(外部からポインタで取得する).
	const float* pSP_RECOVERY_TIME;		// 特殊能力回復時間(外部からポインタで取得する).
public:
	CSPEffectTimeItem();
	CSPEffectTimeItem(
		const float* pSPRecoveryValue,
		const float* pSPRecoveryTime,
		const CItemBase::SParameter* pParam );
	virtual ~CSPEffectTimeItem();

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

#endif	// #ifndef SP_EFFECT_TIME_H.