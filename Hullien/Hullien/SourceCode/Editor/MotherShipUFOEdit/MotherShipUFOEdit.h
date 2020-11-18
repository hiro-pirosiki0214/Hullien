#ifndef MOTHER_SHIP_UFO_EDIT_H
#define MOTHER_SHIP_UFO_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\MotherShipUFO\MotherShipUFO.h"

class CMotherShipUFOEdit : public CEditBase
{
	const char* MOTHER_SHIP_UFO_PARAM_FILE_PATH = "Data\\GameParam\\Alien\\MotherShipUFOParam\\MotherShipUFOParam.bin";
public:
	CMotherShipUFOEdit();
	virtual ~CMotherShipUFOEdit();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// モデルの描画.
	virtual void ModelRender() override;
	// エフェクトの描画.
	virtual void EffectRender() override;

private:
	std::unique_ptr<CMotherShipUFO>		m_pMotherShipUFO;
	CMotherShipUFO::SMotherShipUFOParam	m_MotherShipUFOParam;
};

#endif	// #ifndef MOTHER_SHIP_UFO_EDIT_H.