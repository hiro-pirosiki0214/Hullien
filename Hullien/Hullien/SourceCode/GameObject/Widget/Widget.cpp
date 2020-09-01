#include "Widget.h"

#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Common\Sprite\CSprite.h"

/****************************************
*	UI元クラス.
**/
CWidget::CWidget()
	: m_pSprite			(nullptr)
	, m_Alpha			(0.0f)
	, m_AlphaMax		(ALPHA_MAX)
	, m_FadeState		(CWidget::enFadeState::Finish)
	, m_AlphaSpeed	(0.0f)
{
}

CWidget::~CWidget()
{
}

// フェードイン設定関数.
void CWidget::SetFadeIn(const float& alphaSpeed)
{
	m_FadeState = CWidget::enFadeState::In;
	m_AlphaSpeed = alphaSpeed;
}

// フェードアウト設定関数.
void CWidget::SetFadeOut(const float& alphaSpeed)
{
	m_FadeState = CWidget::enFadeState::Out;
	m_AlphaSpeed = -alphaSpeed;
}

// フェードイン関数.
void CWidget::FadeIn(float& alpha)
{
	if (m_FadeState != m_OldFadeState)
	{
		alpha = 0.0f;
		m_OldFadeState = m_FadeState;
	}

	SetAlpha(alpha, m_AlphaSpeed);
}

// フェードアウト関数.
void CWidget::FadeOut(float& alpha)
{
	if (m_FadeState != m_OldFadeState)
	{
		alpha = m_AlphaMax;
		m_OldFadeState = m_FadeState;
	}

	SetAlpha(alpha, m_AlphaSpeed);
}

// フェード更新関数.
void CWidget::FadeUpdate(float & alpha)
{
	if (m_FadeState == EFadeState::In) { FadeIn(alpha); }
	if (m_FadeState == EFadeState::Out) { FadeOut(alpha); }
}

	

//透過値設定関数.
void CWidget::SetAlpha(float & alpha, const float & alphaSpeed)
{
	alpha += alphaSpeed;
}
