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

// �������֐�.
bool CSlinder::Init()
{
	if( SpriteSetting() == false ) return false;
	// �o�[�̉摜���̎擾.
	m_BackBarImageWidthSize = m_pSprites[ESpriteNo_BackBar]->GetSpriteSize().x;
	// �o�[�̕`����W���擾.
	m_vPosition = m_IconPosition = m_pSprites[ESpriteNo_BackBar]->GetRenderPos();

	SetIconPosition();
	return true;
}

// �X�V�֐�.
void CSlinder::Update()
{
	SetIconPosition();
}

// �`��֐�.
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

// �l�𑝂₷.
void CSlinder::AddValue( const float& value )
{ 
	m_Value += value;
	m_Value = m_Value >= VALUE_MAX ? VALUE_MAX : m_Value;
}

// �l�����炷.
void CSlinder::SubValue( const float& value )
{ 
	m_Value -= value; 
	m_Value = m_Value <= VALUE_MIN ? VALUE_MIN : m_Value;
}

// ���W�ݒ�֐�.
void CSlinder::SetPosition( const D3DXVECTOR3& vPos )
{
	m_vPosition = vPos;	// �`����W��ݒ肵��.
	SetIconPosition();	// �A�C�R���̍��W���Đݒ肷��.
}

// �A�C�R���̍��W�̐ݒ�.
void CSlinder::SetIconPosition()
{
	// �l�ƌv�Z���āA�A�C�R���̈ʒu��ݒ�.
	const float x = m_BackBarImageWidthSize * ((m_Value-VALUE_MIN)/VALUE_MAX);
	m_BarImageWidthSize.x = x / m_BackBarImageWidthSize;
	m_BarImageWidthSize.x *= m_vScale.x;
	m_BarImageWidthSize.y *= m_vScale.y;
	m_BarImageWidthSize.z *= m_vScale.z;
	m_IconPosition = m_vPosition;
	m_IconPosition.x = m_vPosition.x + x;
}

// �X�v���C�g�ݒ�֐�.
bool CSlinder::SpriteSetting()
{
	const char* spriteNames[] =
	{
		SPRITE_BACK_BAR_NAME,
		SPRITE_BAR_NAME,
		SPRITE_ICON_NAME,
	};
	int SpriteMax = sizeof(spriteNames) / sizeof(spriteNames[0]);

	// �������̍ő�l�ݒ�.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back( CSpriteResource::GetSprite(spriteNames[sprite]) );
		if( m_pSprites[sprite] == nullptr ) return false;
	}
	return true;
}