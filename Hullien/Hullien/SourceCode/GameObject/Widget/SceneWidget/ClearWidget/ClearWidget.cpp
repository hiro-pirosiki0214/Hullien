#include "ClearWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\ReturnTitle\ReturnTitle.h"

/***********************************************
*	クリアUIクラス.
**/
CClearWidget::CClearWidget()
	: m_pReturTitle				( nullptr )
	, m_SpriteDisappearCount	( SPRITE_DIDSPPEAR_TIME )
	, m_SpriteAlpha				( 1.0f )
{
	m_pReturTitle = std::make_unique<CReturnTitle>();
}

CClearWidget::~CClearWidget()
{
}

// 初期化関数.
bool CClearWidget::Init()
{
	//クリア文字の読み込み.
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite( "clearsize" );
	if ( m_pSprite == nullptr ) return false;

	//タイトルに戻るボタンの読み込み.
	if ( m_pReturTitle->Init() == false ) return false;

	return true;
}

// 更新関数.
void CClearWidget::Update()
{
	if (m_pReturTitle == nullptr) return;
	m_pReturTitle->Update();
}

// 描画関数.
void CClearWidget::Render()
{
	//クリア文字.
	if (m_pSprite == nullptr) return;
	if( m_SpriteDisappearCount <= 10.0f ){
		m_SpriteAlpha -= 0.01f;
		if( m_SpriteAlpha <= 0.0f ){
			m_SpriteAlpha = 0.0f;
		}
	}
	if( m_SpriteDisappearCount >= 0.0f ){
		m_SpriteDisappearCount -= 0.1f;
		m_pSprite->SetAlpha( m_SpriteAlpha );
		m_pSprite->SetBlend( true );
		m_pSprite->SetDeprh( false );
		m_pSprite->RenderUI();
		m_pSprite->SetDeprh( true );
		m_pSprite->SetBlend( false );
	}
	if( m_IsSTGEnd == false ){
		//タイトルに戻るボタン.
		m_pReturTitle->Render();
	}
}

bool CClearWidget::SpriteSetting()
{
	return false;
}
