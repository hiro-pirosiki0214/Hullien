#ifndef EDIT_ITME_H
#define EDIT_ITME_H

#include "..\ItemBase.h"

class CEditItem : public CItemBase
{
	const char* CURE_MODEL_NAME		= "item cure";		// ���f����.
	const char* ABILITY_MODEL_NAME	= "item ability";	// ���f����.
	const char* ATTACK_MODEL_NAME	= "item attack";	// ���f����.
	const char* SPEED_MODEL_NAME	= "item speed";		// ���f����.

	const char* CURE_EFFECT_NAME	= "item_effect_cure";		// �G�t�F�N�g��.
	const char* ABILITY_EFFECT_NAME	= "item_effect_ability";	// �G�t�F�N�g��.
	const char* ATTACK_EFFECT_NAME	= "item_effect_attack";		// �G�t�F�N�g��.
	const char* SPEED_EFFECT_NAME	= "item_effect_speed";		// �G�t�F�N�g��.
public:
	CEditItem();
	virtual ~CEditItem();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& pActor ) override;

	// �o������.
	virtual void Drop( const D3DXVECTOR3& vPos ) override;

	// �p�����[�^�̐ݒ�.
	void SetParamter( const SParameter& param );
	// �A�C�e���ԍ��̐ݒ�.
	inline void SetItemNo( const int& no ) { m_NowItemNo = no; }

private:
	// ���f���̐ݒ�.
	bool ModelSetting();
	// ���������ۂ̌��ʂ�^����.
	virtual void GiveEffect( CActor* pActor ) override{};
	// �G�t�F�N�g�̐ݒ�.
	virtual bool EffectSetting() override;
	// �����蔻��̐ݒ�.
	virtual bool ColliderSetting() override;

private:
	std::vector<std::shared_ptr<CDX9StaticMesh>>	m_pStaticMeshs;
	SParameter	m_Parameter;
	int			m_NowItemNo;
	float		m_RotatonY;
};

#endif	// #ifndef EDIT_ITME_H.
