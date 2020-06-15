#ifndef ALIEN_PARAM_EDIT_H
#define ALIEN_PARAM_EDIT_H

#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\GameObject\Actor\Character\Alien\Alien.h"

#include <vector>

class CAlienParamEdit
{
	const ImVec2 WINDOW_SIZE = { 560.0f, 450.0f };	// ウィンドウサイズ.
	const ImVec2 RENDER_POSITION = { 720.0f, 20.0f };		// 描画座標.
	const char* FILE_PATH = "Data\\GameParam\\Alien\\AlienParamPathList.txt";
public:
	CAlienParamEdit();
	~CAlienParamEdit();

	// 初期化関数.
	bool Init();
	// 描画関数.
	void Render();

private:
	// 全ファイルの読み込み.
	bool FileAllReading();
	// ファイルの読み込み.
	bool FileReading( const char* fileName, CAlien::SAlienParam& param );
	// ファイルの書き込み.
	bool FileWriting( const char* fileName, CAlien::SAlienParam& param );
	// スポーン情報の描画.
	void SpawnParamRender( const int& index );

private:
	std::vector<CAlien::SAlienParam> m_AlienParamList;
	std::vector<std::string> m_AlienPathList;
	std::vector<std::string> m_AlienNameList;
	int m_Index;

};

#endif	// #ifndef ALIEN_PARAM_EDIT_H.