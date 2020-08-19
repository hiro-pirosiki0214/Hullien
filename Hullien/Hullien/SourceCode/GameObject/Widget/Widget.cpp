#include "Widget.h"

#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Common\Sprite\CSprite.h"

/****************************************
*	UI元クラス.
**/
CWidget::CWidget()
	: m_pSprite(nullptr)
	, m_Alpha(0.0f)
	, m_AlphaMax(ALPHA_MAX)
	, m_FadeState(CWidget::enFadeState::Finish)
{
}

CWidget::~CWidget()
{
}

// フェードイン設定関数.
void CWidget::SetFadeIn()
{
	m_FadeState = CWidget::enFadeState::In;
}

// フェードアウト設定関数.
void CWidget::SetFadeOut()
{
	m_FadeState = CWidget::enFadeState::Out;
}

// フェードイン関数.
void CWidget::FadeIn(float& alpha, const float& alphaSpeed)
{
	if (m_FadeState != m_OldFadeState)
	{
		alpha = 0.0f;
		m_OldFadeState = m_FadeState;
	}

	SetAlpha(alpha, alphaSpeed);
}

// フェードアウト関数.
void CWidget::FadeOut(float& alpha, const float& alphaSpeed)
{
	if (m_FadeState != m_OldFadeState)
	{
		alpha = m_AlphaMax;
		m_OldFadeState = m_FadeState;
	}

	SetAlpha(alpha, -alphaSpeed);
}

// フェード更新関数.
void CWidget::FadeUpdate(float & alpha, const float & alphaSpeed)
{
	if (m_FadeState == EFadeState::In) { FadeIn(alpha, alphaSpeed); }
	if (m_FadeState == EFadeState::Out) { FadeOut(alpha, alphaSpeed); }
}

	

//透過値設定関数.
void CWidget::SetAlpha(float & alpha, const float & alphaSpeed)
{
	alpha += alphaSpeed;
}
