#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "..\Actor.h"

class CExplosion : public CActor
{
public:
	// 爆発パラメーター.
	struct stExplosionParam
	{
		float		ExplosionTime;		// 爆発時間.
		float		AttackPower;		// 爆発力.
		D3DXVECTOR3	SphereAdjPos;		// スフィアの調整用座標.
		float		SphereAdjRadius;	// スフィアの調整用半径.

		stExplosionParam()
			: ExplosionTime		( 0.0f )
			, AttackPower		( 0.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius	( 0.0f )
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

	// パラメーターの設定.
	void SetExplosionParam( const SExplosionParam& param ){ m_Param = param; }

private:
	// 当たり判定の設定.
	bool ColliderSetting();
	// モデルの取得(なくなってエフェクトになる).
	bool GetModel( const char* modelName );

private:
	SExplosionParam	m_Param;		// 爆発パラメーター.
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;
};

#endif	// #ifndef EXPLOSION_H.