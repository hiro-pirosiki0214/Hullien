#ifndef ALIEN_PARAM_EDIT_H
#define ALIEN_PARAM_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Character\Alien\AlienParam.h"
#include "..\..\GameObject\Actor\Character\Alien\EditAlien\EditAlien.h"
#include "..\..\GameObject\MotherShipUFO\MotherShipUFO.h"
#include "..\..\GameObject\SpawnUFO\SpawnUFO.h"

#include <vector>

class CAlienParamEdit : public CEditBase
{
	const char* FILE_PATH = "Data\\GameParam\\Alien\\AlienParamPathList.txt";
	const char* MOTHER_SHIP_UFO_PARAM_FILE_PATH = "Data\\GameParam\\Alien\\MotherShipUFOParam\\MotherShipUFOParam.bin";
	const char* SPAWN_UFO_PARAM_FILE_PATH = "Data\\GameParam\\Alien\\SpaenParam\\SpaenParam.bin";

	const char* SPAWN_UFO_MODEL_NAME = "ufo_mini";
public:
	CAlienParamEdit();
	~CAlienParamEdit();

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
	// �S�t�@�C���̓ǂݍ���.
	bool FileAllReading();
	// �t�@�C���̓ǂݍ���.
	bool FileReading( const char* fileName, SAlienParam& param );
	// �t�@�C���̏�������.
	bool FileWriting( const char* fileName, SAlienParam& param );
	// �X�|�[�����̕`��.
	void SpawnParamRender( const int& index );
	// �F���l�̏�����.
	bool InitAlien();

private:
	std::vector<SAlienParam>						m_AlienParamList;
	std::vector<std::string>						m_AlienPathList;
	std::vector<std::string>						m_AlienNameList;
	std::vector<std::shared_ptr<CEditAlien>>		m_pEditAliens;
	std::unique_ptr<CMotherShipUFO>					m_pMotherShipUFO;
	CMotherShipUFO::SMotherShipUFOParam				m_MotherShipUFOParam;
	std::unique_ptr<CSpawnUFO>						m_pSpawnUFO;
	stSpawnUFOParam									m_SpawnUFOParam;
	int		m_AlienIndex;
	int		m_NowParamIndex;
	bool	m_IsRisingMotherShip;

};

#endif	// #ifndef ALIEN_PARAM_EDIT_H.