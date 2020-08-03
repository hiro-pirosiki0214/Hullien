#include "SpecialAbilityGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*******************************************
*	特殊能力ゲージクラス.
**/
CSpecialAbilityGauge::CSpecialAbilityGauge()
	: m_pSprite()
{
}

CSpecialAbilityGauge::~CSpecialAbilityGauge()
{
}

// 初期化関数.
bool CSpecialAbilityGauge::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// 更新関数.
void CSpecialAbilityGauge::Update()
{
	if (m_pSprite.size() == 0) return;
}

// 描画関数.
void CSpecialAbilityGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	m_vPosition = D3DXVECTOR3(0.0f, 100.0f, 0.0f);

	for (const auto& s : m_pSprite)
	{
		s->SetPosition(m_vPosition);
		s->SetDeprh(false);
		s->RenderUI();
		s->SetDeprh(true);
	}
}

// スプライト設定関数.
bool CSpecialAbilityGauge::SpriteSetting()
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
		if (m_pSprite[sprite] == nullptr) return false;
	}

	return true;
}
