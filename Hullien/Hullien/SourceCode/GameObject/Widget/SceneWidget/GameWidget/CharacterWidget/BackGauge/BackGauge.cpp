#include "BackGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Actor\Character\Character.h"

/*************************************
*	ライフゲージクラス.
**/
CBackGauge::CBackGauge()
{
}

CBackGauge::~CBackGauge()
{
}

// 初期化関数.
bool CBackGauge::Init()
{
	// スプライト読み込み.
	if( SpriteSetting() == false ) return false;

	return true;
}

// 更新関数.
void CBackGauge::Update()
{
}

// 描画関数.
void CBackGauge::Render()
{
	if( m_pSprite == nullptr ) return;
	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );
}

// スプライト読み込み関数.
bool CBackGauge::SpriteSetting()
{
	if( m_pSprite != nullptr ) return true;
	m_pSprite = CSpriteResource::GetSprite( SPRITE_GAUGEBACK );
	if( m_pSprite == nullptr ) return false;

	return true;
}