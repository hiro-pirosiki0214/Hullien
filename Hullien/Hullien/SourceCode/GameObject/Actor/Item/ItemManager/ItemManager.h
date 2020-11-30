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

	// ������.
	bool Init();
	// �h���b�v.
	void Drop( const DropItemList& dropItemList );
	// �X�V�֐�.
	void Update( std::function<void(CActor*)> updateProc );
	// �`��֐�.
	void Render();
	// �G�t�F�N�g�̕`��.
	void EffectRender();

private:
	std::vector<std::shared_ptr<CItemBase>> m_ItemList;	// �A�C�e�����X�g.
	CItemBase::SParameter	m_ItemParam;		// �A�C�e���̃p�����[�^.
	SEachItemEffect			m_EachItemEffect;	// �e�A�C�e���̌���.
};

#endif	// #ifndef ITEM_MANAGER_H.