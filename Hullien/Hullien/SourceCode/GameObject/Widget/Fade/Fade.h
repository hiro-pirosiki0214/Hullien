#ifndef FADE_H
#define FADE_H

#include "..\Widget.h"

/************************************
*	�t�F�[�h�N���X.
* (�p�����������A�e�֐���static�ɂ��ĊO���ł��Ăяo����悤�ɂ���).
**/
class CFade
{
	const float ALPHA_MAX = 1.0f;

public:
	// �t�F�[�h�̏��.
	enum class enFadeState
	{
		In,			//�t�F�[�h�C��.
		Out,		//�t�F�[�h�A�E�g.
		Finish,		//�t�F�[�h�I��.
	} typedef EFadeState;

public:
	CFade();
	virtual ~CFade();

	// �C���X�^���X�̐���.
	static CFade* GetInstance()
	{
		static std::unique_ptr<CFade> pInstance = std::make_unique<CFade>();
		return pInstance.get();
	}

	// �������֐�.
	static bool Init();
	//�`��֐�.
	static void Render();
	// �t�F�[�h����.
	static inline bool GetIsFade() { return GetInstance()->m_IsFade; }
	// �t�F�[�h��Ԏ擾�֐�.
	static inline EFadeState GetFadeState() { return GetInstance()->m_FadeState; }
	// �t�F�[�h�C���ݒ�֐�.
	static void SetFadeIn();
	// �t�F�[�h�A�E�g�ݒ�֐�.
	static void SetFadeOut();

private:
	// �X�V�֐�.
	void Update();
	// �X�v���C�g�ݒ�֐�.
	bool SpriteSetting();

private:
	std::shared_ptr<CSprite> m_pSprite;			//�X�v���C�g�N���X.
	EFadeState m_FadeState;						//�t�F�[�h�̏��.
	float m_Alpha;								//���ߒl.
	float m_AlphaSpeed;							//���ߑ��x.
	bool m_IsFade;								//�t�F�[�h����.

	//�R�s�[�E���[�u�R���X�g���N�^�A������Z�q�̍폜.
	CFade(const CFade &)				= delete;
	CFade(CFade &&)						= delete;
	CFade& operator = (const CFade &)	= delete;
	CFade& operator =(CFade &&)			= delete;

};

#endif	//#ifndef FADE_H.