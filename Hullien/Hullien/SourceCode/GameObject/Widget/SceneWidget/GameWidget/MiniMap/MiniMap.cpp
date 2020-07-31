#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/**********************************
*	ミニマップクラス.
**/
CMiniMap::CMiniMap()
	: m_pSprite ()
{
}

CMiniMap::~CMiniMap()
{
}

// 初期化関数.
bool CMiniMap::Init()
{
	// スプライト読み込み.
	if ( SpriteSetting() == false ) return false;
	return true;
}

// 更新関数.
void CMiniMap::Update()
{
}

// 描画関数.
void CMiniMap::Render()
{
	for ( const auto& s : m_pSprite )
	{
		s->SetDeprh( false );
		s->RenderUI();
		s->SetDeprh( true );
	}
}

// スプライト設定関数.
bool CMiniMap::SpriteSetting()
{
	if ( m_pSprite.size() != 0 ) return true;

	const char* spriteName[] = 
	{
		"minimapsize",
		"mapiconsize",
	};
	const int spriteMax = sizeof( spriteName ) / sizeof( spriteName[0] );

	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back( CSpriteResource::GetSprite( spriteName[sprite] ) );
		if( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}
