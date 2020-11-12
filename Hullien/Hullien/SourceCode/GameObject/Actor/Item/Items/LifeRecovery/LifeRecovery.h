#ifndef LIFE_RECOVERY_H
#define LIFE_RECOVERY_H

#include "..\..\ItemBase.h"

class CLifeRecoveryItem : public CItemBase
{
	const char* MODEL_NAME	= "item cure";	// モデル名.
	const char* DROP_AND_AVTIVE_EFFECT_NAME	= "item_effect_1";	// エフェクト名.
	const char* HIT_EFFECT_NAME				= "key";	// エフェクト名.
	const float* pLIFE_RECOVERY_VALUE;	// 回復力(外部からポインタで取得する).
public:
	CLifeRecoveryItem();
	CLifeRecoveryItem( 
		const float* pLifeRecoveryValue, 
		const CItemBase::SParameter* pParam );
	virtual ~CLifeRecoveryItem();

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
	// エフェクトの設定.
	virtual bool EffectSetting() override;
};

#endif	// #ifndef LIFE_RECOVERY_H.