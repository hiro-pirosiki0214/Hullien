#include "LifeGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Actor\Character\Character.h"

/*************************************
*	ライフゲージクラス.
**/
CLifeGauge::CLifeGauge()
	: m_pSprite			()
	, m_GaugeState		()
	, m_OldLife			( 0.0f )
	, m_IsOldLifeSet	( false )
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
	// 前回のライフが取得できていなければ.
	if( m_IsOldLifeSet == false ){
		m_OldLife = m_Parameter.LifeMax;	// 入れる.
		m_IsOldLifeSet = true;
	}
	m_GaugeState[GAUGE_NUM].vScale.x = m_Parameter.Life / m_Parameter.LifeMax;

	// 遅延用ライフの処理.
	if( m_OldLife - m_Parameter.Life > 0.0f ){
		m_OldLife -= LIFE_DELAY_SUB_VALUE;
		if( m_OldLife <= m_Parameter.Life ) m_OldLife = m_Parameter.Life;
	}
	m_GaugeState[GAUGEDELAY_NUM].vScale.x = m_OldLife / m_Parameter.LifeMax; 
}

// 描画関数.
void CLifeGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
//		m_pSprite[sprite]->SetPosition( m_GaugeState[sprite].vPosition );
		m_pSprite[sprite]->SetScale( m_GaugeState[sprite].vScale );
		m_pSprite[sprite]->SetAnimNumber( m_GaugeState[sprite].AnimNum );
		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->SetBlend( true );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetBlend( false );
		m_pSprite[sprite]->SetDeprh( true );
	}
}

// スプライト読み込み関数.
bool CLifeGauge::SpriteSetting()
{
	if (m_pSprite.size() != 0) return true;

	//読み込むスプライト名設定.
	const char* spriteName[] =
	{
		SPRITE_GAUGEDELAY,	//ゲージ遅延.
		SPRITE_GAUGE,		//ゲージ.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// 各情報の設定.
	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back();
		m_pSprite[sprite] = CSpriteResource::GetSprite( spriteName[sprite] );
		m_GaugeState.emplace_back();
		m_GaugeState[sprite].AnimNum = (spriteMax - ONE) - sprite;
		if ( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}
