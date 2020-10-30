#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"
#include "..\..\Item\ItemList.h"

#include <random>

class CArm;
class CEffectManager;

class CAlien : public CCharacter
{
	const float ROTATIONAL_SPEED = 0.05f;	// 回転速度.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.
	const float BARRIER_HIT_MOVE_SPEED = -5.0f;	// バリアと衝突時の移動速度.
	const float ADD_SCALE_VALUE = 0.03f;
	const float SCALE_MAX		= 1.0f;
	const float DOWN_SPEED		= 0.02f;
	const int KNOCK_BACK_TIME	= 5;
	const float DEATH_COUNT_ADD_VALUE = 0.005f;
	const float DEATH_SCALE_SUB_VALUE = 0.005f;
	const float DEATH_SCALE_PI	= 6.0f*static_cast<float>(D3DX_PI);
	const float RISING_MOTHER_SHIP_SCALE_SUB_VALUE = 0.01f;
protected:
	const float MODEL_ALPHA_MAX			= 1.0f;				// モデルアルファの最大値.
	const char* HIT_EEFECT_NAME			= "hiteffekt";		// ヒットエフェクト.
	const char* SPAWN_EFFECT_NAME		= "b_attackeffkt";	// スポーンエフェクト.
	const char* DEAD_EFFECT_NAME		= "b_attackeffkt";	// 死亡エフェクト.
	const char* ATTACK_EFFECT_NAME		= "b_attackeffkt";	// 攻撃エフェクト.

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

	enum enAnimNo
	{
		EAnimNo_None = -1,

		EAnimNo_Move,
		EAnimNo_Arm,
		EAnimNo_Damage,
		EAnimNo_Dead,

		EAnimNo_Max,

		EAnimNo_Begin	= EAnimNo_Move,
		EAnimNo_End		= EAnimNo_Max,
	} typedef EAnimNo;

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

public:
	CAlien();
	virtual ~CAlien();

	// エフェクトの描画.
	virtual void EffectRender() override;

	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor ) override;
	// ベクトルの取得.
	virtual void SetVector( const D3DXVECTOR3& vec ) override;
	// スポーン.
	virtual bool Spawn( const SAlienParam& param, const D3DXVECTOR3& spawnPos ) = 0;

	// ライフ計算関数.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ) override;
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
	bool IsRisingMotherShip() const { return m_NowState == EAlienState::RisingMotherShip; }

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
	// ノックバック.
	virtual void KnockBack();
	// 怯み.
	virtual void Fright();
	// 死亡.
	virtual void Death();
	// 逃げる.
	virtual void Escape();
	// マザーシップに昇っている.
	virtual void RisingMotherShip();

	// 女の子との当たり判定.
	void GirlCollision( CActor* pActor );
	// バリアとの当たり判定.
	void BarrierCollision( CActor* pActor );

	// アニメーションコントローラーの取得.
	bool GetAnimationController();

	// アニメーションフレームの設定.
	virtual bool SetAnimFrameList() override;
	// エフェクトの設定.
	virtual bool EffectSetting();

protected:
	std::unique_ptr<CArm>		m_pArm;			// アームクラス.
	LPD3DXANIMATIONCONTROLLER	m_pAC;			// アニメーションコントローラー.
	std::vector<std::shared_ptr<CEffectManager>>	m_pEffects;
	D3DXVECTOR3		m_TargetPosition;			// 女の子の座標.
	D3DXVECTOR3		m_TargetRotation;			// 目標の回転情報.
	D3DXVECTOR3		m_KnockBackVector;
	D3DXVECTOR3		m_BeforeMoveingPosition;	// 移動前の座標.
	D3DXVECTOR3*	m_pAbductUFOPosition;		// UFOの座標.
	SAlienParam		m_Parameter;				// パラメータ.
	EAlienState		m_NowState;					// 現在の状態.
	EMoveState		m_NowMoveState;				// 現在の移動状態.
	EItemList		m_HasAnyItem;				// どのアイテムを持っているか.
	float			m_LifePoint;				// 体力.
	float			m_MoveSpeed;				// 移動速度.
	float			m_DeathScale;				// 死亡時の大きさ.
	float			m_DeathCount;				// 死亡カウント.
	int				m_KnockBackCount;			// ノックバックカウント.
	int				m_WaitCount;				// 待機カウント.
	bool*			m_pIsAlienOtherAbduct;		// 他の宇宙人が連れ去っているかどうか.
	bool			m_IsBarrierHit;				// バリアに当たっているか.
	bool			m_IsExplosion;				// 爆発するか.
	bool			m_IsDelete;					// 消去するかどうか.

	std::shared_ptr<CDX9StaticMesh>	m_pTempStaticMesh;
};

#endif	// #ifndef ALIEN_H.