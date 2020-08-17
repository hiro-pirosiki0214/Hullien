#include "LimitTime.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*****************************************
*	制限時間クラス.
**/
CLimitTime::CLimitTime()
	: m_pSprite		()
	, m_vPosition	()
	, m_Anim		()
	, m_FrameCount	( 0 )
	, m_Seconds		( LIMITTIME_MAX )
	, m_IsFinish	( false )
{
}

CLimitTime::~CLimitTime()
{
}

// 初期化関数.
bool CLimitTime::Init()
{
	// スプライト読み込み.
	if( SpriteSetting() == false ) return false;
	// 位置設定.
	if( PositionSetting() == false ) return false;
	// アニメーション番号読み込み.
	if( AnimSetting() == false ) return false;

	return true;
}

// 更新関数.
void CLimitTime::Update()
{
	if (m_pSprite.size() == 0) return;

	CountDown();	//カウントダウン.
}

// 描画関数.
void CLimitTime::Render()
{
	if( m_pSprite.size() == 0 ) return;

	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		m_pSprite[sprite]->SetPosition( m_vPosition[sprite] );
		m_pSprite[sprite]->SetAnimNumber( m_Anim[sprite] );
		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh( true );
	}
}

// スプライト設定関数.
bool CLimitTime::SpriteSetting()
{
	if( m_pSprite.size() != 0 ) return true;

	const char* spriteName[] =
	{
		SPRITE_TIME,
		SPRITE_TIME,
	};
	const int spriteMax = sizeof( spriteName ) / sizeof( spriteName[0] );

	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back( CSpriteResource::GetSprite( spriteName[sprite] ));
		if ( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}

// 位置設定関数.
bool CLimitTime::PositionSetting()
{
	if ( m_vPosition.size() != 0 ) return true;

	const D3DXVECTOR3 vPos[] =
	{
		{ONETIME_INITPOS},
		{TENTIME_INITPOS},
	};

	for (size_t pos = 0; pos < m_pSprite.size(); pos++)
	{
		m_vPosition.emplace_back( vPos[pos] );
		if ( m_vPosition[pos] == nullptr ) return false;
	}

	return true;
}

// アニメーション番号設定関数.
bool CLimitTime::AnimSetting()
{
	if ( m_Anim.size() != 0 ) return true;

	for (size_t anim = 0; anim < m_pSprite.size(); anim++)
	{
		m_Anim.emplace_back( 0 );
	}

	return true;
}

// カウントダウン関数.
void CLimitTime::CountDown()
{
	FramesToSeconds();	// フレームから秒に変換.

	int Array = 0;
	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		const int fPow = static_cast<int>(pow(TEN, Array));
		// アニメーション番号の設定.
		m_Anim[sprite] = static_cast<int>( m_Seconds / fPow % ANIM_MAX);

		Array++;
	}

	// 制限時間が0になった場合.
	if( m_Seconds == 0 ) m_IsFinish = true;
}

// フレームから秒に変換関数.
void CLimitTime::FramesToSeconds()
{
	m_FrameCount++;

	if ( m_FrameCount % FRAME_DIVISION == 0 )
	{
		m_Seconds--;
	}
}


