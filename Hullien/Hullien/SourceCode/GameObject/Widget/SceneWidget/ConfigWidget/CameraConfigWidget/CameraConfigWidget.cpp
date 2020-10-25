#include "CameraConfigWidget.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Cursor\Cursor.h"

CCameraConfigWidget::CCameraConfigWidget()
	: m_pSprites	()
	, m_pCursor		( nullptr )
{
	m_pCursor = std::make_unique<CCursor>();
}

CCameraConfigWidget::~CCameraConfigWidget()
{
}

// 初期化関数.
bool CCameraConfigWidget::Init()
{
	if( SpriteSetting() == false ) return false;
	if( m_pCursor->Init() == false ) return false;
	return true;
}

// 更新関数.
void CCameraConfigWidget::Update()
{
	m_pCursor->Update();
}

// 描画関数.
void CCameraConfigWidget::Render()
{
	m_pCursor->SetPosition( m_pSprites[0]->GetRenderPos() );
	m_pCursor->Render();
	for( auto& s : m_pSprites ){
		s->SetDeprh(false);
		s->RenderUI();
		s->SetDeprh(false);
	}
}

// スプライト設定関数.
bool CCameraConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_CONTROL_NAME,
		SPRITE_SPEED_NAME,
		SPRITE_CHOICE_NAME,
		SPRITE_NORMAL_NAME,
		SPRITE_INVERSION_NAME,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
	}
	return true;
}