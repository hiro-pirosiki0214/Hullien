#include "GameStartEventWidget.h"

#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*****************************************
*	ゲームスタートシーンのUIクラス.
**/
CGameStartEventWidget::CGameStartEventWidget()
	: m_pSprite		()
	, m_IsDisp		()
	, m_WidgetState	( EWidgetState::None )
	, m_Alpha		( 0.0f )
	, m_WaitCount	( 0.0f ) 
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
		if (m_Alpha < ALPHA_MAX) m_Alpha += ALPHA_SPEED;
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
	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		// 描画フラグが立っていなければ処理しない.
		if( m_IsDisp[sprite] == false ) continue;

		// 透過値の設定.
		m_pSprite[sprite]->SetAlpha( m_Alpha );

		// 描画の設定.
		m_pSprite[sprite]->SetBlend( true );
		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh( true );
		m_pSprite[sprite]->SetBlend( false );
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
	if (m_pSprite.size() != 0) return true;
	// 画像の設定.
	m_pSprite.emplace_back(CSpriteResource::GetSprite("PushYButton"));
	m_pSprite.emplace_back(CSpriteResource::GetSprite("PreserveGirl"));
	// 描画フラグの設定.
	if (m_IsDisp.size() != 0) return true;
	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		m_IsDisp.emplace_back( false );
	}

	if(m_pSprite.size() == 0) return false;
	if(m_IsDisp.size() == 0) return false;
	return true;
}
