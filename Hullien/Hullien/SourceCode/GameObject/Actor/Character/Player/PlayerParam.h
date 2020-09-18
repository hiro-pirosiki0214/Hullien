#ifndef PLAYER_PARAM_H
#define PLAYER_PARAM_H

#include "..\Character.h"

// プレイヤーパラメータ.
struct stPlayerParam : public CCharacter::SParameter
{
	float		SpecialAbilityMax;	// 特殊能力最大値.
	float		SpecialAbilityValue;// 特殊能力回復値.
	int			AttackComboMax;		// 攻撃の最大数.
	int			AttackQueueMax;		// 攻撃キューの最大数.
	float		AvoidMoveDistance;	// 回避の移動距離.
	float		AvoidMoveSpeed;		// 回避用の移動速度.
	float		CameraMoveSpeed;	// カメラの移動速度.
	float		CameraDistance;		// カメラの距離.
	float		CameraHeight;		// カメラの高さ.
	D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
	float		SphereAdjRadius;	// スフィアの調整半径.

	stPlayerParam()
		: SpecialAbilityMax	( 0.0f )
		, AttackComboMax	( 0 )
		, AttackQueueMax	( 0 )
		, AvoidMoveDistance	( 0.0f )
		, AvoidMoveSpeed	( 0.0f )
		, CameraMoveSpeed	( 0.01f )
		, CameraDistance	( 7.0f )
		, CameraHeight		( 4.0f )
		, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		, SphereAdjRadius	( 0.0f )
	{}
} typedef SPlayerParam;

#endif	// #ifndef PLAYER_PARAM_H.