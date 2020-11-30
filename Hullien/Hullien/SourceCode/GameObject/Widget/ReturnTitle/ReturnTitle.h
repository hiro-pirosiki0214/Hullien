#ifndef RETURNTITLE_H
#define RETURNTITLE_H

#include "..\Widget.h"

#include <vector>

/*************************************
*	�^�C�g���ɖ߂�{�^���N���X.
**/
class CReturnTitle : public CWidget
{
private:
	const char* SPRITE_BUTTON_NAME = "buttonB";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_TITLE_NAME = "title";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_PUSH_NAME = "pushMini";	//�X�v���C�g�̃t�@�C����.
	const float	 ALPHA_SPEED = 0.01f;				//���ߑ��x.

public:
	CReturnTitle();
	virtual ~ CReturnTitle();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;


private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
};
#endif	//#ifndef RETURNTITLE_H.