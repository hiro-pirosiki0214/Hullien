#ifndef GAMESTART_EVENT_WIDGET_H
#define GAMESTART_EVENT_WIDGET_H

#include "..\EventWidget.h"
#include <vector>

/*****************************************
*	�Q�[���X�^�[�g�V�[����UI�N���X.
**/
class CGameStartEventWidget : public CEventWidget
{
	const char* SPRITE_BUTTON_NAME			= "buttonY";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_PUSH_NAME			= "push";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_EXCLAMATION_NAME		= "exclamation";
	const char* SPRITE_PRESERVE_GIRL_NAME	= "PreserveGirl";

	const int	PUSH_YBUTTON	= 0;		// Y�{�^���w���̔z��ԍ�.
	const int	PRESERVE_GIRL	= 3;		// ���̎q�����w���̔z��ԍ�.
	const float ALPHA_SPEED		= 0.05f;	// ���ߑ��x.
	const float WAITCOUNT_MAX	= 100.0f;	// �ҋ@�J�E���g�ő�.

public:
	// UI�̏��.
	enum class enWidgetState
	{
		None = 0,		// �Ȃ�.

		Push_YButton,	// Y�{�^��������.
		Preserve_Girl,	// ���̎q�����.

		Max,
	}typedef EWidgetState;

public:
	CGameStartEventWidget();
	virtual ~CGameStartEventWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// UI�̏�Ԑݒ�֐�.
	void SetWidgetState(const EWidgetState& state);
	// �`�悪�I��������.
	inline bool IsDispEnd() { return m_IsDispEnd == true; }

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;			// �X�v���C�g�N���X.
	std::vector<bool>						m_IsDisp;			// �`��t���O.
	D3DXVECTOR3								m_ButtonPos;
	EWidgetState							m_WidgetState;		// UI�̏��.
	float									m_Alpha;			// ���ߒl.
	float									m_WaitCount;		// �ҋ@�J�E���g.
	bool									m_IsDispEnd;		// �`��I��.
};

#endif	//#ifndef GAMESTART_EVENT_WIDGET_H.