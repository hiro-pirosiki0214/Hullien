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
	float		CameraLookHeight;	// カメラの注視点の高さ.
	float		CameraLerpValue;	// カメラ移動の補完値.
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
		, CameraLookHeight	( 0.0 )
		, CameraLerpValue	( 1.0f )
		, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		, SphereAdjRadius	( 0.0f )
	{}
} typedef SPlayerParam;

namespace player
{
// アニメーション番号.
enum enAnimNo
{
	EAnimNo_None = -1,

	EAnimNo_Wait,		// 待機.
	EAnimNo_Walk,		// 走り.
	EAnimNo_Attack1,	// 攻撃1.
	EAnimNo_Attack2,	// 攻撃2.
	EAnimNo_Attack3,	// 攻撃3.

	EAnimNo_Max = EAnimNo_Attack3,
} typedef EAnimNo;

// 攻撃番号,
enum enAttackNo
{
	EAttackNo_None,

	EAttackNo_One,
	EAttackNo_Two, 
	EAttackNo_Three,

	EAttackNo_Max = EAttackNo_Three,
} typedef EAttackNo;

// 効果時間計測番号.
enum enEffectTimerNo
{
	EEffectTimerNo_None,

	EEffectTimerNo_SPRecovery = 0,	// 特殊能力回復.
	EEffectTimerNo_Attack,			// 攻撃力.
	EEffectTimerNo_MoveSpeedUp,		// 移動速度.
	EEffectTimerNo_Paralysis,		// 麻痺.

	EEffectTimerNo_Max,
} typedef EEffectTimerNo;

// 使用エフェクト番号.
enum enEffectNo
{
	enEffectNo_AttackOne,		// 攻撃1.
	enEffectNo_AttackTwo,		// 攻撃2.
	enEffectNo_AttackThree,		// 攻撃3.
	enEffectNo_SP,				// 特殊能力.
	enEffectNo_Avoidance,		// 回避.

	enEffectNo_Max,
} typedef EEffectNo;

// 攻撃用データ.
struct stAttackData
{
	EAnimNo		AnimNo;				// アニメーション番号.
	double		Frame;				// 経過フレーム.
	double		EnabledEndFrame;	// 有効終了フレーム.
	double		EndFrame;			// 終了フレーム.

	stAttackData()
		: AnimNo			( EAnimNo_None )
		, Frame				( 0.0 )
		, EnabledEndFrame	( 1.0 )
		, EndFrame			( 2.0 )
	{}
} typedef SAttackData;

};	// namespace PLAYER.

#endif	// #ifndef PLAYER_PARAM_H.