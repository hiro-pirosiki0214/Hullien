#ifndef WARNING_H
#define WARNING_H

#include "..\..\SceneWidget.h"
#include <vector>

/*************************************
*	�x���N���X.
**/
class CWarning : public CSceneWidget
{
private:
	const char* SPRITE_NAME			= "dangersignsize";	// �X�v���C�g��.
	const char* SPRITEUI_NAME		= "arrowsize";			// �X�v���C�gUI��.
	const int	DANGER_SIGN			= 0;						// �x���T�C��.
	const int	DANGER_ARROW		= 1;					    // �x�����.
	const float ADJ_POS_Y			= 6.0f;					// y���W����.

	// �x�����.
	const float RIGHT_POSITION_X	= WND_W - 90.0f;		//�E�\����y���W.
	const float LEFT_POSITION_X		= 70.0f;					//���\����y���W.
	const float ARROWPOSITION_Y		= WND_H / 2 - 40;			//����y���W.
	const float RIGHT_ROTATION		= 0.0f;					//�E�\���̉�]�l.
	const float LEFT_ROTATION		= static_cast<float>(D3DXToDegree(180.0));	//���\���̉�]�l.

public:
	CWarning();
	virtual ~CWarning();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;
	// ���̎q�̏�Ԑݒ�֐�.
	virtual void SetPosition(const D3DXVECTOR3& vPos)override { m_vTargetPos = vPos; }
	// ���̎q����ʊO��.
	inline void SetIsGirlOffScreen(const bool& IsOffScreen) { m_IsGirlOffScreen = IsOffScreen; }
	// �����E�ɕ\�����邩.
	inline void SetIsDispRight(const bool& IsDispRight) { m_IsDispRight = IsDispRight; }

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	// �X�v���C�g�̕`��֐�.
	void RenderDangerSign();
	// �X�v���C�gUI�̕`��֐�.
	void RenderDangerArrow();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;					// �X�v���C�g�N���X.
	D3DXVECTOR3								m_vTargetPos;				//�^�[�Q�b�g�ʒu.
	bool												m_IsGirlOffScreen;		//���̎q����ʊO��.
	bool												m_IsDispRight;				//�E�ɕ\�����邩.
};


#endif	//#ifndef WARNING_H.