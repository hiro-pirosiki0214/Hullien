#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include "..\ItemList.h"
#include "..\ItemBase.h"
#include "..\EachItemEffectStruct.h"

#include <memory>
#include <vector>

class CActor;

class CItemManager
{
	const char* ITEM_PARAM_FILE_PATH = "Data\\GameParam\\Item\\ItemParam.bin";
	const char* EACH_ITEM_EFFECT_FILE_PATH = "Data\\GameParam\\Item\\EachItemEffect.bin";

public:
	CItemManager();
	~CItemManager();

	// 初期化.
	bool Init();
	// ドロップ.
	void Drop( const DropItemList& dropItemList );
	// 更新関数.
	void Update( std::function<void(CActor*)> updateProc );
	// 描画関数.
	void Render();

private:
	std::vector<std::shared_ptr<CItemBase>> m_ItemList;	// アイテムリスト.
	CItemBase::SParameter	m_ItemParam;		// アイテムのパラメータ.
	SEachItemEffect			m_EachItemEffect;	// 各アイテムの効果.
};

#endif	// #ifndef ITEM_MANAGER_H.