#ifndef EACH_ITEM_EFFECT_STRUCT_H
#define EACH_ITEM_EFFECT_STRUCT_H

// 各アイテムの効果構造体.
struct stEachItemEffect
{
	// 回復アイテム.
	float LifeRecoveryValue;	// 回復力.

	// 特殊能力回復速度UPアイテム.
	float SPRecoveryValue;	// 特殊能力回復力.
	float SPRecoveryTime;	// 特殊能力回復時間.

	// 攻撃力UPアイテム.
	float AttackPower;	// 攻撃力.
	float AttackUpTime;	// 攻撃力UP時間.

	// 移動速度UPアイテム.
	float MovePower;	// 移動速度.
	float MoveUpTime;	// 移動速度UP時間.

	stEachItemEffect()
		: LifeRecoveryValue	( 0.0f )
		, SPRecoveryValue	( 0.0f )
		, SPRecoveryTime	( 0.0f )
		, AttackPower		( 0.0f )
		, AttackUpTime		( 0.0f )
		, MovePower			( 0.0f )
		, MoveUpTime		( 0.0f )
	{}
} typedef SEachItemEffect;

#endif	// #ifndef EACH_ITEM_EFFECT_STRUCT_H.