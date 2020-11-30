#include "GameStartEventWidget.h"

#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*****************************************
*	ゲームスタートシーンのUIクラス.
**/
CGameStartEventWidget::CGameStartEventWidget()
	: m_pSprites	()
	, m_IsDisp		()
	, m_ButtonPos	( 0.0f, 0.0f, 0.0f )
	, m_WidgetState	( EWidgetState::None )
	, m_Alpha		( 0.0f )
	, m_WaitCount	( 0.0f ) 
	, m_IsDispEnd	( false )
{
}

CGameStartEventWidget::~CGameStartEventWidget()
{
}

// 初期化関数.
bool CGameStartEventWidget::Init()
{
	if (CEventWidget::Init() == false) return false;
	if( SpriteSetting() == false ) return false;
	return true;
}

// 更新関数.
void CGameStartEventWidget::Update()
{
	CEventWidget::Update();

	switch (m_WidgetState)
	{
	case CGameStartEventWidget::EWidgetState::None:
		if(m_Alpha == 0.0f) return;

		for (auto d : m_IsDisp)
		{
			if (d == false) continue;
			d = false;
		}
		m_Alpha = 0.0f;
		break;

	case CGameStartEventWidget::EWidgetState::Push_YButton:
		if(m_IsDisp[PUSH_YBUTTON] == false) m_IsDisp[PUSH_YBUTTON] = true;
		if(m_IsDisp[PUSH_YBUTTON+1] == false) m_IsDisp[PUSH_YBUTTON+1] = true;
		if(m_IsDisp[PUSH_YBUTTON+2] == false) m_IsDisp[PUSH_YBUTTON+2] = true;
		if (m_Alpha < ALPHA_MAX) m_Alpha += ALPHA_SPEED;
		else { m_IsDispEnd = true; }
		break;

	case CGameStartEventWidget::EWidgetState::Preserve_Girl:
		if (m_IsDisp[PRESERVE_GIRL] == false) m_IsDisp[PRESERVE_GIRL] = true;

		if (m_WaitCount == 0 && m_Alpha < ALPHA_MAX) m_Alpha += ALPHA_SPEED;
		else {
			m_WaitCount++;

			if (m_WaitCount >= WAITCOUNT_MAX)
			{
				m_Alpha -= ALPHA_SPEED;
				if (m_Alpha <= 0.0f) { m_WidgetState = EWidgetState::None; }
			}
		}
		break;
	default:
		break;
	}
}

// 描画関数.
void CGameStartEventWidget::Render()
{
	for (size_t sprite = 0; sprite < m_pSprites.size(); sprite++)
	{
		// 描画フラグが立っていなければ処理しない.
		if( m_IsDisp[sprite] == false ) continue;

		// 透過値の設定.
		m_pSprites[sprite]->SetAlpha( m_Alpha );
		if( sprite == 0 ){
			m_pSprites[sprite]->SetPosition( m_ButtonPos );
			m_pSprites[sprite]->SetScale( 1.0f );
		}
		// 描画の設定.
		m_pSprites[sprite]->SetBlend( true );
		m_pSprites[sprite]->SetDeprh( false );
		m_pSprites[sprite]->RenderUI();
		m_pSprites[sprite]->SetDeprh( true );
		m_pSprites[sprite]->SetBlend( false );
	}

	CEventWidget::Render();
}

// UIの状態設定関数.
void CGameStartEventWidget::SetWidgetState(const EWidgetState& state)
{
	m_WidgetState = state;
}

// スプライト設定関数.
bool CGameStartEventWidget::SpriteSetting()
{
	if (m_pSprites.size() != 0) return true;
	const char* spriteName[] =
	{
		SPRITE_BUTTON_NAME,
		SPRITE_PUSH_NAME,
		SPRITE_EXCLAMATION_NAME,
		SPRITE_PRESERVE_GIRL_NAME
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
	}
	m_ButtonPos = m_pSprites[1]->GetRenderPos();
	m_ButtonPos.x += m_pSprite->GetSpriteSize().x;

	// 描画フラグの設定.
	if (m_IsDisp.size() != 0) return true;
	for (size_t sprite = 0; sprite < m_pSprites.size(); sprite++)
	{
		m_IsDisp.emplace_back( false );
	}

	if(m_pSprites.size() == 0) return false;
	if(m_IsDisp.size() == 0) return false;
	return true;
}
