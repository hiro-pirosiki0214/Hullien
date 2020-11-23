#include "ClearWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\ReturnTitle\ReturnTitle.h"

/***********************************************
*	クリアUIクラス.
**/
CClearWidget::CClearWidget()
	: m_pSkipSprites			()
	, m_IsSTGEnd				( false )
	, m_SpriteDisappearCount	( SPRITE_DIDSPPEAR_TIME )
	, m_SpriteAlpha				( 1.0f )
{
}

CClearWidget::~CClearWidget()
{
}

// 初期化関数.
bool CClearWidget::Init()
{
	//クリア文字の読み込み.
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite( "gameClear" );
	if ( m_pSprite == nullptr ) return false;
	if( SpriteSetting() == false ) return false;

	return true;
}

// 更新関数.
void CClearWidget::Update()
{
	if(m_Alpha <= 0.01f)	  CClearWidget::SetFadeIn(0.01f);
	if(m_Alpha >= m_AlphaMax) CClearWidget::SetFadeOut(0.01f);

	CClearWidget::FadeUpdate( m_Alpha );
}

// 描画関数.
void CClearWidget::Render()
{
	if (m_pSprite == nullptr) return;
	if( m_SpriteDisappearCount <= 10.0f ){
		m_SpriteAlpha -= 0.01f;
		if( m_SpriteAlpha <= 0.0f ){
			m_SpriteAlpha = 0.0f;
		}
	}
	//クリア文字.
	if( m_SpriteDisappearCount >= 0.0f ){
		m_SpriteDisappearCount -= 0.1f;
		m_pSprite->SetAlpha( m_SpriteAlpha );
		m_pSprite->SetBlend( true );
		m_pSprite->SetDeprh( false );
		m_pSprite->RenderUI();
		m_pSprite->SetDeprh( true );
		m_pSprite->SetBlend( false );
	} else {
		// 文字スプライトの描画.
		int i = 0;
		for( auto& s : m_pSkipSprites ){
			if( i == 1 ){
				s->SetPosition( m_vPosition );
				s->SetScale( 0.5f );
			}
			s->SetAlpha( m_Alpha );
			s->SetBlend( true );
			s->SetDeprh( false );
			s->RenderUI();
			s->SetDeprh( true );
			s->SetBlend( false);
			i++;
		}
	}
}

bool CClearWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_TITLE_NAME,
		SPRITE_BUTTON_NAME,
		SPRITE_PUSH_NAME
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// メモリの最大値設定.
	m_pSkipSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSkipSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSkipSprites[sprite] == nullptr ) return false;
	}
	m_vPosition = m_pSkipSprites[0]->GetRenderPos();
	m_vPosition.x -= m_pSkipSprites[1]->GetSpriteSize().x/2.0f;

	return true; 
}
