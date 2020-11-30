#ifndef SPAWN_EDIT_H
#define SPAWN_EDIT_H

#include "..\EditBase.h"

class CDX9StaticMesh;
struct stSpawnUFOParam;

class CSpawnEdit : public CEditBase
{
	const char* FILE_PATH = "Data\\GameParam\\Alien\\SpaenParam\\SpaenParam.bin";
	const char* MODEL_NAME = "ufo_mini";
public:
	CSpawnEdit();
	~CSpawnEdit();

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
	// ファイルの読み込み.
	bool FileReading();
	// ファイルの書き込み.
	bool FileWriting();
	// インデックスの描画.
	void IndexRender();
	// スポーン情報の描画.
	void SpawnParamRender( const int& index );

private:
	std::vector<stSpawnUFOParam> m_SpawnPramList;
	std::shared_ptr<CDX9StaticMesh> m_pStaticMesh;
	bool m_IsSucceeded;
	int m_SucceedRenderCount;
};

#endif	// #ifndef SPAWN_EDIT_H.