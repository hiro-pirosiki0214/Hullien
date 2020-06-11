#ifndef ACTOR_H
#define ACTOR_H

#include "..\GameObject.h"
#include <functional>

class CCollisionManager;	// 当たり判定クラス.

class CActor : public CGameObject
{
public:
	CActor();
	virtual ~CActor();

	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) = 0;

	// ライフ計算関数.
	virtual void LifeCalculation( const std::function<void(float&)>& ){}
	// 特殊能力回復時間、効力時間設定関数.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ){}
	// 攻撃力、効力時間設定関数.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ){}
	// 移動速度、効力時間設定関数.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ){}
	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& actor ){}

	// 当たり判定取得関数.
	CCollisionManager* GetCollManager(){ return m_pCollManager.get(); }

protected:
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// 当たり判定クラス.
};

#endif	// #ifndef ACTOR_H.