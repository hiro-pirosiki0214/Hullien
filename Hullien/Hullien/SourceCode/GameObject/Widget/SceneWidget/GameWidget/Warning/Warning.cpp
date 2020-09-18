#include "Warning.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	警告クラス.
**/
CWarning::CWarning()
	: m_vTargetPos	(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_IsGirlOffScreen	( false )
	, m_IsDispRight		( false )
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
	if (m_IsGirlOffScreen == true ) return;
	m_vPosition = m_vTargetPos;
	m_vPosition.y = m_vTargetPos.y + ADJ_POS_Y; // 女の子の頭上に設定.
}

// 描画関数.
void CWarning::Render()
{
	if ( m_pSprite.size() == 0 ) return;
	// 表示フラグが立っていなければ描画しない.
	for (auto s : m_pSprite)
	{
		s->SetPosition(m_vPosition);
	}

	if (m_IsGirlOffScreen == false)
	{
		// 警告サイン.
		RenderDangerSign();
	}
	else
	{
		// 警告矢印.
		RenderDangerArrow();
	}
}

// スプライトの設定関数.
bool CWarning::SpriteSetting()
{
	if (m_pSprite.size() != 0 ) return true;
	m_pSprite.emplace_back( CSpriteResource::GetSprite( SPRITE_NAME ));
	m_pSprite.emplace_back( CSpriteResource::GetSprite( SPRITEUI_NAME ));
	if (m_pSprite.size() == 0) return false;

	return true;
}

// スプライトの描画関数.
void CWarning::RenderDangerSign()
{
	m_pSprite[DANGER_SIGN]->Render(true);	// ビルボードにする.
}

// スプライトUIの描画関数.
void CWarning::RenderDangerArrow()
{
	if (m_IsDispRight == true)
	{
		// 右に表示されているとき.
		m_vPosition.x = RIGHT_POSITION_X;
		m_vRotation.y = RIGHT_ROTATION;
	}
	else
	{
		// 左に表示されているとき.
		m_vPosition.x = LEFT_POSITION_X;
		m_vRotation.y = LEFT_ROTATION;

	}

	m_vPosition.y = ARROWPOSITION_Y;
	m_pSprite[DANGER_ARROW]->SetRotation(m_vRotation);
	m_pSprite[DANGER_ARROW]->SetDeprh(false);
	m_pSprite[DANGER_ARROW]->RenderUI();
	m_pSprite[DANGER_ARROW]->SetDeprh(true);
}
