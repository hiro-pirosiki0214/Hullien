#include "ReturnTitle.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	タイトルに戻るボタンクラス.
**/
CReturnTitle::CReturnTitle()
{
	//フェードイン設定.
	SetFadeIn(ALPHA_SPEED);
}

CReturnTitle::~CReturnTitle()
{
}

// 初期化関数.
bool CReturnTitle::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// 更新関数.
void CReturnTitle::Update()
{
	switch (m_FadeState)
	{
	case CWidget::enFadeState::In:
		if ( m_Alpha >= m_AlphaMax )
		{
			SetFadeOut(ALPHA_SPEED);
		}
		break;

	case CWidget::enFadeState::Out:
		if (m_Alpha <= 0.0f)
		{
			SetFadeIn(ALPHA_SPEED);
		}
		break;
	default:
		break;
	}

	FadeUpdate(m_Alpha);
}

// 描画関数.
void CReturnTitle::Render()
{
	if ( m_pSprite == nullptr ) return;

	m_pSprite->SetAlpha(m_Alpha);

	m_pSprite->SetBlend( true );
	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );
	m_pSprite->SetBlend( false );
}

bool CReturnTitle::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_NAME);
	if (m_pSprite == nullptr) return false;

	return true;
}
