#ifndef ACTOR_H
#define ACTOR_H

#include "..\GameObject.h"
#include <functional>

class CCollisionManager;			// 当たり判定クラス.
struct stBoxWall typedef SBoxWall;	// 見えない壁の構造体.

class CActor : public CGameObject
{
public:
	CActor();
	virtual ~CActor();

	// エフェクトの描画.
	virtual void EffectRender(){}
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) = 0;

	// ライフ計算関数.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ){}
	// 特殊能力回復時間、効力時間設定関数.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ){}
	// 攻撃力、効力時間設定関数.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ){}
	// 移動速度、効力時間設定関数.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ){}
	// 麻痺の設定.
	virtual void SetParalysisTime( const std::function<void(float&)>& ){}
	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& actor ){}
	// ベクトルの取得.
	virtual void SetVector( const D3DXVECTOR3& vec ){};

	// 見えない壁の設定.
	void SetBoxWall( SBoxWall* box ){ m_pBoxWall = box; }

	// 当たり判定取得関数.
	CCollisionManager* GetCollManager(){ return m_pCollManager.get(); }

protected:
	// 壁に衝突したか.
	bool IsCrashedWall();
	// 壁に衝突したか : X.
	bool IsCrashedWallX();
	// 壁に衝突したか : Z.
	bool IsCrashedWallZ();

protected:
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// 当たり判定クラス.
	SBoxWall*							m_pBoxWall;		// 見えない壁 : 外部からポインタで取得.
};

#endif	// #ifndef ACTOR_H.