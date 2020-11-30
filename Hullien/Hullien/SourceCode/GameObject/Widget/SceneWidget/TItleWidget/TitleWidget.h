#ifndef CTITLEWIDGET_H
#define CTITLEWIDGET_H

#include "..\SceneWidget.h"
#include <vector>

//#define IS_CONFIG_RENDER

/************************************
*	�^�C�g��UI�N���X.
**/
class CTitleWidget : public CSceneWidget
{
private:
	const char* SPRITE_BACKGROUND	= "BackGround";	//�w�i�X�v���C�g��.
	const char* SPRITE_SELECTSTART	= "start";	//�J�n�X�v���C�g��.
	const char* SPRITE_SELECTCONFIG	= "Config";		//�ݒ�X�v���C�g��.
	const char* SPRITE_SELECTEXIT	= "exit";	  	//�I���X�v���C�g��.
	const char* SPRITE_TITLE		= "hulianTitle";	//�^�C�g���X�v���C�g��.

	const int BACKGROUND	= 0;	//�w�i�z��ԍ�.
	const int START			= 1;	//�J�n�z��ԍ�.
#ifndef IS_CONFIG_RENDER
	const int CONFIG		= 2;	//�J�n�z��ԍ�.
	const int END			= 3;	//�I���z��ԍ�.
#else	// #ifndef IS_CONFIG_RENDER.
	const int END			= 2;	//�I���z��ԍ�.
#endif	// #ifndef IS_CONFIG_RENDER.

public:
	//�I�����.
	enum class enSelectState
	{
		Start,		//�J�n.
#ifndef IS_CONFIG_RENDER
		Config,		//�ݒ�.
#endif	// #ifndef IS_CONFIG_RENDER.
		End,		//�I��.
	} typedef ESelectState;

public:
	CTitleWidget();
	virtual ~CTitleWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

	// �I����Ԃ̎擾�֐�.
	inline ESelectState GetSelectState() const { return m_SelectState; }

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	// �J�[�\���ݒ�֐�.
	void CursorSetting();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprite;				//�X�v���C�g�N���X.
	std::shared_ptr<CCursor>					m_pCursor;				//�J�[�\���N���X.
	ESelectState									m_SelectState;		//�I�����.
	ESelectState									m_OldSelectState;	//�I�����.

};

#endif	//#ifndef CTITLEWIDGET_H
