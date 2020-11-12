#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "..\Actor.h"

class CEffectManager;	// エフェクトクラス.

class CExplosion : public CActor
{
	const char* EFFECT_NAME = "c_deadEffect";	// エフェクトの名前.
public:
	// 爆発パラメーター.
	struct stExplosionParam
	{
		float		ExplosionSpeed;		// 爆発速度.
		float		AttackPower;		// 爆発力.
		float		SphereMaxRadius;	// スフィアの最大半径.
		D3DXVECTOR3	SphereAdjPos;		// スフィアの調整用座標.

		stExplosionParam()
			: ExplosionSpeed	( 0.0f )
			, AttackPower		( 0.0f )
			, SphereMaxRadius	( 0.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		{}
	} typedef SExplosionParam;


public:
	CExplosion();
	virtual ~CExplosion();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& pActor ) override;
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;
	
	// 止まっているかどうか.
	bool IsStop();

	// パラメーターの設定.
	inline void SetExplosionParam( const SExplosionParam& param ){ m_Param = param; }

private:
	// エフェクトの設定.
	bool EffectSetting();
	// 当たり判定の設定.
	bool ColliderSetting();

private:
	std::shared_ptr<CEffectManager>	m_pEffect;	// エフェクト.
	SExplosionParam	m_Param;	// 爆発パラメーター.
	float m_CollSphereRadius;	// 当たり判定の半径.
	bool m_IsEffectPlay;		// エフェクトが再生されたか.
#if _DEBUG
	float m_ResizeCollTime;		// 当たり判定の半径のリサイズする時間(デバッグ用).
#endif	// #if _DEBUG.
};

#endif	// #ifndef EXPLOSION_H.