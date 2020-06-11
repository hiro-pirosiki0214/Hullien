#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"

class CAlien : public CCharacter
{
	const float ROTATIONAL_SPEED = 0.05f;	// 回転速度.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.

public:
	// 宇宙人パラメータ.
	struct stAlienParam : public stParameter
	{
		float		RotationalSpeed;	// 回転速度.
		D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
		float		SphereAdjRadius;	// スフィアの調整半径.

		stAlienParam()
			: SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius	( 0.0f )
		{}
	} typedef SAlienParam;

protected:
	// 宇宙人状態.
	enum class enAlienState
	{
		None,

		Spawn,	// スポーン.
		Move,	// 移動.
		Abduct,	// 拐う.
		Fright,	// 怯み.
		Death,	// 死亡.
		Escape,	// 逃げる.

		Max,

	} typedef EAlienState;

	// 移動状態.
	enum class enMoveState
	{
		None,

		Rotation,
		Move,
		Attack,

		Max,
	} typedef EMoveState;
public:
	CAlien();
	virtual ~CAlien();

	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor ) override;
	// スポーン.
	virtual bool Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos ) = 0;

protected:
	// 現在の状態の更新関数.
	void CurrentStateUpdate();
	// 移動ベクトル設定関数.
	void SetMoveVector( const D3DXVECTOR3& targetPos );
	// 目的の座標へ回転.
	void TargetRotation();
	// 移動関数.
	virtual void VectorMove( const float& moveSpeed );

	// スポーン中.
	virtual void Spawning() = 0;
	// 移動.
	virtual void Move() override {}
	// 拐う.
	virtual void Abduct() = 0;
	// 怯み.
	virtual void Fright() = 0;
	// 死亡.
	virtual void Death() = 0;
	// 逃げる.
	virtual void Escape() = 0;

protected:
	D3DXVECTOR3	m_GirlPosition;		// 女の子の座標.
	D3DXVECTOR3	m_TargetRotation;	// 目標の回転情報.
	EAlienState	m_NowState;			// 現在の状態.
};

#endif	// #ifndef ALIEN_H.