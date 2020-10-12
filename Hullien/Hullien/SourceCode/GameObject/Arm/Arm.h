#ifndef ARM_H
#define ARM_H

#include "..\GameObject.h"

class CArm : public CGameObject
{
public:
	inline static const float GRAB_DISTANCE	= 5.0f;		// 掴むときの距離.

private:
	const char* MODEL_NAME = "armsandwich_s";

	const float APPEARANCE_ADD_VALUE	= 0.005f;	// 出現時の加算値.
	const float APPEARANCE_COUNT_MAX	= 1.0f;		// 出現時の最大数.
	const float CLEAN_UP_SUB_VALUE		= 0.015f;	// 片づけ時の減算値.
	const float CLEAN_UP_COUNT_MIN		= 0.0f;		// 片づけ時の最小数.

	const float SCALING_VALUE			= 0.5f;		// 拡縮値.
	const float SCALING_VALUE_MAX		= 1.0f;		// 拡縮の最大値.
	const float SCALING_ADD_VALUE		= (1.0f-SCALING_VALUE)*0.04f;	// 拡縮の加算値.

	enum class enArmState
	{
		None,

		Appearance,	// 出現.
		Grab,		// 掴む.
		CleanUp,	// 片づける.

		End,		// 終了.

		Start = None,

		Max,


	} typedef EArmState;
public:
	CArm();
	virtual ~CArm();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// Y軸の設定.
	void SetRotationY( const float& rotY ){ m_vRotation.y = rotY; }

	// 掴んでいるか.
	bool IsGrab(){ return m_NowArmState == EArmState::End; }
	// 片づけているか.
	bool IsCleanUp(){ return m_NowArmState == EArmState::Start; }
	// 掴んでいる座標の取得.
	D3DXVECTOR3 GetGrabPosition();

	// 出現する用意.
	void SetAppearance();
	// 片づける用意.
	void SetCleanUp();

private:
	// 出現.
	void Appearance();
	// 掴む.
	void Grab();
	// 片づける.
	void CleanUp();

private:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;	// スキンメッシュ.
	LPD3DXANIMATIONCONTROLLER		m_pAC;			// アニメーションコントローラー.
	std::shared_ptr<CDX9StaticMesh>	m_pAMesh;		// スキンメッシュ.
	std::shared_ptr<CDX9StaticMesh>	m_pGirlMesh;	// スキンメッシュ.
	D3DXVECTOR3						m_GrabPosition;	// 掴んでいる時の座標.
	EArmState						m_NowArmState;	// 現在のアームの状態.
	EArmState						m_OldArmState;	// 前回のアームの状態.
	float	m_AppearanceCount;	// 出現カウント.
	float	m_ScalingValue;		// 拡縮値.
	double	m_AnimSpeed;		// アニメーション速度.
	double	m_AnimEndFrame;		// アニメーションの終了フレーム.

};

#endif	// #ifndef ARM_H.