#include "Fade.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/************************************
*	�t�F�[�h�N���X.
**/
CFade::CFade()
	: m_IsFade(false)
	, m_FadeState(EFadeState::Finish)
{
	m_AlphaSpeed = 0.01f;
}

CFade::~CFade()
{
}

// �������֐�.
bool CFade::Init()
{
	if (GetInstance()->SpriteSetting() == false) return false;
	return true;
}

//	�X�V�֐�.
void CFade::Update()
{
	if (m_pSprite == nullptr) return;
	if (m_FadeState == EFadeState::Finish) return;

	//�t�F�[�h�C����Ԃ̎�.
	if (m_FadeState == EFadeState::In)
	{
		//���ߒl���ő�l��菬������΃t�F�[�h��.
		if (m_Alpha < ALPHA_MAX)
		{
			m_Alpha += m_AlphaSpeed;
		}
		else
		{
			m_IsFade = false;
		}
	}
	//�t�F�[�h�A�E�g��Ԃ̎�.
	else if (m_FadeState == EFadeState::Out)
	{
		//���ߒl���ŏ��l���傫����΃t�F�[�h��.
		if (m_Alpha > 0.0f)
		{
			m_Alpha -= m_AlphaSpeed;
		}
		else
		{
			m_IsFade = false;
			m_FadeState = EFadeState::Finish;
		}
	}
}

// �`��֐�.
void CFade::Render()
{
	if (GetInstance()->m_pSprite == nullptr) return;
	// �t�F�[�h�̏������łȂ���Βʂ��Ȃ�.
	if (GetInstance()->m_FadeState == EFadeState::Finish) return;
	GetInstance()->Update();

	GetInstance()->m_pSprite->SetAlpha(GetInstance()->m_Alpha);
	GetInstance()->m_pSprite->SetBlend(true);
	GetInstance()->m_pSprite->SetDeprh(false);
	GetInstance()->m_pSprite->RenderUI();
	GetInstance()->m_pSprite->SetDeprh(true);
	GetInstance()->m_pSprite->SetBlend(false);
}

// �t�F�[�h�C���ݒ�֐�.
void CFade::SetFadeIn()
{
	if(GetInstance()->m_FadeState == EFadeState::In) return;

	GetInstance()->m_Alpha = 0.0f;
	GetInstance()->m_IsFade = true;
	GetInstance()->m_FadeState = EFadeState::In;
}

// �t�F�[�h�A�E�g�ݒ�֐�.
void CFade::SetFadeOut()
{
	if (GetInstance()->m_FadeState == EFadeState::Out) return;

	GetInstance()->m_Alpha = GetInstance()->ALPHA_MAX;
	GetInstance()->m_IsFade = true;
	GetInstance()->m_FadeState = EFadeState::Out;
}

// �X�v���C�g�ݒ�֐�.
bool CFade::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite("CBackGround");
	if (m_pSprite == false) return false;

	return true;
}
