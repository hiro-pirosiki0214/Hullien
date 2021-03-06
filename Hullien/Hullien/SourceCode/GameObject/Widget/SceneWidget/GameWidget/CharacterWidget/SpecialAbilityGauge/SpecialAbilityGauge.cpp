#include "SpecialAbilityGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\XAudio2\SoundManager.h"

/*******************************************
*	特殊能力ゲージクラス.
**/
CSpecialAbilityGauge::CSpecialAbilityGauge()
	: m_pSprite			()
	, m_GaugeState	()
	, m_Alpha			()
	, m_WaitTime		( 0 )
	, m_IsOnlyFirst		( false )
{
	SetFadeIn(FADE_IN_SPEED);
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
	m_GaugeState[GAUGE_NUM].vScale.x = m_Parameter.SpecialAbility / m_Parameter.SpecialAbilityMax;
	if (m_GaugeState[GAUGE_NUM].vScale.x != SCALE_MAX) m_IsOnlyFirst = false;

	// 特殊能力ゲージがたまっていなければ処理しない.
	if (m_GaugeState[GAUGE_NUM].vScale.x != SCALE_MAX) return;
	// SEを鳴らす..
	if (m_IsOnlyFirst == false)
	{
		CSoundManager::NoMultipleSEPlay("SpecialAbilityMax");
		m_IsOnlyFirst = true;
	}
	// 光らせる処理.
	ShineBar();

}

// 描画関数.
void CSpecialAbilityGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		if (sprite == 1 && m_GaugeState[GAUGE_NUM].vScale.x != SCALE_MAX) continue;

		m_pSprite[sprite]->SetScale(m_GaugeState[sprite].vScale);
		m_pSprite[sprite]->SetAnimNumber(m_GaugeState[sprite].AnimNum);
		m_pSprite[sprite]->SetAlpha(m_Alpha[sprite]);

		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->SetBlend( true );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetBlend( false );
		m_pSprite[sprite]->SetDeprh( true );
	}
}

// スプライト設定関数.
bool CSpecialAbilityGauge::SpriteSetting()
{
	if (m_pSprite.size() != 0) return true;

	//読み込むスプライト名設定.
	const char* spriteName[] =
	{
		SPRITE_GAUGE,		    //ゲージ.
		SPRITE_GAUGE,		    //ゲージ.
		SPRITE_GAUGE_NAME,		//ゲージの名前.
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

	m_GaugeState[0].AnimNum = 0;
	m_GaugeState[1].AnimNum = 1;
	m_GaugeState[2].AnimNum = 1;
	m_Alpha[1] = 0.0f;

	return true;
}

// バーを光らせる処理.
void CSpecialAbilityGauge::ShineBar()
{
	switch (m_FadeState)
	{
	case CWidget::EFadeState::In:
		if (m_Alpha[1] >= ALPHA_MAX) { SetFadeOut(FADE_OUT_SPEED); }
		break;
	case CWidget::EFadeState::Out:
		if (m_Alpha[1] <= 0.0f) { m_FadeState = CWidget::EFadeState::Finish; }
		break;
	case CWidget::EFadeState::Finish:
		m_WaitTime++;

		if (m_WaitTime >= WAITTIME_MAX)
		{
			m_WaitTime = 0;
			SetFadeIn(FADE_IN_SPEED);
		}
		break;
	default:
		break;
	}

	if (m_FadeState == CWidget::EFadeState::Finish) return;
	FadeUpdate(m_Alpha[1]);
}
