#ifndef CLEARWIDGET_H
#define CLEARWIDGET_H

#include "..\SceneWidget.h"
#include <vector>

class CReturnTitle;

/***********************************************
*	�N���AUI�N���X.
**/
class CClearWidget : public CSceneWidget
{
	const float SPRITE_DIDSPPEAR_TIME = 30.0f;
	const char* SPRITE_BUTTON_NAME = "buttonB";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_TITLE_NAME = "skip";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_PUSH_NAME = "hold";	//�X�v���C�g�̃t�@�C����.
public:
	CClearWidget();
	virtual ~CClearWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

	inline bool IsSpriteRenderEnd(){ return m_SpriteAlpha <= 0.0f; }
	inline void SetIsSTGEnd(){ m_IsSTGEnd = true; }

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>> m_pSkipSprites;	// �X�v���C�g�N���X.
	bool	m_IsSTGEnd;
	float	m_SpriteDisappearCount;
	float	m_SpriteAlpha;
};

#endif	//#ifndef CLEARWIDGET_H.
