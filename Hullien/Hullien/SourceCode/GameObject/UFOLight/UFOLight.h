#ifndef UFO_LIGHT_H
#define UFO_LIGHT_H

#include "..\GameObject.h"

class CUFOLight : public CGameObject
{
	const char* MODEL_NAME = "light";

	const float SCALE_UP_HEIGHT_SPEED = 0.2f;
	const float SCALE_UP_WEDTH_SPEED = 0.02f;
	const float SCALE_MAX = 1.0f;

	enum class enUFOLightState
	{
		None,
		

		Discharge,		// 取り出す.
		CleanUP,		// 片づける.
		EndDischarge,	// 取り出し終わった.
		EndCleanUP,		// 片づけ終わった.

		Max,
	} typedef EUFOLightState;
public:
	CUFOLight();
	virtual ~CUFOLight();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 光を放出する.
	void Discharge();
	// 光を片づける.
	void CleanUP();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;
	EUFOLightState					m_NowState;
};

#endif	// #ifndef UFO_LIGHT_H.