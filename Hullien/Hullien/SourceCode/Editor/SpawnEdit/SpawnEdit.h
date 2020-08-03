#ifndef SPAWN_EDIT_H
#define SPAWN_EDIT_H

#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include <vector>

class CDX9StaticMesh;
struct stSpawnUFOParam;

class CSpawnEdit
{
	const ImVec2 WINDOW_SIZE		= { 450.0f, 350.0f };	// ウィンドウサイズ.
	const ImVec2 RENDER_POSITION	= { 260.0f, 20.0f };		// 描画座標.
	const char* FILE_PATH = "Data\\GameParam\\Alien\\SpaenParam\\SpaenParam.bin";
public:
	CSpawnEdit();
	~CSpawnEdit();

	// 初期化関数.
	bool Init();
	// 描画関数.
	void Render();

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
	int m_Index;
};

#endif	// #ifndef SPAWN_EDIT_H.