#ifndef WIDGET_H
#define WIDGET_H

#include "..\GameObject.h"

/****************************************
*	UI���N���X.
**/
class CWidget : public CGameObject
{
public:
	// �t�F�[�h�̏��.
	enum class enFadeState
	{
		In,			//�t�F�[�h�C��.
		Out,		//�t�F�[�h�A�E�g.
		Finish,		//�t�F�[�h�I��.
	} typedef EFadeState;

protected:
	const float ALPHA_MAX = 1.0f;

public:
	CWidget();
	virtual ~CWidget();

	// �t�F�[�h�C���ݒ�֐�.
	void SetFadeIn(const float& alphaSpeed );
	// �t�F�[�h�A�E�g�ݒ�֐�.
	void SetFadeOut( const float& alphaSpeed );

protected:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() = 0;
	// �t�F�[�h�C��.
	void FadeIn( float& alpha);
	// �t�F�[�h�A�E�g.
	void FadeOut( float& alpha );
	// �t�F�[�h�X�V�֐�.
	void FadeUpdate( float& alpha );

protected:
	std::shared_ptr<CSprite>	m_pSprite;		//�X�v���C�g�N���X.
	EFadeState					m_FadeState;	//�t�F�[�h���.
	float						m_Alpha;		//���ߒl.
	float						m_AlphaMax;		//���ߍő�l.
	float						m_AlphaSpeed;	//���ߒl���x.

private:
	// ���ߒl�ݒ�֐�.
	void SetAlpha(float& alpha, const float& alphaSpeed);

private:
	EFadeState		m_OldFadeState;		//�ߋ��̃t�F�[�h���.

};

#endif	// #ifndef WIDGET_H.