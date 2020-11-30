#ifndef SP_EFFECT_TIME_H
#define SP_EFFECT_TIME_H

#include "..\..\ItemBase.h"

class CSPEffectTimeItem : public CItemBase
{
	const char* MODEL_NAME	= "item ability";			// ���f����.
	const char* DROP_AND_AVTIVE_EFFECT_NAME	= "item_effect_ability";	// �h���b�v �A�N�e�B�u���̃G�t�F�N�g��.
	const char* HIT_EFFECT_NAME				= "key";	// �q�b�g���̃G�t�F�N�g��.
	const float* pSP_RECOVERY_VALUE;	// ����\�͉񕜗�(�O������|�C���^�Ŏ擾����).
	const float* pSP_RECOVERY_TIME;		// ����\�͉񕜎���(�O������|�C���^�Ŏ擾����).
public:
	CSPEffectTimeItem();
	CSPEffectTimeItem(
		const float* pSPRecoveryValue,
		const float* pSPRecoveryTime,
		const CItemBase::SParameter* pParam );
	virtual ~CSPEffectTimeItem();

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

#endif	// #ifndef SP_EFFECT_TIME_H.