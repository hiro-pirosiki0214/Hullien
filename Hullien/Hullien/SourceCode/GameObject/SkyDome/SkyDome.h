#ifndef SKY_DOME_H
#define SKY_DOME_H

#include "..\GameObject.h"

class CSkyDomeModel;

class CSkyDome : public CGameObject
{
	const char* MODEL_NAME = "Data\\Mesh\\space\\space.x";	// モデル名.

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
	std::unique_ptr<CSkyDomeModel>	m_pDomeModel;	// メッシュ.
};

#endif	// #ifndef SKY_DOME_H.