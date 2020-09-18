#ifndef ITEM_EDIT_H
#define ITEM_EDIT_H

#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\GameObject\Actor\Item\ItemBase.h"
#include "..\..\GameObject\Actor\Item\EachItemEffectStruct.h"
#include <vector>
#include <string>

class CItemEdit
{
	const ImVec2 WINDOW_SIZE		= { 510.0f, WND_H-70.0f };	// ウィンドウサイズ.
	const ImVec2 RENDER_POSITION	= { 0.0f, 70.0f };	// 描画座標.
	const char* ITEM_PARAM_FILE_PATH = "Data\\GameParam\\Item\\ItemParam.bin";
	const char* EACH_ITEM_EFFECT_FILE_PATH = "Data\\GameParam\\Item\\EachItemEffect.bin";
	const std::vector<std::string> TAG_LIST =
	{
		"Parameter",
		u8"回復アイテム",
		u8"特殊能力アイテム",
		u8"攻撃力UPアイテム",
		u8"移動速度UPアイテム"
	};

public:
	CItemEdit();
	~CItemEdit();

	// 初期化関数.
	bool Init();
	// 描画関数.
	void Render();

private:
	// 各タグの描画.
	void TagRender( const int& index );
	// パラメータの描画.
	void ParamRender();

	// ファイルの読み込み.
	bool FileReading();

private:
	CItemBase::SParameter m_Prameter;
	SEachItemEffect	m_EachItemEffect;
};

#endif	// #ifndef ITEM_EDIT_H.