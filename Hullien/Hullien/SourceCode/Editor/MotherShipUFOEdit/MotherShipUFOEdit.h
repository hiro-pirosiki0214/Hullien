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

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// ���f���̕`��.
	virtual void ModelRender() override;
	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() override;

private:
	std::unique_ptr<CMotherShipUFO>		m_pMotherShipUFO;
	CMotherShipUFO::SMotherShipUFOParam	m_MotherShipUFOParam;
};

#endif	// #ifndef MOTHER_SHIP_UFO_EDIT_H.