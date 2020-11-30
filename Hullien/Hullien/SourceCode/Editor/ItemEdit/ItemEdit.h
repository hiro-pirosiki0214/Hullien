#ifndef ITEM_EDIT_H
#define ITEM_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Item\ItemBase.h"
#include "..\..\GameObject\Actor\Item\EachItemEffectStruct.h"
#include <vector>
#include <string>

class CEditItem;

class CItemEdit : public CEditBase
{
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
	virtual ~CItemEdit();

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
	// 各タグの描画.
	void TagRender( const int& index );
	// パラメータの描画.
	void ParamRender();

	// ファイルの読み込み.
	bool FileReading();

private:
	std::unique_ptr<CEditItem>	m_pItem;
	CItemBase::SParameter		m_Prameter;
	SEachItemEffect				m_EachItemEffect;
	int		m_ItemNo;
};

#endif	// #ifndef ITEM_EDIT_H.