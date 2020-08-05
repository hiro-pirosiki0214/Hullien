#include "ReturnTitle.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	タイトルに戻るボタンクラス.
**/
CReturnTitle::CReturnTitle()
{
	//フェードイン設定.
	SetFadeIn();
}

CReturnTitle::~CReturnTitle()
{
}

// 初期化関数.
bool CReturnTitle::Init()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_NAME);
	if (m_pSprite == nullptr) return false;
	return true;
}

// 更新関数.
void CReturnTitle::Update()
{
	switch (m_FadeState)
	{
	case CWidget::enFadeState::In:
		 FadeIn(m_Alpha, ALPHA_SPEED);

		if ( m_Alpha >= m_AlphaMax )
		{
			SetFadeOut();
		}
		break;

	case CWidget::enFadeState::Out:
		FadeOut( m_Alpha, ALPHA_SPEED);

		if (m_Alpha <= 0.0f)
		{
			SetFadeIn();
		}
		break;
	default:
		break;
	}
}

// 描画関数.
void CReturnTitle::Render()
{
	if ( m_pSprite == nullptr ) return;

	m_pSprite->SetCoverage( true );
	m_pSprite->SetAlpha( m_Alpha );
	m_pSprite->RenderUI();
	m_pSprite->SetCoverage(false);

}
