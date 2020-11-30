#ifndef LIFEGAUGE_H
#define LIFEGAUGE_H

#include "..\ChatacterWidget.h"
#include <vector>

/*************************************
*	���C�t�Q�[�W�N���X.
**/
class CLifeGauge : public CCharacterWidget
{
private:
	const char* SPRITE_GAUGE		= "hpGauge";	//�Q�[�W.
	const char* SPRITE_GAUGEDELAY	= "hpGauge";	//�Q�[�W�x��.
	const char* SPRITE_GAUGE_NAME	= "hp";			//HP����.
	const int	HP_NAME_NUM			= 2;		//HP�����̔z��ԍ�.
	const int	GAUGE_NUM			= 1;		//�Q�[�W�̔z��ԍ�.
	const int	GAUGEDELAY_NUM		= 0;		//�Q�[�W�̔z��ԍ�.
	const int	ONE					= 2;		//��.

	inline static float LIFE_DELAY_SUB_VALUE = 0.005f;

public:
	CLifeGauge();
	virtual ~CLifeGauge();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;
	// �p�����[�^�ݒ�֐�.
//	virtual void SetParameter(CCharacter& pChara) override;

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	//�X�v���C�g�N���X.
	std::vector<SGaugeState> m_GaugeState;			//�Q�[�W�̏��.
	float m_OldLife;
	bool	m_IsOldLifeSet;
};

#endif	//#ifndef LIFEGAUGE_H
