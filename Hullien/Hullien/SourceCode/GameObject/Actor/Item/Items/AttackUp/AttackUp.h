#ifndef ATTACK_UP_H
#define ATTACK_UP_H

#include "..\..\ItemBase.h"

class CAttackUpItem : public CItemBase
{
	const char* MODEL_NAME	= "item attack";	// ���f����.
	const char* DROP_AND_AVTIVE_EFFECT_NAME	= "item_effect_attack";	// �G�t�F�N�g��.
	const char* HIT_EFFECT_NAME				= "key";	// �G�t�F�N�g��.
	const float* pATTACK_UP_POWER;	// �U����(�O������|�C���^�Ŏ擾����).
	const float* pATTACK_UP_TIME;	// �U����(�O������|�C���^�Ŏ擾����).

public:
	CAttackUpItem();
	CAttackUpItem(
		const float* pAttackUpPower,
		const float* pAttackUpTime,
		const CItemBase::SParameter* pParam );
	virtual ~CAttackUpItem();

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

private:
	// ���������ۂ̌��ʂ�^����.
	virtual void GiveEffect( CActor* pActor ) override;
	// �G�t�F�N�g�̐ݒ�.
	virtual bool EffectSetting() override;
};

#endif	// #ifndef ATTACK_UP_H.