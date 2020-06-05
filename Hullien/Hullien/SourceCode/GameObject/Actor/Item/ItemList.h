#ifndef ITEM_H
#define ITEM_H

// アイテムリスト.
enum enItemList
{
	None,

	LifeRecovery,	// 体力回復.
	SPEffectTime,	// 特殊能力回復速度UP.
	AttackUp,		// 攻撃力UP.
	MoveSpeedUp,	// 移動速度UP.

	Max,
} typedef EItemList;

#endif	// #ifndef ITEM_H.