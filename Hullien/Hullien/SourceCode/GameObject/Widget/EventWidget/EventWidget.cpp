#include "EventWidget.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	イベントUI元クラス.
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

// 初期化関数
bool CEventWidget::Init()
{
	if( CEventWidget::SpriteSetting() == false ) return false;
	return true;
}

// 更新関数.
void CEventWidget::Update()
{
	if (m_IsSkip == true) return;

	if(m_Alpha <= 0.01f)	  CEventWidget::SetFadeIn(0.01f);
	if(m_Alpha >= m_AlphaMax) CEventWidget::SetFadeOut(0.01f);

	CEventWidget::FadeUpdate( m_Alpha );
}

// 描画関数.
void CEventWidget::Render()
{
	if( m_IsSkip == true ) return;

	// ボタンスプライトの描画.
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetAlpha( m_Alpha );
	m_pSprite->SetBlend( true );
	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );
	m_pSprite->SetBlend( false);
	// 文字スプライトの描画.
	for( auto& s : m_pSkipSprites ){
		s->SetAlpha( m_Alpha );
		s->SetBlend( true );
		s->SetDeprh( false );
		s->RenderUI();
		s->SetDeprh( true );
		s->SetBlend( false);
	}
}

// スプライト設定関数.
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

	// メモリの最大値設定.
	m_pSkipSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSkipSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSkipSprites[sprite] == nullptr ) return false;
	}
	m_vPosition = m_pSkipSprites[1]->GetRenderPos();
	m_vPosition.x -= m_pSprite->GetSpriteSize().x;
	
	return true; 
}
