#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"

class CRotLookAtCenter;

class CPlayer : public CCharacter
{
	const char* MODEL_NAME = "Sayaka_s";	// モデル名.
public:
	CPlayer();
	virtual ~CPlayer();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	// 操作関数.
	void Controller();
	// 移動関数.
	virtual void Move() override;

private:
	std::shared_ptr<CDX9SkinMesh>		m_pSkinMesh;	// スキンメッシュ.
	std::shared_ptr<CRotLookAtCenter>	m_pCamera;		// カメラクラス.
};

#endif	// #ifndef PLAYER_H.