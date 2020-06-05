#ifndef GROUND_STAGE_H
#define GROUND_STAGE_H

#include "..\GameObject.h"

/**********************************
*	地面クラス.
*/
class CGroundStage : public CGameObject
{
	const char* MODEL_NAME = "ground";	// モデルの名前.
public:
	CGroundStage();
	virtual ~CGroundStage();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;
};

#endif	// #ifndef GROUND_STAGE_H.