#ifndef SKY_DOME_H
#define SKY_DOME_H

#include "..\GameObject.h"

class CSkyDome : public CGameObject
{
	const char* MODEL_NAME = "space";	// モデル名.

public:
	CSkyDome();
	virtual ~CSkyDome();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	// モデルの取得.
	bool GetModel();

private:
	std::shared_ptr<CDX9StaticMesh>	m_StaticMesh;	// メッシュ.
};

#endif	// #ifndef SKY_DOME_H.