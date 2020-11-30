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
		u8"�񕜃A�C�e��",
		u8"����\�̓A�C�e��",
		u8"�U����UP�A�C�e��",
		u8"�ړ����xUP�A�C�e��"
	};

public:
	CItemEdit();
	virtual ~CItemEdit();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// ���f���̕`��.
	virtual void ModelRender() override;
	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() override;

private:
	// �e�^�O�̕`��.
	void TagRender( const int& index );
	// �p�����[�^�̕`��.
	void ParamRender();

	// �t�@�C���̓ǂݍ���.
	bool FileReading();

private:
	std::unique_ptr<CEditItem>	m_pItem;
	CItemBase::SParameter		m_Prameter;
	SEachItemEffect				m_EachItemEffect;
	int		m_ItemNo;
};

#endif	// #ifndef ITEM_EDIT_H.