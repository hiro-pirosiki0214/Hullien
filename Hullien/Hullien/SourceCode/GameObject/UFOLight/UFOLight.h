#ifndef UFO_LIGHT_H
#define UFO_LIGHT_H

#include "..\GameObject.h"

class CUFOLight : public CGameObject
{
	const char* MODEL_NAME = "light";

	const float SCALE_UP_HEIGHT_SPEED = 0.2f;
	const float SCALE_UP_WEDTH_SPEED = 0.02f;
	const float DEFAULT_SCALE_HEIGHT_MAX = 1.0f;
	const float DEFAULT_SCALE_WEDTH_MAX = 1.0f;
	
	const float SCALE_HEIGHT_MAX;
	const float SCALE_WEDTH_MAX;

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
	CUFOLight( const float& height, const float& wedht );
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
	// 光を完全に放出する.
	void DischargePreparation();
	// 光を片づける.
	void CleanUPPreparation();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;
	EUFOLightState					m_NowState;
};

#endif	// #ifndef UFO_LIGHT_H.