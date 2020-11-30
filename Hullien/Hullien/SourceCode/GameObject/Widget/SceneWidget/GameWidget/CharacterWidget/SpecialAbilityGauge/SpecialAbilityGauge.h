#ifndef SPECIALABILITYGAUGE_H
#define SPECIALABILITYGAUGE_H

#include "..\ChatacterWidget.h"
#include <vector>

/*******************************************
*	����\�̓Q�[�W�N���X.
**/
class CSpecialAbilityGauge : public CCharacterWidget
{
private:
	const char*	SPRITE_GAUGE		= "abilityGauge";	// �Q�[�W.
	const char* SPRITE_GAUGE_NAME	= "ability";
	const int	GAUGE_NUM			= 0;				// �Q�[�W�̔z��ԍ�.
	const int	ONE					= 1;				// ��.
	const int	WAITTIME_MAX		= 100;				//�ő�ҋ@����.
	const float	POS_Y				= 50.0f;			// �Q�[�W�`��ʒuy���W.
	const float	FADE_IN_SPEED		= 0.1f;				//�t�F�[�h�C�����x.
	const float	FADE_OUT_SPEED		= 0.01f;			//�t�F�[�h�A�E�g���x.
	const float	SCALE_MAX			= 1.0f;				//�傫���̍ő�.


public:
	CSpecialAbilityGauge();
	virtual ~CSpecialAbilityGauge();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	// �o�[�����点�鏈��.
	void ShineBar();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprite;			// �X�v���C�g�N���X.
	std::vector<SGaugeState>				m_GaugeState;	//�Q�[�W�̏��.
	std::vector<float>						m_Alpha;				//���ߒl.
	int										m_WaitTime;		//�ҋ@����.
	bool									m_IsOnlyFirst;		//��x������������.

};

#endif	//#ifndef SPECIALABILITYGAUGE_H.