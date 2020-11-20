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
	// 全ファイルの読み込み.
	bool FileAllReading();
	// ファイルの読み込み.
	bool FileReading( const char* fileName, SAlienParam& param );
	// ファイルの書き込み.
	bool FileWriting( const char* fileName, SAlienParam& param );
	// スポーン情報の描画.
	void SpawnParamRender( const int& index );
	// 宇宙人の初期化.
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