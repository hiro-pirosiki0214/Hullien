#include "SpecialAbilityGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*******************************************
*	特殊能力ゲージクラス.
**/
CSpecialAbilityGauge::CSpecialAbilityGauge()
	: m_pSprite()
	, m_GaugeState()
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
	m_GaugeState[GAUGE_NUM].vScale.x = m_Parameter.SpecialAbilityGauge / 10.0f; // プレイヤー側からライフ最大数を受け取れるようにしたい.
}

// 描画関数.
void CSpecialAbilityGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		m_pSprite[sprite]->SetPosition(m_GaugeState[sprite].vPosition);
		m_pSprite[sprite]->SetScale(m_GaugeState[sprite].vScale);
		m_pSprite[sprite]->SetAnimNumber(m_GaugeState[sprite].AnimNum);
		m_pSprite[sprite]->SetDeprh(false);
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh(true);
	}
}

// スプライト設定関数.
bool CSpecialAbilityGauge::SpriteSetting()
{
	if (m_pSprite.size() != 0) return true;

	//読み込むスプライト名設定.
	const char* spriteName[] =
	{
		SPRITE_GAUGEBACK,	//ゲージ背景.
		SPRITE_GAUGE,		    //ゲージ.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// 各情報の設定.
	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back();
		m_pSprite[sprite] = CSpriteResource::GetSprite(spriteName[sprite]);
		m_GaugeState.emplace_back();
		m_GaugeState[sprite].AnimNum = (spriteMax - ONE) - sprite;
		if (m_pSprite[sprite] == nullptr) return false;
	}

	return true;
}
