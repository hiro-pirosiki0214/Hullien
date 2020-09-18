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

	// ‰Šú‰»ŠÖ”.
	virtual bool Init() override;
	// XVŠÖ”.
	virtual void Update() override;
	// •`‰æŠÖ”.
	virtual void Render() override;
	// ƒ‚ƒfƒ‹‚Ì•`‰æ.
	virtual void ModelRender() override;

private:
	std::unique_ptr<CMotherShipUFO>		m_pMotherShipUFO;
	CMotherShipUFO::SMotherShipUFOParam	m_MotherShipUFOParam;
};

#endif	// #ifndef MOTHER_SHIP_UFO_EDIT_H.