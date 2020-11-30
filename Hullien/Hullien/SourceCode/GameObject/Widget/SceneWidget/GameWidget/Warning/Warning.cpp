#include "Warning.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	�x���N���X.
**/
CWarning::CWarning()
	: m_vTargetPos	(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_IsGirlOffScreen	( false )
	, m_IsDispRight		( false )
{
}

CWarning::~CWarning()
{
}

// �������֐�.
bool CWarning::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CWarning::Update()
{
	if (m_IsGirlOffScreen == true ) return;
	m_vPosition = m_vTargetPos;
	m_vPosition.y = m_vTargetPos.y + ADJ_POS_Y; // ���̎q�̓���ɐݒ�.
}

// �`��֐�.
void CWarning::Render()
{
	if ( m_pSprite.size() == 0 ) return;
	// �\���t���O�������Ă��Ȃ���Ε`�悵�Ȃ�.
	if (m_IsGirlOffScreen == false)
	{
		// �x���T�C��.
		RenderDangerSign();
	}
	else
	{
		// �x�����.
		RenderDangerArrow();
	}
}

// �X�v���C�g�̐ݒ�֐�.
bool CWarning::SpriteSetting()
{
	if (m_pSprite.size() != 0 ) return true;
	m_pSprite.emplace_back( CSpriteResource::GetSprite( SPRITE_NAME ));
	m_pSprite.emplace_back( CSpriteResource::GetSprite( SPRITEUI_NAME ));
	if (m_pSprite.size() == 0) return false;

	return true;
}

// �X�v���C�g�̕`��֐�.
void CWarning::RenderDangerSign()
{
	m_pSprite[DANGER_SIGN]->SetPosition(m_vPosition);
	m_pSprite[DANGER_SIGN]->SetDeprh(false);
	m_pSprite[DANGER_SIGN]->SetBlend(true);
	m_pSprite[DANGER_SIGN]->Render(true);	// �r���{�[�h�ɂ���.
	m_pSprite[DANGER_SIGN]->SetBlend(false);
	m_pSprite[DANGER_SIGN]->SetDeprh(true);
}

// �X�v���C�gUI�̕`��֐�.
void CWarning::RenderDangerArrow()
{
	if (m_IsDispRight == true)
	{
		// �E�ɕ\������Ă���Ƃ�.
		m_vPosition.x = RIGHT_POSITION_X;
		m_vRotation.y = RIGHT_ROTATION;
	}
	else
	{
		// ���ɕ\������Ă���Ƃ�.
		m_vPosition.x = LEFT_POSITION_X;
		m_vRotation.y = LEFT_ROTATION;

	}

	m_vPosition.y = ARROWPOSITION_Y;
	m_pSprite[DANGER_ARROW]->SetPosition( m_vPosition);
	m_pSprite[DANGER_ARROW]->SetRotation(m_vRotation);
	m_pSprite[DANGER_ARROW]->SetDeprh(false);
	m_pSprite[DANGER_ARROW]->SetBlend(true);
	m_pSprite[DANGER_ARROW]->RenderUI();
	m_pSprite[DANGER_ARROW]->SetBlend(false);
	m_pSprite[DANGER_ARROW]->SetDeprh(true);
}
