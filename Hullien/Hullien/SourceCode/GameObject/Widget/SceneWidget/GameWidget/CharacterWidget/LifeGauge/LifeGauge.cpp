#include "LifeGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Actor\Character\Character.h"

/*************************************
*	ライフゲージクラス.
**/
CLifeGauge::CLifeGauge()
	: m_pSprite			()
	, m_GaugeState	()
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
	m_GaugeState[1].vScale.x = m_Parameter.Life / 10.0f;
}

// 描画関数.
void CLifeGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	int sprite = 0;
	for (const auto& s : m_pSprite)
	{
		s->SetPosition( m_GaugeState[sprite].vPosition );
		s->SetScale(m_GaugeState[sprite].vScale );
		s->SetAnimNumber(m_GaugeState[sprite].AnimNum);
		s->SetDeprh(false);
		s->RenderUI();
		s->SetDeprh( true );
		sprite++;
	}

}

// パラメータ設定関数.
void CLifeGauge::SetParameter(CCharacter& pChara)
{
}

// スプライト読み込み関数.
bool CLifeGauge::SpriteSetting()
{
	if (m_pSprite.size() != 0) return true;

	//読み込むスプライト名設定.
	const char* spriteName[] =
	{
		SPRITE_GAGEBACK,	//ゲージ背景.
		SPRITE_GAGE,		    //ゲージ.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back();
		m_pSprite[sprite] = CSpriteResource::GetSprite( spriteName[sprite] );
		m_GaugeState.emplace_back();
		m_GaugeState[sprite].AnimNum = 1- sprite;
		if ( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}
