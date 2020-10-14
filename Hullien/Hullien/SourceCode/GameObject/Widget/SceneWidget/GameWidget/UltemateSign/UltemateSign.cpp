#include "UltemateSign.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"

/*************************************************
*	アルティメット出現サインクラス.
**/
CUltemateSing::CUltemateSing()
	: m_IsAppUltemate	( false )
	, m_ObjCount		( 0 )
{
}

CUltemateSing::~CUltemateSing()
{
}

// 初期化関数.
bool CUltemateSing::Init()
{
	// スプライトの設定.
	if (SpriteSetting() == false) return false;
	return true;
}

// 更新関数.
void CUltemateSing::Update()
{
	if (m_IsAppUltemate == false) return;
	// サイン表示.
	DispSign();
}

// 描画関数.
void CUltemateSing::Render()
{
	if (m_IsAppUltemate == false) return;
	m_pSprite->SetAlpha(m_Alpha);

	m_pSprite->SetBlend( true );
	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );
	m_pSprite->SetBlend( false );
}

// アルティメットが出現しているか.
void CUltemateSing::IsAppUltemate(CGameActorManager* pActor)
{
	auto l = pActor->GetObjPositionList();

	if(m_ObjCount == l.size()) return;
	if (m_ObjCount > l.size()) {
		m_ObjCount = l.size();
		return;
	}
	// すでに処理したオブジェクト分は無視する.
	for (auto a = l.begin() + m_ObjCount; a < l.end(); a++)
	{
		m_ObjCount++;
		// アルティメットでなければ処理しない.
		if (a->first != EObjectTag::Alien_D) continue;
		// 出現フラグを立てる.
		m_IsAppUltemate = true;
	}
}

// スプライト設定関数.
bool CUltemateSing::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite( SPRITE_NAME );
	if (m_pSprite == nullptr) return false;

	return true;
}

// サイン表示関数.
void CUltemateSing::DispSign()
{
	if (m_Alpha < ALPHA_MAX && m_DispTime == 0)
	{
		// フェードインの設定.
		if ( m_FadeState != EFadeState::In ) { SetFadeIn(ALPHA_SPEED); }
	}
	else
	{
		m_DispTime++;

		// 最大表示時間を超えていなければ処理しない.
		if (m_DispTime < DISPTIME_MAX) return;
		// フェードアウトの設定.
		if (m_FadeState != EFadeState::Out) { SetFadeOut(ALPHA_SPEED); }

		// 透過値が0ならば非表示にする.
		if (m_Alpha < 0.0f)
		{
			m_DispTime = 0;
			m_IsAppUltemate = false;
		}
	}

	// フェードの更新.
	FadeUpdate(m_Alpha);
}
