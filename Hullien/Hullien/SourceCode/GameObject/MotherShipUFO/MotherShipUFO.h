#ifndef MOTHER_SHIP_UFO_H
#define MOTHER_SHIP_UFO_H

#include "..\GameObject.h"

class CActor;
class CCollisionManager;	// 当たり判定クラス.

class CMotherShipUFO : public CGameObject
{
	const char* MODEL_NAME = "ufo";	// モデル名.
	const D3DXVECTOR3 INIT_POS = { 30.0f, 10.0f, 0.0f };
	const float COLLISION_RADIUS = 5.0f;
	const float ADD_POS_POWER = 0.1f;

public:
	CMotherShipUFO();
	virtual ~CMotherShipUFO();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定.
	void Collision( CActor* pActor );

private:
	// モデルの取得.
	bool GetModel();
	// 当たり判定の設定.
	bool CollisionSetting();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;	// スタティックメッシュ.
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// 当たり判定クラス.
};

#endif	// #ifndef MOTHER_SHIP_UFO_H.