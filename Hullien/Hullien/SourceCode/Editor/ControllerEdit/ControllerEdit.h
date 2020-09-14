#ifndef CONTROLLER_EDIT_H
#define CONTROLLER_EDIT_H

#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\Utility\XInput\XInput.h"

#include <filesystem>

namespace fs = std::filesystem;

class CControllerEdit
{
private:
	const ImVec2 WINDOW_SIZE		= { 250.0f, 300.0f };	// ウィンドウサイズ.
	const ImVec2 RENDER_POSITION	= { 0.0f, 70.0f };		// 描画座標.
	const char* BUTTON_ITEMS[6]		= { "A", "B", "X", "Y", "Left", "Right" };

public:
	CControllerEdit();
	~CControllerEdit();

	// 描画関数.
	void Render();

private:
	// ラベルの描画.
	void LabelRender( const int& index );

	// ボタン設定の決定.
	void ButtonSettingDetermination();

	// ファイルの読み込み.
	void FileReading();
	// ファイルの書き込み.
	void FileWriting();

private:
	std::vector<fs::path> m_LabelList;
	std::vector<CXInput::enBUTTON_LIST> m_ItemList;
};

#endif	// #ifndef CONTROLLER_EDIT_H.