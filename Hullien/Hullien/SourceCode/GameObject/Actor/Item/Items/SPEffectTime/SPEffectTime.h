#ifndef SP_EFFECT_TIME_H
#define SP_EFFECT_TIME_H

#include "..\..\ItemBase.h"

class CSPEffectTimeItem : public CItemBase
{
	const char* MODEL_NAME	= "item ability";			// モデル名.
	const char* DROP_AND_AVTIVE_EFFECT_NAME	= "item_effect_1";	// ドロップ アクティブ時のエフェクト名.
	const char* HIT_EFFECT_NAME				= "key";	// ヒット時のエフェクト名.
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
	// エフェクトの描画.
	virtual void EffectRender() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& pActor ) override;

private:
	// 当たった際の効果を与える.
	virtual void GiveEffect( CActor* pActor ) override;
	// エフェクトの設定.
	virtual bool EffectSetting() override;
};

#endif	// #ifndef SP_EFFECT_TIME_H.