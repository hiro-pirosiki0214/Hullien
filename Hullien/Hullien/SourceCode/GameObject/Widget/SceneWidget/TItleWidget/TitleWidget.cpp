#include "TitleWidget.h"
#include "..\..\Cursor\Cursor.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/************************************
*	タイトルUI元クラス.
**/
CTitleWidget::CTitleWidget()
	: m_pSprites		()
	, m_pCursor			( nullptr )
	, m_SelectState	( CTitleWidget::ESelectState::Start )
{
	m_pCursor = std::make_unique<CCursor>();
}

CTitleWidget::~CTitleWidget()
{
}

// 初期化関数.
bool CTitleWidget::Init()
{
	// スプライトの読み込み.
	const char* SpriteName[] =
	{	
		"BackGround",
		"SelectStart",
		"SelectExit",
		"Title",
	};
	int SpriteMax = sizeof(SpriteName) / sizeof(SpriteName[0]);

	m_pSprites.reserve(SpriteMax);
	for (int sprite = 0; sprite < SpriteMax; sprite++)
	{
		m_pSprites.emplace_back(CSpriteResource::GetSprite(SpriteName[sprite]));
		if (m_pSprites[sprite] == nullptr) return false;
	}

	// カーソルの読み込み.
	if(m_pCursor->Init() == false) return false;

	return true;
}

// 更新関数.
void CTitleWidget::Update()
{
	// カーソルの設定.
	SettingCursor();
}

// 描画関数.
void CTitleWidget::Render()
{
	//背景.
	m_pSprites[BACKGROUND]->SetDeprh(false);
	m_pSprites[BACKGROUND]->RenderUI();
	m_pSprites[BACKGROUND]->SetDeprh(true);

	// カーソル.
	m_pCursor->Render();

	// 文字.
	for (size_t sprite = START; sprite < m_pSprites.size(); sprite++)
	{
		m_pSprites[sprite]->SetBlend(true);
		m_pSprites[sprite]->SetDeprh(false);
		m_pSprites[sprite]->RenderUI();
		m_pSprites[sprite]->SetDeprh(true);
	}
}

// カーソル設定関数.
void CTitleWidget::SettingCursor()
{
	if (GetAsyncKeyState( VK_UP ) & 0x0001)
	{
		m_SelectState = CTitleWidget::ESelectState::Start;
	}
	if (GetAsyncKeyState( VK_DOWN ) & 0x0001)
	{
		m_SelectState = CTitleWidget::ESelectState::End;
	}

	switch (m_SelectState)
	{
	case CTitleWidget::ESelectState::Start:
		m_pCursor->SetPosition(m_pSprites[START]->GetRenderPos());
		break;
	case CTitleWidget::ESelectState::End:
		m_pCursor->SetPosition(m_pSprites[END]->GetRenderPos());
		break;
	default:
		break;
	}
	m_pCursor->Update();

}
