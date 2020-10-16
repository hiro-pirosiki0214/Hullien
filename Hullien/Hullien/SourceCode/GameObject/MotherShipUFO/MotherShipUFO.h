#ifndef MOTHER_SHIP_UFO_H
#define MOTHER_SHIP_UFO_H

#include "..\GameObject.h"

class CActor;
class CCollisionManager;	// 当たり判定クラス.

class CMotherShipUFO : public CGameObject
{
public:
	// マザーシップのパラメータ.
	struct stMotherShipUFOParam
	{
		D3DXVECTOR3 Position;			// 座標.
		float		AddPosYPower;		// 持ち上げる力.
		D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
		float		CollisionRadius;	// 当たり判定の半径.

		stMotherShipUFOParam()
			: Position			( 0.0f, 0.0f, 0.0f )
			, AddPosYPower		( 0.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, CollisionRadius	( 1.0f )
		{}

	} typedef SMotherShipUFOParam;

private:
	const char* MODEL_NAME = "ufo";	// モデル名.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Alien\\MotherShipUFOParam\\MotherShipUFOParam.bin";
//	const SMotherShipUFOParam*	pPRAMETER;	// パラメータ(外部からポインタで受け取る).

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
	// パラメータの設定.
	void SetParameter( const SMotherShipUFOParam& param );
	// 描画フラグ設定関数.
	void SetDisp(const bool& disp) { m_IsDisp = disp; }
	// 宇宙人が帰還したか取得関数.
	bool IsReturnAlien() const { return m_IsReturnAlien; }

private:
	// モデルの取得.
	bool GetModel();
	// 当たり判定の設定.
	bool CollisionSetting();

private:
	std::shared_ptr<CDX9StaticMesh>		m_pStaticMesh;	// スタティックメッシュ.
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// 当たり判定クラス.
	SMotherShipUFOParam					m_Param;
	bool								m_IsDisp;		// 描画しているか.
	bool								m_IsReturnAlien;// 宇宙人が帰還したか.
};

#endif	// #ifndef MOTHER_SHIP_UFO_H.