#ifndef ACTOR_H
#define ACTOR_H

#include "..\GameObject.h"
#include <functional>

class CActor : public CGameObject
{
public:
	CActor();
	virtual ~CActor();

	// ライフ計算関数.
	virtual void LifeCalculation( const std::function<void(float&)>& ){}
	// 特殊能力回復時間、効力時間設定関数.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ){}
	// 攻撃力、効力時間設定関数.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ){}
	// 移動速度、効力時間設定関数.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ){}
	// 相手座標の設定.
	virtual void SetOpponentPos( CActor& actor ){}

};

#endif	// #ifndef ACTOR_H.