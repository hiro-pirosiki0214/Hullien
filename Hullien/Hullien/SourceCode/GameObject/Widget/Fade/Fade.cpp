#include "Fade.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/************************************
*	フェードクラス.
**/
CFade::CFade()
	: m_IsFade(true)
{
	m_FadeState = EFadeState::Out;
}

CFade::~CFade()
{
}

// 初期化関数.
bool CFade::Init()
{
	if (SpriteSetting() == false) return false;
	return true;
}

//	更新関数.
void CFade::Update()
{
	if (m_pSprite == nullptr) return;
	FadeUpdate(m_Alpha);

	//フェードイン状態の時.
	if (m_FadeState == EFadeState::In)
	{
		//透過値が最大値より小さければフェード中.
		if (m_Alpha < ALPHA_MAX)
		{
			m_IsFade = true;
		}
		else
		{
			m_IsFade = false;
		}
	}
	//フェードアウト状態の時.
	else if (m_FadeState == EFadeState::Out)
	{
		//透過値が最小値より大きければフェード中.
		if (m_Alpha > 0.0f)
		{
			m_IsFade = true;
		}
		else
		{
			m_IsFade = false;
		}
	}
}

// 描画関数.
void CFade::Render()
{
	if (m_pSprite == nullptr) return;
	m_pSprite->SetAlpha(m_Alpha);

	m_pSprite->SetBlend(true);
	m_pSprite->SetDeprh(false);
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh(true);
	m_pSprite->SetBlend(false);
}

// スプライト設定関数.
bool CFade::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite("CBackGround");
	if (m_pSprite == false) return false;

	return true;
}
