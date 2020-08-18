#include "UltemateSign.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\Actor.h"

/*************************************************
*	アルティメット出現サインクラス.
**/
CUltemateSing::CUltemateSing()
	: m_IsAppUltemate	( false )
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
}

// 描画関数.
void CUltemateSing::Render()
{
	if (m_IsAppUltemate == false) return;
	m_pSprite->SetDeprh(false);
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh(true);
}

// アルティメットが出現しているか.
void CUltemateSing::IsAppUltemate(CActor * pActor)
{
	// アルティメットでなければ処理しない.
	if (pActor->GetObjectTag() != EObjectTag::Alien_D) return;
	// 出現フラグを立てる.
	m_IsAppUltemate = true;
}

// スプライト設定関数.
bool CUltemateSing::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite("ultemate signsize");
	if (m_pSprite == nullptr) return false;

	return true;
}
