#include "VolumeBar.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

CVolumeBar::CVolumeBar()
	: m_pSprite			( nullptr )
	, m_CursorPositionX	( 0.0f )
	, m_ImageWidthSize	( 0.0f )
	, m_Volume			( 1.0f )
{
}

CVolumeBar::~CVolumeBar()
{
}

// 初期化関数.
bool CVolumeBar::Init()
{
	if( SpriteSetting() == false ) return false;
	return true;
}

// 更新関数.
void CVolumeBar::Update()
{
	const float x = m_ImageWidthSize*m_Volume;
	m_vScale.x = x / m_ImageWidthSize;
	m_CursorPositionX = m_pSprite->GetRenderPos().x+x;
}

// 描画関数.
void CVolumeBar::Render()
{
	if (m_pSprite == nullptr) return;

	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetScale( m_vScale );
	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );
}

// スプライト設定関数.
bool CVolumeBar::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_NAME);
	if (m_pSprite == nullptr) return false;
	m_vPosition = m_pSprite->GetRenderPos();
	m_ImageWidthSize = m_pSprite->GetSpriteSize().x;

	const float x = m_ImageWidthSize*m_Volume;
	m_vScale.x = x / m_ImageWidthSize;
	m_CursorPositionX = m_pSprite->GetRenderPos().x+x;
	return true;
}