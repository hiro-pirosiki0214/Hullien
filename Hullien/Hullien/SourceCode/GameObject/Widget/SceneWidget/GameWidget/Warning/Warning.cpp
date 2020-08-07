#include "Warning.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\Character\Girl\Girl.h"

/*************************************
*	警告クラス.
**/
CWarning::CWarning()
	: m_IsDisp( false )
{
}

CWarning::~CWarning()
{
}

// 初期化関数.
bool CWarning::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// 更新関数.
void CWarning::Update()
{
}

// 描画関数.
void CWarning::Render()
{
	if ( m_pSprite == nullptr ) return;
	// 表示フラグが立っていなければ描画しない.
	if (m_IsDisp != true) return;
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetDeprh( false );
	m_pSprite->Render( true );	// ビルボードにする.
	m_pSprite->SetDeprh( true );
}

// 女の子の状態設定関数.
void CWarning::SetGirlState(CGirl* pGirl)
{
	// 女の子が危険な状態か.
	if (pGirl->IsDanger() == true)
	{
		m_vPosition = pGirl->GetPosition();	//女の子の位置取得.
		m_IsDisp = true;								//描画フラグを立てる.
	}
	else
	{
		m_IsDisp = false;
	}
}

// スプライトの設定関数.
bool CWarning::SpriteSetting()
{
	if ( m_pSprite != nullptr ) return true;
	m_pSprite = CSpriteResource::GetSprite( SPRITE_NAME );
	if (m_pSprite == nullptr) return false;

	return true;
}
