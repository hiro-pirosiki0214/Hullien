#ifndef ALIEN_PARAM_H
#define ALIEN_PARAM_H

#include "..\Character.h"

// 宇宙人パラメータ.
struct stAlienParam : public CCharacter::SParameter
{
	// 共通パラメータ---------------------.
	float		RotationalSpeed;			// 回転速度.

	float		SpawnScaleAddValue;			// スポーン時のスケールの加算値.
	float		SpawnDownSpeed;				// スポーン時の降下速度.
	float		MotherShipUpScaleSubValue;	// マザーシップに上昇する時のスケールの減算値.

	int			WaitTime;					// 待機時間.
	int			KnockBackTime;				// ノックバックの時間.

	float		DeadCountAddValue;			// 死亡カウントの加算値.
	float		DeadScaleSubValue;			// 死亡時のスケールの減算値.

											// 宇宙人Bパラメータ------------------.
	float		PlayerAimLenght;			// プレイヤーを狙う範囲.

	float		ResearchLenght;				// 再検査する際の距離.

	float		AttackLenght;				// 攻撃する距離.
	float		AttackRotInitPower;			// 攻撃時の初期回転力.
	float		AttackRotPower;				// 攻撃時の回転力.
	float		AttackRotAddValue;			// 攻撃時の回転加算値.
	float		AttackMoveSpeed;			// 攻撃移動速度.
	float		AttackMoveRange;			// 攻撃移動範囲.

											// 宇宙人Dパラメータ------------------.
	float		LaserMoveSpeed;				// レーザーの移動速度.
	float		ParalysisTime;				// 麻痺の時間.

	float		AttackRangeSpritePosY;		// 攻撃時の範囲表示の調整用.
	float		AttackRangeSpriteScale;		// 攻撃時の範囲表示の画像サイズ.
	float		AttackRangeAddValue;		// 攻撃時の範囲表示のアルファが足される値.
	float		AttackRangeSubValue;		// 攻撃時の範囲表示のアルファが引かれる値.

	float		ControlPointOneLenght;		// ベジェ曲線の一つ目操作座標の距離.
	float		ControlPointOneLenghtY;		// ベジェ曲線の一つ目操作座標のy座標の距離.
	float		ControlPointTwoLenght;		// ベジェ曲線の二つ目操作座標の距離.
	float		ControlPointTwoLenghtY;		// ベジェ曲線の二つ目操作座標のy座標の距離.

	D3DXVECTOR3 SphereAdjPos;				// スフィアの調整座標.
	float		SphereAdjRadius;			// スフィアの調整半径.

	stAlienParam()
		: RotationalSpeed			( 0.0f )

		, SpawnScaleAddValue		( 0.0f )
		, SpawnDownSpeed			( 0.0f )
		, MotherShipUpScaleSubValue	( 0.0f )

		, WaitTime					( 0 )
		, KnockBackTime				( 0 )

		, DeadCountAddValue			( 0.0f )
		, DeadScaleSubValue			( 0.0f )

		, PlayerAimLenght			( 0.0f )
		, ResearchLenght			( 0.0f )

		, AttackLenght				( 0.0f )
		, AttackRotInitPower		( 0.0f )
		, AttackRotPower			( 0.0f )
		, AttackRotAddValue			( 0.0f )
		, AttackMoveSpeed			( 0.0f )
		, AttackMoveRange			( 0.0f )

		, LaserMoveSpeed			( 0.0f )
		, ParalysisTime				( 0.0f )

		, AttackRangeSpritePosY		( 0.0f )
		, AttackRangeSpriteScale	( 1.0f )
		, AttackRangeSubValue		( 0.0f )

		, ControlPointOneLenght		( 0.0f )
		, ControlPointOneLenghtY	( 0.0f )
		, ControlPointTwoLenght		( 0.0f )
		, ControlPointTwoLenghtY	( 0.0f )

		, SphereAdjPos				( 0.0f, 0.0f, 0.0f )
		, SphereAdjRadius			( 0.0f )
	{}
} typedef SAlienParam;

namespace alien
{
	// 宇宙人状態.
	enum class enAlienState
	{
		None,

		Spawn,				// スポーン.
		Move,				// 移動.
		Abduct,				// 拐う.
		KnockBack,			// ノックバック.
		Fright,				// 怯み.
		Death,				// 死亡.
		Escape,				// 逃げる.
		RisingMotherShip,	// 上に上がる(マザーシップに).

		Max,

	} typedef EAlienState;

	// 移動状態.
	enum class enMoveState
	{
		None,

		Rotation,
		Move,
		Attack,
		Wait,

		Max,
	} typedef EMoveState;

	// アニメーション番号.
	enum enAnimNo
	{
		EAnimNo_None = -1,

		EAnimNo_Move,		// 移動.
		EAnimNo_Arm,		// アーム.
		EAnimNo_Damage,		// ダメージ.
		EAnimNo_Dead,		// 死亡.

		EAnimNo_Max,

		EAnimNo_Attack	= EAnimNo_Arm,	// 攻撃.

		EAnimNo_Begin	= EAnimNo_Move,
		EAnimNo_End		= EAnimNo_Max,
	} typedef EAnimNo;

	// エフェクト番号.
	enum enEffectNo
	{
		EEffectNo_None = -1,

		EEffectNo_Hit,			// ヒット.
		EEffectNo_Spawn,		// スポーン.
		EEffectNo_Dead,			// 死亡.
		EEffectNo_Attack,		// 攻撃.

		EEffectNo_Max,
	} typedef EEffectNo;
};

#endif	// #ifndef ALIEN_PARAM_H.