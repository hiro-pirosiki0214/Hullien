#include "GameOverWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\ReturnTitle\ReturnTitle.h"

/********************************************
*	ゲームオーバーUIクラス.
**/
CGameOverWidget::CGameOverWidget()
	: m_pReturnTitle ( nullptr )
{
	m_pReturnTitle = std::make_unique<CReturnTitle>();
}

CGameOverWidget::~CGameOverWidget()
{
}

//	初期化関数.
bool CGameOverWidget::Init()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite( "gameOver" );
	if (m_pSprite == nullptr) return false;

	if (m_pReturnTitle->Init() == false) return false;

	return true;
}

// 更新関数.
void CGameOverWidget::Update()
{
	m_pReturnTitle->Update();
}

//	描画関数.
void CGameOverWidget::Render()
{
	if (m_pSprite == nullptr) return;
	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );

	m_pReturnTitle->Render();
}

bool CGameOverWidget::SpriteSetting()
{
	return false;
}
