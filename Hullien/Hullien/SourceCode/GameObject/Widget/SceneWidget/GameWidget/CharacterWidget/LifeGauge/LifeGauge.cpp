#include "LifeGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	ライフゲージクラス.
**/
CLifeGauge::CLifeGauge()
	: m_pSprite ()
{
}

CLifeGauge::~CLifeGauge()
{
}

// 初期化関数.
bool CLifeGauge::Init()
{
	// スプライト読み込み.
	if (SpriteSetting() == false) return false;

	return true;
}

// 更新関数.
void CLifeGauge::Update()
{
	if ( m_pSprite.size() == 0 ) return;
}

// 描画関数.
void CLifeGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (const auto& s : m_pSprite)
	{
		s->SetDeprh( false );
		s->SetPosition( m_vPosition );
		s->RenderUI();
		s->SetDeprh( true );
	}

}

// スプライト読み込み関数.
bool CLifeGauge::SpriteSetting()
{
	if (m_pSprite.size() != 0) return true;

	//読み込むスプライト名設定.
	const char* spriteName[] =
	{
		SPRITE_GAGEBACK,	//ゲージ背景.
		SPRITE_GAGE,		//ゲージ.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back();
		m_pSprite[sprite] = CSpriteResource::GetSprite( spriteName[sprite] );
		if ( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}
