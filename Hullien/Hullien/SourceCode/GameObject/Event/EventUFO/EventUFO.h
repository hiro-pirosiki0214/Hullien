#ifndef EVENT_UFO_H
#define EVENT_UFO_H

#include "..\..\GameObject.h"

/**************************************
*	イベント用UFOクラス.
**/
class CEventUFO : public CGameObject
{
	const char* MODEL_NAME = "ufo_mini";	// モデル名.
	const float POS_HEIGHT = 10.0f;			// UFOの高さ.

public:
	CEventUFO();
	virtual ~CEventUFO();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 大きさの設定関数.
	void SetScale(const D3DXVECTOR3& vScale) { m_vSclae = vScale; }

private:
	// モデルの取得.
	bool GetModel();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;	// メッシュ.

};

#endif //#ifndef EVENT_UFO_H.
