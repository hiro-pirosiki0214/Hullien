#ifndef ARM_H
#define ARM_H

#include "..\GameObject.h"

class CArm : public CGameObject
{
	const char* MODEL_NAME = "arm_open";

	const float APPEARANCE_ADD_VALUE	= 0.005f;
	const float APPEARANCE_COUNT_MAX	= 1.0f;
	const float SCALING_VALUE			= 0.8f;
	const float SCALING_VALUE_MAX		= 1.0f;
	const float SCALING_ADD_VALUE		= (1.0f-SCALING_VALUE)*0.04f;

	enum class enArmState
	{
		None,

		Appearance,	// 出現.


		Max,

		Start = Appearance,

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

private:
	// 出現.
	void Appearance();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pSkinMesh;	// スキンメッシュ.
	std::shared_ptr<CDX9StaticMesh>	m_pAMesh;	// スキンメッシュ.
	EArmState						m_NowArmState;
	float	m_AppearanceCount;	// 出現カウント.
	float	m_ScalingValue;		// 拡縮値.
	bool	m_IsScaleDown;		// 縮小しているか.
};

#endif	// #ifndef ARM_H.