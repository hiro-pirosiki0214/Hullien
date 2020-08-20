#include "ContinueWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Cursor\Cursor.h"

/********************************************
*	コンテニューUIクラス.
**/
CContinueWidget::CContinueWidget()
	: m_pSprite			()
	, m_pCursor			( nullptr )
	, m_SelectState	( ESelectState::Yes )
{
	m_pCursor = std::make_shared<CCursor>();
}

CContinueWidget::~CContinueWidget()
{
}

// 初期化関数.
bool CContinueWidget::Init()
{
	// スプライトの読み込み.
	if ( SpriteSetting() == false ) return false;
	// カーソルの読み込み.
	if ( m_pCursor->Init() == false ) return false;

	return true;
}

// 更新関数.
void CContinueWidget::Update()
{
	// カーソルの設定.
	if (m_pCursor == nullptr) return;
	CursorSetting();
}

// 描画関数.
void CContinueWidget::Render()
{
	if ( m_pSprite.size() == 0 )return;

	//背景.
	m_pSprite[BACKGROUND]->SetBlend(true);
	m_pSprite[BACKGROUND]->SetAlpha( BACKGROUND_ALPHA );
	m_pSprite[BACKGROUND]->SetDeprh(false);
	m_pSprite[BACKGROUND]->RenderUI();
	m_pSprite[BACKGROUND]->SetDeprh(true);
	m_pSprite[BACKGROUND]->SetBlend(false);

	// カーソル.
	m_pCursor->Render();

	// 文字.
	for (size_t sprite = YES; sprite < m_pSprite.size(); sprite++)
	{
		m_pSprite[sprite]->SetDeprh(false);
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh(true);
	}
}

// スプライト設定関数.
bool CContinueWidget::SpriteSetting()
{
	if ( m_pSprite.size() != 0 ) return true;

	// 読み込むスプライト名の設定.
	const char* spriteName[] =
	{
		SPRITE_BACK,		//背景.
		SPRITE_YES,			//イエス.
		SPRITE_NO,			//ノー.
		SPRITE_CONTINUE,	//コンテニュー.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// スプライトの設定.
	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if ( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}

// カーソル設定関数.
void CContinueWidget::CursorSetting()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_SelectState = CContinueWidget::ESelectState::Yes;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_SelectState = CContinueWidget::ESelectState::No;
	}

	switch (m_SelectState)
	{
	case CContinueWidget::ESelectState::Yes:
		m_pCursor->SetPosition( m_pSprite[YES]->GetRenderPos() );
		break;
	case CContinueWidget::ESelectState::No:
		m_pCursor->SetPosition( m_pSprite[NO]->GetRenderPos() );
		break;
	}
	m_pCursor->Update();
}
