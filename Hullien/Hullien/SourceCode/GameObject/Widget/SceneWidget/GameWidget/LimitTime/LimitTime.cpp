#include "LimitTime.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*****************************************
*	�������ԃN���X.
**/
CLimitTime::CLimitTime()
	: m_pTimeSprites		()
	, m_pMiniBackSprites	()
	, m_vTimePosition		()
	, m_vMiniBackPosition	(2)
	, m_vMiniBackScale		(2)
	, m_Anim				()
	, m_FrameCount			( 0 )
	, m_Seconds				( LIMITTIME_MAX )
	, m_IsFinish			( false )
	, m_RotationCount		( 0.0f )
{
}

CLimitTime::~CLimitTime()
{
}

// �������֐�.
bool CLimitTime::Init()
{
	// �X�v���C�g�ǂݍ���.
	if( SpriteSetting() == false ) return false;
	// �ʒu�ݒ�.
	if( PositionSetting() == false ) return false;
	// �A�j���[�V�����ԍ��ǂݍ���.
	if( AnimSetting() == false ) return false;
	m_vMiniBackPosition[0] = MINIBACK_INITPOS;
	m_vMiniBackPosition[1] = MINIBACK_INITPOS;
	return true;
}

// �X�V�֐�.
void CLimitTime::Update()
{
	if (m_pTimeSprites.size() == 0) return;
	CountDown();	//�J�E���g�_�E��.
	MiniBackPositionUpdate();
}

// �`��֐�.
void CLimitTime::Render()
{
	if( m_pTimeSprites.size() == 0 ) return;

	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );

	for (size_t sprite = 0; sprite < m_pMiniBackSprites.size(); sprite++)
	{
		m_pMiniBackSprites[sprite]->SetPosition( m_vMiniBackPosition[sprite] );
		m_pMiniBackSprites[sprite]->SetScale( m_vMiniBackScale[sprite] );
		m_pMiniBackSprites[sprite]->SetDeprh( false );
		m_pMiniBackSprites[sprite]->SetBlend( true );
		m_pMiniBackSprites[sprite]->RenderUI();
		m_pMiniBackSprites[sprite]->SetBlend( false );
		m_pMiniBackSprites[sprite]->SetDeprh( true );
	}

	for (size_t sprite = 0; sprite < m_pTimeSprites.size(); sprite++)
	{
		m_pTimeSprites[sprite]->SetPosition( m_vTimePosition[sprite] );
		m_pTimeSprites[sprite]->SetAnimNumber( m_Anim[sprite] );
		m_pTimeSprites[sprite]->SetDeprh( false );
		m_pTimeSprites[sprite]->SetBlend( true );
		m_pTimeSprites[sprite]->RenderUI();
		m_pTimeSprites[sprite]->SetBlend( false );
		m_pTimeSprites[sprite]->SetDeprh( true );
	}

}

// �X�v���C�g�ݒ�֐�.
bool CLimitTime::SpriteSetting()
{
	if( m_pTimeSprites.size() != 0 ) return true;

	const char* timeSpriteName[] =
	{
		SPRITE_TIME,
		SPRITE_TIME,
	};
	const int timeSpriteMax = sizeof( timeSpriteName ) / sizeof( timeSpriteName[0] );

	for (int sprite = 0; sprite < timeSpriteMax; sprite++)
	{
		m_pTimeSprites.emplace_back( CSpriteResource::GetSprite( timeSpriteName[sprite] ));
		if ( m_pTimeSprites[sprite] == nullptr ) return false;
	}

	const char* miniSpriteName[] =
	{
		SPRITE_TIME_BACK_MINI,
		SPRITE_TIME_BACK_MINI,
	};
	const int miniSpriteMax = sizeof( miniSpriteName ) / sizeof( miniSpriteName[0] );

	for (int sprite = 0; sprite < miniSpriteMax; sprite++)
	{
		m_pMiniBackSprites.emplace_back( CSpriteResource::GetSprite( miniSpriteName[sprite] ));
		if ( m_pMiniBackSprites[sprite] == nullptr ) return false;
	}

	m_pSprite = CSpriteResource::GetSprite( SPRITE_TIME_BACK );
	if ( m_pSprite == nullptr ) return false;


	return true;
}

// �ʒu�ݒ�֐�.
bool CLimitTime::PositionSetting()
{
	if ( m_vTimePosition.size() != 0 ) return true;

	const D3DXVECTOR3 vPos[] =
	{
		{ONETIME_INITPOS},
		{TENTIME_INITPOS},
	};

	for (size_t pos = 0; pos < m_pTimeSprites.size(); pos++)
	{
		m_vTimePosition.emplace_back( vPos[pos] );
		if ( m_vTimePosition[pos] == nullptr ) return false;
	}

	return true;
}

// �A�j���[�V�����ԍ��ݒ�֐�.
bool CLimitTime::AnimSetting()
{
	if ( m_Anim.size() != 0 ) return true;

	for (size_t anim = 0; anim < m_pTimeSprites.size(); anim++)
	{
		m_Anim.emplace_back( 0 );
	}

	return true;
}

// �J�E���g�_�E���֐�.
void CLimitTime::CountDown()
{
	FramesToSeconds();	// �t���[������b�ɕϊ�.

	int Array = 0;
	for (size_t sprite = 0; sprite < m_pTimeSprites.size(); sprite++)
	{
		const int fPow = static_cast<int>(pow(TEN, Array));
		// �A�j���[�V�����ԍ��̐ݒ�.
		m_Anim[sprite] = static_cast<int>( m_Seconds / fPow % ANIM_MAX);

		Array++;
	}

	// �������Ԃ�0�ɂȂ����ꍇ.
	if( m_Seconds == 0 ) m_IsFinish = true;
}

// �t���[������b�ɕϊ��֐�.
void CLimitTime::FramesToSeconds()
{
	m_FrameCount++;

	if ( m_FrameCount % FRAME_DIVISION == 0 )
	{
		m_Seconds--;
	}
}

// �~�j�w�i�̍��W�X�V.
void CLimitTime::MiniBackPositionUpdate()
{	
	m_vMiniBackPosition[0].x = MINIBACK_INITPOS.x + sinf( static_cast<float>(D3DX_PI*0.5) * m_RotationCount ) * MINIBACK_MAX_WIDTH;
	m_vMiniBackPosition[1].x = MINIBACK_INITPOS.x - sinf( static_cast<float>(D3DX_PI*0.5) * m_RotationCount ) * MINIBACK_MAX_WIDTH;
	m_vMiniBackScale[0] = fabsf( sinf( static_cast<float>(D3DX_PI*0.25) * m_RotationCount ) ) + 0.2f;
	m_vMiniBackScale[1] = fabsf( cosf( static_cast<float>(D3DX_PI*0.25) * m_RotationCount ) ) + 0.2f;
	m_RotationCount += MINIBACK_COUNT_ADD;
}