#include "BackGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Actor\Character\Character.h"

/*************************************
*	���C�t�Q�[�W�N���X.
**/
CBackGauge::CBackGauge()
{
}

CBackGauge::~CBackGauge()
{
}

// �������֐�.
bool CBackGauge::Init()
{
	// �X�v���C�g�ǂݍ���.
	if( SpriteSetting() == false ) return false;

	return true;
}

// �X�V�֐�.
void CBackGauge::Update()
{
}

// �`��֐�.
void CBackGauge::Render()
{
	if( m_pSprite == nullptr ) return;
	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );
}

// �X�v���C�g�ǂݍ��݊֐�.
bool CBackGauge::SpriteSetting()
{
	if( m_pSprite != nullptr ) return true;
	m_pSprite = CSpriteResource::GetSprite( SPRITE_GAUGEBACK );
	if( m_pSprite == nullptr ) return false;

	return true;
}