#ifndef ITEM_H
#define ITEM_H

#include <unordered_map>
#include "..\..\..\Global.h"

// アイテムリスト.
enum  class enItemList
{
	None,

	LifeRecovery,	// 体力回復.
	SPEffectTime,	// 特殊能力回復速度UP.
	AttackUp,		// 攻撃力UP.
	MoveSpeedUp,	// 移動速度UP.

	Max,

} typedef EItemList;

typedef std::unordered_map<EItemList, D3DXVECTOR3>	DropItemList;	// 落とすアイテムのリスト.


#endif	// #ifndef ITEM_H.