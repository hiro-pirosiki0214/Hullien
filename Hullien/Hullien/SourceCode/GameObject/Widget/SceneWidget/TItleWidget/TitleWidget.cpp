#include "TitleWidget.h"

#include "..\..\Cursor\Cursor.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/************************************
*	タイトルUI元クラス.
**/
CTitleWidget::CTitleWidget()
	: m_pSprite		()
	, m_pCursor			( nullptr )
	, m_SelectState		( CTitleWidget::ESelectState::Start )
{
	m_pCursor = std::make_shared<CCursor>();
}

CTitleWidget::~CTitleWidget()
{
}

// 初期化関数.
bool CTitleWidget::Init()
{
	// スプライトの読み込み.
	if( SpriteSetting() == false ) return false;

	// カーソルの読み込み.
	if( m_pCursor->Init() == false ) return false;

	return true;
}

// 更新関数.
void CTitleWidget::Update()
{
	// カーソルの設定.
	if (m_pCursor == nullptr) return;
	CursorSetting();
}

// 描画関数.
void CTitleWidget::Render()
{
	if (m_pSprite.size() == 0) return;

	//背景.
	m_pSprite[BACKGROUND]->SetDeprh(false);
	m_pSprite[BACKGROUND]->RenderUI();
	m_pSprite[BACKGROUND]->SetDeprh(true);

	// カーソル.
	m_pCursor->Render();

	// 文字.
	for (size_t sprite = START; sprite < m_pSprite.size(); sprite++)
	{
		m_pSprite[sprite]->SetDeprh(false);
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh(true);
	}
}

// スプライト読み込み関数.
bool CTitleWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_BACKGROUND,		//背景.
		SPRITE_SELECTSTART,		//開始.
		SPRITE_SELECTEXIT,			//終了.
		SPRITE_TITLE,					//タイトル.
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// メモリの最大値設定.
	m_pSprite.reserve(SpriteMax);
	for (int sprite = 0; sprite < SpriteMax; sprite++)
	{
		m_pSprite.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if (m_pSprite[sprite] == nullptr) return false;
	}

	return true;
}

// カーソル設定関数.
void CTitleWidget::CursorSetting()
{
	if (GetAsyncKeyState( VK_UP ) & 0x8000)
	{
		m_SelectState = CTitleWidget::ESelectState::Start;
	}
	if (GetAsyncKeyState( VK_DOWN ) & 0x8000)
	{
		m_SelectState = CTitleWidget::ESelectState::End;
	}

	switch (m_SelectState)
	{
	case CTitleWidget::ESelectState::Start:
		m_pCursor->SetPosition( m_pSprite[START]->GetRenderPos() );
		break;
	case CTitleWidget::ESelectState::End:
		m_pCursor->SetPosition( m_pSprite[END]->GetRenderPos() );
		break;
	default:
		break;
	}
	m_pCursor->Update();

}
