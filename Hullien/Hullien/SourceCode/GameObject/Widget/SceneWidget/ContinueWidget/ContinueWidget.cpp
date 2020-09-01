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
	, m_TextAlpha		( 0.0f )
	, m_IsDrawing		( true )
	, m_SelectState	( ESelectState::Yes )
	, m_DrawTurn		( EDrawTurn::BackGround )
{
	m_pCursor = std::make_shared<CCursor>();
	m_vSclae = D3DXVECTOR3(0.9f, 0.9f, 0.9f);
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
	switch (m_DrawingState)
	{
	case CSceneWidget::EDrawingState::NowDrawing:
		// スプライトの描画.
		SpriteDrawing();
		break;
	case CSceneWidget::EDrawingState::Finish:
		// カーソルの設定.
		if (m_pCursor == nullptr) return;
		CursorSetting();
		break;
	default:
		break;
	}

}

// 描画関数.
void CContinueWidget::Render()
{
	if ( m_pSprite.size() == 0 )return;

	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		if (sprite == 0)
		{
			// 背景の透過値設定.
			m_pSprite[sprite]->SetAlpha(m_Alpha);
		}
		else
		{
			// 文字の透過値設定.
			m_pSprite[sprite]->SetAlpha(m_TextAlpha);
		}

		m_pSprite[sprite]->SetBlend( true );
		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh( true );
		m_pSprite[sprite]->SetBlend( false );

		if (sprite != 0) continue;
		// カーソル.
		m_pCursor->Render();
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
	// スプライト描画中は処理しない.
	if ( m_IsDrawing == true )return;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_SelectState = CContinueWidget::ESelectState::Yes;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_SelectState = CContinueWidget::ESelectState::No;
	}
	// カーソル位置の設定.
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

// スプライトの描画.
void CContinueWidget::SpriteDrawing()
{
	switch (m_DrawTurn)
	{
	case CContinueWidget::EDrawTurn::BackGround:
		m_Alpha += BACKGROUND_ALPHA_SPEED;

		if (m_Alpha >= BACKGROUND_ALPHA)
		{
			m_Alpha = BACKGROUND_ALPHA;
			m_DrawTurn = CContinueWidget::EDrawTurn::Text;
		}
		break;
	case CContinueWidget::EDrawTurn::Text:
		m_TextAlpha += TEXT_ALPHA_SPEED;

		if (m_TextAlpha >= ALPHA_MAX)
		{
			m_TextAlpha = ALPHA_MAX;
			m_DrawingState = CSceneWidget::EDrawingState::Finish;
			m_IsDrawing = false;
		}
		break;
	default:
		break;
	}
}
