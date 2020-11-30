#ifndef BACK_GAUGE_H
#define BACK_GAUGE_H

#include "..\ChatacterWidget.h"

/*************************************
*	���C�t�Q�[�W�N���X.
**/
class CBackGauge : public CCharacterWidget
{
private:
	const char* SPRITE_GAUGEBACK	= "gaugeWaku";	//�Q�[�W�w�i.

public:
	CBackGauge();
	virtual ~CBackGauge();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
};

#endif	// #ifndef BACK_GAUGE_H.