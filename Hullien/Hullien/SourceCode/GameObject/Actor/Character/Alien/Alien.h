#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"

class CAlien : public CCharacter
{
	const float ROTATIONAL_SPEED = 0.05f;	// 回転速度.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.

protected:
	const float MODEL_ALPHA_MAX = 1.0f;	// モデルアルファの最大値.


public:
	// 宇宙人パラメータ.
	struct stAlienParam : public stParameter
	{
		float		RotationalSpeed;	// 回転速度.

		float		ModelAlphaAddValue;	// モデルのアルファ値の加算する値.
		float		ModelAlphaSubValue;	// モデルのアルファ値の減算する値.

		int			WaitTime;			// 待機時間.

		float		PlayerAimLenght;	// プレイヤーを狙う範囲.

		float		AttackLenght;		// 攻撃する距離.
		float		AttackRotInitPower;	// 攻撃時の初期回転力.
		float		AttackRotPower;		// 攻撃時の回転力.
		float		AttackRotAddValue;	// 攻撃時の回転加算値.
		float		AttackMoveSpeed;	// 攻撃移動速度.
		float		AttackMoveRange;	// 攻撃移動範囲.

		D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
		float		SphereAdjRadius;	// スフィアの調整半径.

		stAlienParam()
			: RotationalSpeed		( 0.0f )

			, ModelAlphaAddValue	( 0.0f )
			, ModelAlphaSubValue	( 0.0f )

			, WaitTime				( 0 )

			, PlayerAimLenght		( 0.0f )

			, AttackLenght			( 0.0f )
			, AttackRotInitPower	( 0.0f )
			, AttackRotPower		( 0.0f )
			, AttackRotAddValue		( 0.0f )
			, AttackMoveSpeed		( 0.0f )
			, AttackMoveRange		( 0.0f )

			, SphereAdjPos			( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius		( 0.0f )
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
		Wait,

		Max,
	} typedef EMoveState;

public:
	CAlien();
	virtual ~CAlien();

	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor ) override;
	// スポーン.
	virtual bool Spawn( const SAlienParam& param, const D3DXVECTOR3& spawnPos ) = 0;

	// ライフ計算関数.
	virtual void LifeCalculation( const std::function<void(float&)>& ) override;
	// モデルのアルファ値の取得.
	float GetModelAplha() const { return m_ModelAlpha; }
	// 連れ去っているかどうか.
	bool IsAbduct() const { return m_NowState == EAlienState::Abduct; }
	// ほかの宇宙人が連れ去っているか設定.
	void SetOtherAbduct( bool* pisAbduct ){ m_pIsAlienOtherAbduct = pisAbduct; }
	// 連れ去るUFOの座標の取得.
	void SetAbductUFOPosition( D3DXVECTOR3* pos ){ m_pAbductUFOPosition = pos; }

	// 消去するかどうか.
	bool IsDelete() const { return m_IsDelete; }
protected:
	// 現在の状態の更新関数.
	void CurrentStateUpdate();
	// 移動ベクトル設定関数.
	virtual void SetMoveVector( const D3DXVECTOR3& targetPos );
	// 目的の座標へ回転.
	void TargetRotation();
	// 移動関数.
	virtual void VectorMove( const float& moveSpeed );
	// 待機関数.
	virtual void WaitMove();

	// スポーン中.
	virtual void Spawning();
	// 移動.
	virtual void Move() override;
	// 拐う.
	virtual void Abduct();
	// 怯み.
	virtual void Fright();
	// 死亡.
	virtual void Death();
	// 逃げる.
	virtual void Escape();

	// 女の子との当たり判定.
	void GirlCollision( CActor* pActor );

protected:
	D3DXVECTOR3		m_TargetPosition;		// 女の子の座標.
	D3DXVECTOR3		m_TargetRotation;		// 目標の回転情報.
	D3DXVECTOR3*	m_pAbductUFOPosition;	// UFOの座標.
	SAlienParam		m_Parameter;			// パラメータ.
	EAlienState		m_NowState;				// 現在の状態.
	EMoveState		m_NowMoveState;			// 現在の移動状態.
	float			m_ModelAlpha;			// モデルのアルファ値.
	int				m_WaitCount;			// 待機カウント.
	bool*			m_pIsAlienOtherAbduct;	// 他の宇宙人が連れ去っているかどうか.

	bool			m_IsDelete;			// 消去するかどうか.
};

#endif	// #ifndef ALIEN_H.