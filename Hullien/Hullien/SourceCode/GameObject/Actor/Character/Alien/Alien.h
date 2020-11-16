#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"
#include "AlienParam.h"
#include "..\..\Item\ItemList.h"

#include <random>

class CArm;
class CEffectManager;

class CAlien : public CCharacter
{
protected:
	inline static const float TOLERANCE_RADIAN			= static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.
	inline static const float BARRIER_HIT_MOVE_SPEED	= -5.0f;									// バリアと衝突時の移動速度.
	inline static const float SCALE_MAX					= 1.0f;										// モデルの最大値.
	inline static const float DEATH_SCALE_PI			= 6.0f*static_cast<float>(D3DX_PI);			// 死亡時のスケールの範囲.

protected:
	inline static const char* HIT_EEFECT_NAME		= "hiteffekt";		// ヒットエフェクト.
	inline static const char* SPAWN_EFFECT_NAME		= "uvtest";			// スポーンエフェクト.
	inline static const char* DEAD_EFFECT_NAME		= "uvtest";			// 死亡エフェクト.
	inline static const char* ATTACK_EFFECT_NAME	= "uvtest";			// 攻撃エフェクト.

	const SAlienParam*	pPARAMETER;	// パラメータのポインタ : 外部から取得.

public:
	CAlien();
	CAlien( const SAlienParam* pParam );
	virtual ~CAlien();

	// エフェクトの描画.
	virtual void EffectRender() override;

	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor ) override;
	// ベクトルの取得.
	virtual void SetVector( const D3DXVECTOR3& vec ) override;
	// スポーン.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) = 0;

	// ライフ計算関数.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ) override;
	// 連れ去っているかどうか.
	inline bool IsAbduct() const { return m_NowState == alien::EAlienState::Abduct; }
	// どのアイテムを持っているか取得.
	inline EItemList GetAnyItem() const { return m_HasAnyItem; }
	// ほかの宇宙人が連れ去っているか設定.
	inline void SetOtherAbduct( bool* pisAbduct ){ m_pIsAlienOtherAbduct = pisAbduct; }
	// 連れ去るUFOの座標の取得.
	inline void SetAbductUFOPosition( D3DXVECTOR3* pos ){ m_pAbductUFOPosition = pos; }
	// アイテムの設定.
	inline void SetItem( const EItemList& item ){ m_HasAnyItem = item; }
	// 爆発するか.
	inline bool IsExplosion() const { return m_IsExplosion; }
	// 消去するかどうか.
	inline bool IsDelete() const { return m_IsDelete; }
	// マザーシップに昇っているか.
	inline bool IsRisingMotherShip() const { return m_NowState == alien::EAlienState::RisingMotherShip; }

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
	D3DXVECTOR3				m_TargetPosition;			// 女の子の座標.
	D3DXVECTOR3				m_TargetRotation;			// 目標の回転情報.
	D3DXVECTOR3				m_KnockBackVector;			// ノックバックの移動ベクトル.
	D3DXVECTOR3				m_BeforeMoveingPosition;	// 移動前の座標.
	D3DXVECTOR3*			m_pAbductUFOPosition;		// UFOの座標.
	alien::EAlienState		m_NowState;					// 現在の状態.
	alien::EMoveState		m_NowMoveState;				// 現在の移動状態.
	EItemList				m_HasAnyItem;				// どのアイテムを持っているか.
	float					m_LifePoint;				// 体力.
	float					m_MoveSpeed;				// 移動速度.
	float					m_DeathScale;				// 死亡時の大きさ.
	float					m_DeathCount;				// 死亡カウント.
	int						m_KnockBackCount;			// ノックバックカウント.
	int						m_WaitCount;				// 待機カウント.
	bool*					m_pIsAlienOtherAbduct;		// 他の宇宙人が連れ去っているかどうか.
	bool					m_IsBarrierHit;				// バリアに当たっているか.
	bool					m_IsRisingMotherShip;		// マザーシップに上っているか.
	bool					m_IsExplosion;				// 爆発するか.
	bool					m_IsDelete;					// 消去するかどうか.

	std::shared_ptr<CDX9StaticMesh>	m_pTempStaticMesh;
};

#endif	// #ifndef ALIEN_H.