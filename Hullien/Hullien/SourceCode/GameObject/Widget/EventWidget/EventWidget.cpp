#include "EventWidget.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	�C�x���gUI���N���X.
**/
CEventWidget::CEventWidget()
	: m_pSkipSprites	()
	, m_IsSkip			( false )
	, m_Alpha			( 0.0f )
{
}

CEventWidget::~CEventWidget()
{
}

// �������֐�
bool CEventWidget::Init()
{
	if( CEventWidget::SpriteSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CEventWidget::Update()
{
	if (m_IsSkip == true) return;

	if(m_Alpha <= 0.01f)	  CEventWidget::SetFadeIn(0.01f);
	if(m_Alpha >= m_AlphaMax) CEventWidget::SetFadeOut(0.01f);

	CEventWidget::FadeUpdate( m_Alpha );
}

// �`��֐�.
void CEventWidget::Render()
{
	if( m_IsSkip == true ) return;

	// �{�^���X�v���C�g�̕`��.
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetAlpha( m_Alpha );
	m_pSprite->SetScale( 0.5f );
	m_pSprite->SetBlend( true );
	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );
	m_pSprite->SetBlend( false);
	// �����X�v���C�g�̕`��.
	for( auto& s : m_pSkipSprites ){
		s->SetAlpha( m_Alpha );
		s->SetBlend( true );
		s->SetDeprh( false );
		s->RenderUI();
		s->SetDeprh( true );
		s->SetBlend( false);
	}
}

// �X�v���C�g�ݒ�֐�.
bool CEventWidget::SpriteSetting()
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
	m_pSkipSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSkipSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSkipSprites[sprite] == nullptr ) return false;
	}
	m_vPosition = m_pSkipSprites[0]->GetRenderPos();
	m_vPosition.x -= m_pSprite->GetSpriteSize().x/2.0f;
	
	return true; 
}
