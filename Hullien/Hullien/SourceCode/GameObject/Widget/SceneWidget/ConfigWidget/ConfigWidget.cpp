#include "ConfigWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"	
#include "VolumeBar/VolumeBar.h"

/********************************************
*	設定UIクラス.
**/
CConfigWidget::CConfigWidget()
	: m_pVolumeBars		()	
	, m_pSprites		()
	, m_pCursor			( nullptr )
	, m_CursorPosition	( 0.0f, 0.0f, 0.0f )
	, m_NowSelectVolume	( ESpriteNo_Master )
{
	m_pVolumeBars.resize(3);
	for( auto& v : m_pVolumeBars ) v = std::make_shared<CVolumeBar>();
}

CConfigWidget::~CConfigWidget()
{
}

// 初期化関数.
bool CConfigWidget::Init()
{
	if( SpriteSetting() == false ) return false;
	for( auto& v : m_pVolumeBars ) if( v->Init() == false ) return false;
	m_CursorPosition.x = m_pVolumeBars[m_NowSelectVolume]->GetNowCursorPosition();
	m_CursorPosition.y = m_pSprites[m_NowSelectVolume]->GetRenderPos().y;

	return true;
}

// 更新関数.
void CConfigWidget::Update()
{
	for( auto& v : m_pVolumeBars ) v->Update();
	m_CursorPosition.x = m_pVolumeBars[m_NowSelectVolume]->GetNowCursorPosition();
	m_CursorPosition.y = m_pSprites[m_NowSelectVolume]->GetRenderPos().y;
}

// 描画関数.
void CConfigWidget::Render()
{
	int i = 0;
	for( auto& v : m_pVolumeBars ){
		v->SetPositionY( m_pSprites[i]->GetRenderPos().y );
		v->Render();
		i++;
	}
	for( auto& s : m_pSprites ){
		s->SetDeprh( false );
		s->RenderUI();
		s->SetDeprh( true );
	}

	m_pCursor->SetPosition( m_CursorPosition );
	m_pCursor->SetDeprh( false );
	m_pCursor->RenderUI();
	m_pCursor->SetDeprh( true );
}

// スプライト設定関数.
bool CConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_MASTER_NAME,
		SPRITE_BGM_NAME,
		SPRITE_SE_NAME,
		SPRITE_CURSOR_NAME,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	for (int sprite = 0; sprite < SpriteMax; sprite++)
	{
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if (m_pSprites[sprite] == nullptr) return false;
	}

	if( m_pCursor == nullptr ){
		m_pCursor = CSpriteResource::GetSprite(SPRITE_CURSOR_NAME);
		if( m_pCursor == nullptr ) return false;
		m_CursorPosition = m_pSprites[ESpriteNo_Master]->GetRenderPos();
	}

	return true;
}