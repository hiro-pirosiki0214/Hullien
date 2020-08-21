#include "SpecialAbilityGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*******************************************
*	特殊能力ゲージクラス.
**/
CSpecialAbilityGauge::CSpecialAbilityGauge()
	: m_pSprite			()
	, m_GaugeState	()
	, m_Alpha			()
	, m_WaitTime		( 0 )
{
	m_FadeState = CWidget::EFadeState::In;
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
	m_GaugeState[GAUGE_NUM].vScale.x = m_Parameter.SpecialAbilityGauge / 10.0f; 

	// 特殊能力ゲージがたまっていなければ処理しない.
	if (m_GaugeState[GAUGE_NUM].vScale.x != SCALE_MAX) return;
	// 光らせる処理.
	ShineBar();

}

// 描画関数.
void CSpecialAbilityGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		if (sprite == 2 && m_GaugeState[GAUGE_NUM].vScale.x != SCALE_MAX) continue;

		m_pSprite[sprite]->SetPosition(m_GaugeState[sprite].vPosition);
		m_pSprite[sprite]->SetScale(m_GaugeState[sprite].vScale);
		m_pSprite[sprite]->SetAnimNumber(m_GaugeState[sprite].AnimNum);
		m_pSprite[sprite]->SetAlpha(m_Alpha[sprite]);
		m_pSprite[sprite]->SetBlend( true );
		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh( true );
		m_pSprite[sprite]->SetBlend( false );
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
		SPRITE_GAUGE,		    //ゲージ.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// 各情報の設定.
	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back();
		m_pSprite[sprite] = CSpriteResource::GetSprite(spriteName[sprite]);
		m_GaugeState.emplace_back();
		m_GaugeState[sprite].vPosition.y = POS_Y;
		m_Alpha.emplace_back(ALPHA_MAX);
		if (m_pSprite[sprite] == nullptr) return false;
	}

	m_GaugeState[0].AnimNum = 1;
	m_GaugeState[1].AnimNum = 0;
	m_GaugeState[2].AnimNum = 2;
	m_Alpha[2] = 0.0f;

	return true;
}

// バーを光らせる処理.
void CSpecialAbilityGauge::ShineBar()
{
	switch (m_FadeState)
	{
	case CWidget::EFadeState::In:
		CWidget::FadeIn(m_Alpha[2], FADE_IN_SPEED);
		if (m_Alpha[2] >= ALPHA_MAX) { CWidget::SetFadeOut(); }
		break;
	case CWidget::EFadeState::Out:
		CWidget::FadeOut(m_Alpha[2], FADE_OUT_SPEED);
		if (m_Alpha[2] <= 0.0f) { m_FadeState = CWidget::EFadeState::Finish; }
		break;
	case CWidget::EFadeState::Finish:
		m_WaitTime++;

		if (m_WaitTime >= WAITTIME_MAX)
		{
			m_WaitTime = 0;
			CWidget::SetFadeIn();
		}
		break;
	default:
		break;
	}
}
