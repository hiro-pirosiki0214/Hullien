#include "ReturnTitle.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	�^�C�g���ɖ߂�{�^���N���X.
**/
CReturnTitle::CReturnTitle()
	: m_pSprites	()
{
	//�t�F�[�h�C���ݒ�.
	SetFadeIn(ALPHA_SPEED);
}

CReturnTitle::~CReturnTitle()
{
}

// �������֐�.
bool CReturnTitle::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CReturnTitle::Update()
{
	switch (m_FadeState)
	{
	case CWidget::enFadeState::In:
		if ( m_Alpha >= m_AlphaMax )
		{
			SetFadeOut(ALPHA_SPEED);
		}
		break;

	case CWidget::enFadeState::Out:
		if (m_Alpha <= 0.0f)
		{
			SetFadeIn(ALPHA_SPEED);
		}
		break;
	default:
		break;
	}

	FadeUpdate(m_Alpha);
}

// �`��֐�.
void CReturnTitle::Render()
{
	if ( m_pSprite == nullptr ) return;

	// �{�^���X�v���C�g�̕\��.
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetAlpha(m_Alpha);
	m_pSprite->SetScale( 0.5f );
	m_pSprite->SetBlend( true );
	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );
	m_pSprite->SetBlend( false );
	// �����X�v���C�g�̕\��.
	for( auto& s : m_pSprites ){
		s->SetAlpha(m_Alpha);
		s->SetBlend( true );
		s->SetDeprh( false );
		s->RenderUI();
		s->SetDeprh( true );
		s->SetBlend( false );
	}
}

bool CReturnTitle::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_BUTTON_NAME);
	if (m_pSprite == nullptr) return false;

	const char* spriteName[] =
	{
		SPRITE_TITLE_NAME,
		SPRITE_PUSH_NAME
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �������̍ő�l�ݒ�.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
	}
	m_vPosition = m_pSprites[0]->GetRenderPos();
	m_vPosition.x -= m_pSprite->GetSpriteSize().x/2.0f;
	return true;
}