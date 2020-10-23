#include "TitleWidget.h"

#include "..\..\Cursor\Cursor.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"

/************************************
*	タイトルUI元クラス.
**/
CTitleWidget::CTitleWidget()
	: m_pSprite				()
	, m_pCursor				( nullptr )
	, m_SelectState		( CTitleWidget::ESelectState::Start )
	, m_OldSelectState	( CTitleWidget::ESelectState::Start )
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
	m_InputWaitTime--;
}

// 描画関数.
void CTitleWidget::Render()
{
	if (m_pSprite.size() == 0) return;

	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		m_pSprite[sprite]->SetDeprh(false);
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh(true);

		if (sprite != BACKGROUND) continue;
		// カーソル.
		m_pCursor->SetPosition(m_vPosition);
		m_pCursor->Render();
	}
}

// スプライト読み込み関数.
bool CTitleWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_BACKGROUND,		//背景.
		SPRITE_SELECTSTART,		//開始.
		SPRITE_SELECTCONFIG,	//設定.
		SPRITE_SELECTEXIT,		//終了.
		SPRITE_TITLE,			//タイトル.
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
	if( m_InputWaitTime <= 0.0f ){
		int s = static_cast<int>(m_SelectState);
		if (GetAsyncKeyState( VK_UP ) & 0x8000 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX)
		{
			s--;
			m_SelectState = static_cast<CTitleWidget::ESelectState>(s);
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			if( m_SelectState <= CTitleWidget::ESelectState::Start ){
				m_SelectState = CTitleWidget::ESelectState::Start;
				m_InputWaitTime = 0.0f;
			}
		}
		if (GetAsyncKeyState( VK_DOWN ) & 0x8000 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN)
		{
			s++;
			m_SelectState = static_cast<CTitleWidget::ESelectState>(s);
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			if( m_SelectState >= CTitleWidget::ESelectState::End ){
				m_SelectState = CTitleWidget::ESelectState::End;
				m_InputWaitTime = 0.0f;
			}
		}
	}

	switch (m_SelectState)
	{
	case CTitleWidget::ESelectState::Start:
		m_vPosition = m_pSprite[START]->GetRenderPos();
		break;
	case CTitleWidget::ESelectState::Config:
		m_vPosition = m_pSprite[CONFIG]->GetRenderPos();
		break;
	case CTitleWidget::ESelectState::End:
		m_vPosition = m_pSprite[END]->GetRenderPos();
		break;
	default:
		break;
	}

	// SEを鳴らす.
	if (m_SelectState != m_OldSelectState)
	{
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldSelectState = m_SelectState;
	}

	m_pCursor->Update();

}
