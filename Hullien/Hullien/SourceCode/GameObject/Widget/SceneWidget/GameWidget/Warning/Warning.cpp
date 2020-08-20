#include "Warning.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\Character\Girl\Girl.h"
#include "..\..\..\..\..\Camera\CameraManager\CameraManager.h"

/*************************************
*	警告クラス.
**/
CWarning::CWarning()
	: m_vTargetPos	(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vTargetRot	(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_IsDisp			( false )
	, m_IsUI				( false )
	, m_pCamera		( nullptr )
{
	m_pCamera = std::make_shared<CCameraManager>();
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
	m_vPosition = m_vTargetPos;
	m_vPosition.y = m_vTargetPos.y + ADJ_POS_Y; // 女の子の頭上に設定.
}

// 描画関数.
void CWarning::Render()
{
	if ( m_pSprite == nullptr ) return;
	// 表示フラグが立っていなければ描画しない.
	if (m_IsDisp != true) return;
	m_pSprite->SetPosition( m_vPosition );
	if (m_IsUI == false)
	{
		m_pSprite->Render(true);	// ビルボードにする.
	}
}

// 女の子の状態設定関数.
void CWarning::SetGirlState(CGirl* pGirl)
{
	// 女の子の位置取得.
	m_vTargetPos = pGirl->GetPosition();

	// 女の子が危険な状態か.
	if (pGirl->IsDanger() == true)
	{
		m_IsDisp = true;					//描画フラグを立てる.
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