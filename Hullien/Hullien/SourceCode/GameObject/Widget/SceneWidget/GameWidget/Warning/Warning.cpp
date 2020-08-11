#include "Warning.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\Actor.h"
#include "..\..\..\..\Actor\Character\Girl\Girl.h"

/*************************************
*	警告クラス.
**/
CWarning::CWarning()
	: m_vPlayePos		(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vPlayeRot		(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vTargetPos	(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vTargetRot	(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_IsDisp			( false )
	, m_IsUI				( false )
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
	m_vTargetRot.y = atan2f(
		m_vTargetPos.x - m_vPlayePos.x,
		m_vTargetPos.z - m_vPlayePos.z );

	if (m_vTargetRot.y > D3DX_PI)
	{
		m_vTargetRot.y = 0.0f;
	}
	else if (m_vTargetRot.y < 0.0f)
	{
		m_vTargetRot.y = D3DX_PI;
	}

	if (m_vTargetRot.y <=  m_vPlayeRot.y
		&& m_vTargetRot.y >=  m_vPlayeRot.y)
	{
		m_vPosition = m_vTargetPos;	
		m_IsUI = false;
	}
	else
	{
		m_vPosition = D3DXVECTOR3(100.0f, 450.0f, 0.0f);
		m_IsUI = true;
	}
}

// 描画関数.
void CWarning::Render()
{
	if ( m_pSprite == nullptr ) return;
	// 表示フラグが立っていなければ描画しない.
//	if (m_IsDisp != true) return;
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetColor(D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	m_pSprite->SetDeprh( false );
	if (m_IsUI == false)
	{
		m_pSprite->Render(true);	// ビルボードにする.
	}
	else
	{
		m_pSprite->RenderUI();
	}
	m_pSprite->SetDeprh( true );
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

// プレイヤー位置取得.
void CWarning::SetPlayerPos(CActor * pActor)
{
	// プレイヤーでなければ処理しない.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;
	m_vPlayePos = pActor->GetPosition();
}

// スプライトの設定関数.
bool CWarning::SpriteSetting()
{
	if ( m_pSprite != nullptr ) return true;
	m_pSprite = CSpriteResource::GetSprite( SPRITE_NAME );
	if (m_pSprite == nullptr) return false;

	return true;
}