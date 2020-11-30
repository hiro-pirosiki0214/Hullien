#ifndef EVENT_WIDGET_H
#define EVENT_WIDGET_H

#include "..\Widget.h"

#include <vector>

/*************************************
*	�C�x���gUI���N���X.
**/
class CEventWidget : public CWidget
{
	const char* SPRITE_BUTTON_NAME = "buttonB";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_TITLE_NAME = "skip";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_PUSH_NAME = "hold";	//�X�v���C�g�̃t�@�C����.
public:
	CEventWidget();
	virtual ~CEventWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �X�L�b�v�������ݒ�֐�.
	inline void SetSkip(const bool& skip) { m_IsSkip = skip; }

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSkipSprites;	// �X�v���C�g�N���X.
	bool					 m_IsSkip;	// �X�L�b�v������.
	float					 m_Alpha;	// ���ߒl.
};

#endif	//#ifndef EVENT_WIDGET_H.
