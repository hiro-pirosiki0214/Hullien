#include "Slider.h"

#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"


CSlinder::CSlinder()
	: CSlinder	( DEFALUT_VALUE_MAX, DEFALUT_VALUE_MIN )
{
}

CSlinder::CSlinder( const float& valueMax, const float& valueMin )
	: VALUE_MAX					( valueMax )
	, VALUE_MIN					( valueMin )
	, m_pSprites				()
	, m_Value					( valueMax )
	, m_IconPosition			( 0.0f, 0.0f, 0.0f )
	, m_BackBarImageWidthSize	( 0.0f )
	, m_BarImageWidthSize		( 1.0f, 1.0f, 1.0f )
{
}

CSlinder::~CSlinder()
{
}

// 初期化関数.
bool CSlinder::Init()
{
	if( SpriteSetting() == false ) return false;
	// バーの画像幅の取得.
	m_BackBarImageWidthSize = m_pSprites[ESpriteNo_BackBar]->GetSpriteSize().x;
	// バーの描画座標を取得.
	m_vPosition = m_IconPosition = m_pSprites[ESpriteNo_BackBar]->GetRenderPos();

	SetIconPosition();
	return true;
}

// 更新関数.
void CSlinder::Update()
{
	SetIconPosition();
}

// 描画関数.
void CSlinder::Render()
{
	m_pSprites[ESpriteNo_BackBar]->SetPosition( m_vPosition );
	m_pSprites[ESpriteNo_BackBar]->SetScale( m_vScale );
	m_pSprites[ESpriteNo_Bar]->SetPosition( m_vPosition );
	m_pSprites[ESpriteNo_Bar]->SetScale( m_BarImageWidthSize );
	m_pSprites[ESpriteNo_Icon]->SetPosition( m_IconPosition );
	m_pSprites[ESpriteNo_Icon]->SetScale( m_vScale );
	for( auto& s : m_pSprites ){
		s->SetDeprh( false );
		s->RenderUI();
		s->SetDeprh( true );
	}
}

// 値を増やす.
void CSlinder::AddValue( const float& value )
{ 
	m_Value += value;
	m_Value = m_Value >= VALUE_MAX ? VALUE_MAX : m_Value;
}

// 値を減らす.
void CSlinder::SubValue( const float& value )
{ 
	m_Value -= value; 
	m_Value = m_Value <= VALUE_MIN ? VALUE_MIN : m_Value;
}

// 座標設定関数.
void CSlinder::SetPosition( const D3DXVECTOR3& vPos )
{
	m_vPosition = vPos;	// 描画座標を設定して.
	SetIconPosition();	// アイコンの座標も再設定する.
}

// アイコンの座標の設定.
void CSlinder::SetIconPosition()
{
	// 値と計算して、アイコンの位置を設定.
	const float x = m_BackBarImageWidthSize * (m_Value/VALUE_MAX);
	m_BarImageWidthSize.x = x / m_BackBarImageWidthSize;
	m_BarImageWidthSize.x *= m_vScale.x;
	m_BarImageWidthSize.y *= m_vScale.y;
	m_BarImageWidthSize.z *= m_vScale.z;
	m_IconPosition = m_vPosition;
	m_IconPosition.x = m_vPosition.x + x;
}

// スプライト設定関数.
bool CSlinder::SpriteSetting()
{
	const char* spriteNames[] =
	{
		SPRITE_BACK_BAR_NAME,
		SPRITE_BAR_NAME,
		SPRITE_ICON_NAME,
	};
	int SpriteMax = sizeof(spriteNames) / sizeof(spriteNames[0]);

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back( CSpriteResource::GetSprite(spriteNames[sprite]) );
		if( m_pSprites[sprite] == nullptr ) return false;
	}
	return true;
}