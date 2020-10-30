#ifndef ALIEN_PARAM_EDIT_H
#define ALIEN_PARAM_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Character\Alien\Alien.h"

#include <vector>

class CAlienParamEdit : public CEditBase
{
	const char* FILE_PATH = "Data\\GameParam\\Alien\\AlienParamPathList.txt";

	const char* A_MODEL_NAME = "a_s";
	const char* B_MODEL_NAME = "b_s";
	const char* C_MODEL_NAME = "c_s";
	const char* D_MODEL_NAME = "ultemate_s";
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

private:
	// 全ファイルの読み込み.
	bool FileAllReading();
	// ファイルの読み込み.
	bool FileReading( const char* fileName, CAlien::SAlienParam& param );
	// ファイルの書き込み.
	bool FileWriting( const char* fileName, CAlien::SAlienParam& param );
	// スポーン情報の描画.
	void SpawnParamRender( const int& index );
	// モデルの初期化.
	bool GetModel();

private:
	std::vector<CAlien::SAlienParam>				m_AlienParamList;
	std::vector<std::shared_ptr<CDX9SkinMesh>>		m_pSkinMeshs;
	std::vector<std::string>						m_AlienPathList;
	std::vector<std::string>						m_AlienNameList;
	std::vector<D3DXVECTOR4>						m_AlienNameColorList;
	int m_Index;

};

#endif	// #ifndef ALIEN_PARAM_EDIT_H.