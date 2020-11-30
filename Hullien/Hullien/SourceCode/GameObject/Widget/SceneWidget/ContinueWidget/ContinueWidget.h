#ifndef CONTINUEWIDGET_H
#define CONTINUEWIDGET_H

#include "..\SceneWidget.h"
#include <vector>

/********************************************
*	�R���e�j���[UI�N���X.
**/
class CContinueWidget : public CSceneWidget
{
public:
	const char* SPRITE_BACK				= "CBackGround";	//�w�i
	const char* SPRITE_CONTINUE			= "continue";	//�R���e�j���[.
	const char* SPRITE_YES				= "yes";		//�C�G�X
	const char* SPRITE_NO				= "no";			//�m�[.
	const int BACKGROUND				= 0;				//�w�i�z��ԍ�.
	const int YES						= 1;				//�C�G�X�z��ԍ�.
	const int NO						= 2;				//�m�[�z��ԍ�.
	const float BACKGROUND_ALPHA		= 0.7f;				//�w�i���ߒl.
	const float BACKGROUND_ALPHA_SPEED	= 0.005f;			//�w�i���ߑ��x.
	const float TEXT_ALPHA_SPEED		= 0.005f;			//�������ߑ��x.

																			//�I�����.
	enum class enSelectState
	{
		Yes,		//�C�G�X.
		No,		//�m�[..
	} typedef ESelectState;

	// �`�揇.
	enum class enDrawTurn
	{
		BackGround,	//�w�i.
		Text,				//����.
	}typedef EDrawTurn;

public:
	CContinueWidget();
	virtual ~CContinueWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �I����Ԃ̎擾�֐�.
	inline ESelectState GetSelectState() const { return m_SelectState; }
	// �`�撆���擾�֐�.
	inline bool GetIsDrawing() const { return m_IsDrawing; }

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	// �J�[�\���ݒ�֐�.
	void CursorSetting();
	// �X�v���C�g�̕`��.
	void SpriteDrawing();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprite;				//�X�v���C�g�N���X.
	std::shared_ptr<CCursor>					m_pCursor;				//�J�[�\���N���X.
	float												m_TextAlpha;			//�����̓��ߒl.
	bool												m_IsDrawing;			//�`�撆��.
	ESelectState									m_SelectState;		//�I�����.
	ESelectState									m_OldSelectState;	//�I��O���.
	EDrawTurn										m_DrawTurn;			//�`�揇.
};

#endif	//#ifndef CONTINUEWIDGET_H
