#include "Fade.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/************************************
*	フェードクラス.
**/
CFade::CFade()
	: m_IsFade(false)
	, m_FadeState(EFadeState::Finish)
{
	m_AlphaSpeed = 0.01f;
}

CFade::~CFade()
{
}

// 初期化関数.
bool CFade::Init()
{
	if (GetInstance()->SpriteSetting() == false) return false;
	return true;
}

//	更新関数.
void CFade::Update()
{
	if (GetInstance()->m_pSprite == nullptr) return;
	if (GetInstance()->m_FadeState == EFadeState::Finish) return;

	//フェードイン状態の時.
	if (GetInstance()->m_FadeState == EFadeState::In)
	{
		//透過値が最大値より小さければフェード中.
		if (GetInstance()->m_Alpha < ALPHA_MAX)
		{
			GetInstance()->m_Alpha += m_AlphaSpeed;
		}
		else
		{
			GetInstance()->m_IsFade = false;
		}
	}
	//フェードアウト状態の時.
	else if (GetInstance()->m_FadeState == EFadeState::Out)
	{
		//透過値が最小値より大きければフェード中.
		if (GetInstance()->m_Alpha > 0.0f)
		{
			GetInstance()->m_Alpha -= m_AlphaSpeed;
		}
		else
		{
			GetInstance()->m_IsFade = false;
			GetInstance()->m_FadeState = EFadeState::Finish;
		}
	}
}

// 描画関数.
void CFade::Render()
{
	if (GetInstance()->m_pSprite == nullptr) return;
	// フェードの処理中でなければ通さない.
	if (GetInstance()->m_FadeState == EFadeState::Finish) return;
	GetInstance()->Update();

	GetInstance()->m_pSprite->SetAlpha(GetInstance()->m_Alpha);
	GetInstance()->m_pSprite->SetBlend(true);
	GetInstance()->m_pSprite->SetDeprh(false);
	GetInstance()->m_pSprite->RenderUI();
	GetInstance()->m_pSprite->SetDeprh(true);
	GetInstance()->m_pSprite->SetBlend(false);
}

// フェードイン設定関数.
void CFade::SetFadeIn()
{
	if(GetInstance()->m_FadeState == EFadeState::In) return;

	GetInstance()->m_Alpha = 0.0f;
	GetInstance()->m_IsFade = true;
	GetInstance()->m_FadeState = EFadeState::In;
}

// フェードアウト設定関数.
void CFade::SetFadeOut()
{
	if (GetInstance()->m_FadeState == EFadeState::Out) return;

	GetInstance()->m_Alpha = GetInstance()->ALPHA_MAX;
	GetInstance()->m_IsFade = true;
	GetInstance()->m_FadeState = EFadeState::Out;
}

// スプライト設定関数.
bool CFade::SpriteSetting()
{
	if (GetInstance()->m_pSprite != nullptr) return true;
	GetInstance()->m_pSprite = CSpriteResource::GetSprite("CBackGround");
	if (GetInstance()->m_pSprite == false) return false;

	return true;
}
