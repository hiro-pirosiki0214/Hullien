#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"
#include "..\..\Item\ItemList.h"

#include <random>

class CArm;

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

		float		ResearchLenght;		// 再検査する際の距離.

		float		AttackLenght;		// 攻撃する距離.
		float		AttackRotInitPower;	// 攻撃時の初期回転力.
		float		AttackRotPower;		// 攻撃時の回転力.
		float		AttackRotAddValue;	// 攻撃時の回転加算値.
		float		AttackMoveSpeed;	// 攻撃移動速度.
		float		AttackMoveRange;	// 攻撃移動範囲.

		float		LaserMoveSpeed;		// レーザーの移動速度.
		float		ParalysisTime;		// 麻痺の時間.

		float		AttackRangeSpritePosY;	// 攻撃時の範囲表示の調整用.
		float		AttackRangeSpriteScale;	// 攻撃時の範囲表示の画像サイズ.
		float		AttackRangeAddValue;	// 攻撃時の範囲表示のアルファが足される値.
		float		AttackRangeSubValue;	// 攻撃時の範囲表示のアルファが引かれる値.

		float		ControlPointOneLenght;	// ベジェ曲線の一つ目操作座標の距離.
		float		ControlPointOneLenghtY;	// ベジェ曲線の一つ目操作座標のy座標の距離.
		float		ControlPointTwoLenght;	// ベジェ曲線の二つ目操作座標の距離.
		float		ControlPointTwoLenghtY;	// ベジェ曲線の二つ目操作座標のy座標の距離.

		D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
		float		SphereAdjRadius;	// スフィアの調整半径.

		stAlienParam()
			: RotationalSpeed		( 0.0f )

			, ModelAlphaAddValue	( 0.0f )
			, ModelAlphaSubValue	( 0.0f )

			, WaitTime				( 0 )

			, PlayerAimLenght		( 0.0f )
			, ResearchLenght		( 0.0f )

			, AttackLenght			( 0.0f )
			, AttackRotInitPower	( 0.0f )
			, AttackRotPower		( 0.0f )
			, AttackRotAddValue		( 0.0f )
			, AttackMoveSpeed		( 0.0f )
			, AttackMoveRange		( 0.0f )

			, LaserMoveSpeed		( 0.0f )
			, ParalysisTime			( 0.0f )

			, AttackRangeSpritePosY		( 0.0f )
			, AttackRangeSpriteScale	( 1.0f )
			, AttackRangeSubValue		( 0.0f )

			, ControlPointOneLenght		( 0.0f )
			, ControlPointOneLenghtY	( 0.0f )
			, ControlPointTwoLenght		( 0.0f )
			, ControlPointTwoLenghtY	( 0.0f )

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
	// どのアイテムを持っているか取得.
	EItemList GetAnyItem() const { return m_HasAnyItem; }
	// ほかの宇宙人が連れ去っているか設定.
	void SetOtherAbduct( bool* pisAbduct ){ m_pIsAlienOtherAbduct = pisAbduct; }
	// 連れ去るUFOの座標の取得.
	void SetAbductUFOPosition( D3DXVECTOR3* pos ){ m_pAbductUFOPosition = pos; }
	// アイテムの設定.
	void SetItem( const EItemList& item ){ m_HasAnyItem = item; }
	// 爆発するか.
	bool IsExplosion() const { return m_IsExplosion; }
	// 消去するかどうか.
	bool IsDelete() const { return m_IsDelete; }
	// マザーシップに昇っているか.
	bool IsRisingMotherShip() const { return m_IsRisingMotherShip; }

protected:
	// 現在の状態の更新関数.
	void CurrentStateUpdate();
	// 女の子の座標を設定.
	void SetGirlPos( CActor& actor );
	// 座標設定関数.
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;

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

	// アルファブレンドの設定.
	void AlphaBlendSetting();

	// 女の子との当たり判定.
	void GirlCollision( CActor* pActor );
	// バリアとの当たり判定.
	void BarrierCollision( CActor* pActor );

protected:
	std::unique_ptr<CArm>	m_pArm;				// アームクラス.
	D3DXVECTOR3		m_TargetPosition;			// 女の子の座標.
	D3DXVECTOR3		m_TargetRotation;			// 目標の回転情報.
	D3DXVECTOR3		m_BeforeMoveingPosition;	// 移動前の座標.
	D3DXVECTOR3*	m_pAbductUFOPosition;		// UFOの座標.
	SAlienParam		m_Parameter;				// パラメータ.
	EAlienState		m_NowState;					// 現在の状態.
	EMoveState		m_NowMoveState;				// 現在の移動状態.
	EItemList		m_HasAnyItem;				// どのアイテムを持っているか.
	float			m_LifePoint;				// 体力.
	float			m_MoveSpeed;				// 移動速度.
	float			m_ModelAlpha;				// モデルのアルファ値.
	int				m_WaitCount;				// 待機カウント.
	bool*			m_pIsAlienOtherAbduct;		// 他の宇宙人が連れ去っているかどうか.
	bool			m_IsBarrierHit;				// バリアに当たっているか.
	bool			m_IsExplosion;				// 爆発するか.
	bool			m_IsDelete;					// 消去するかどうか.
	bool			m_IsRisingMotherShip;		// マザーシップに昇っているか.
};

#endif	// #ifndef ALIEN_H.