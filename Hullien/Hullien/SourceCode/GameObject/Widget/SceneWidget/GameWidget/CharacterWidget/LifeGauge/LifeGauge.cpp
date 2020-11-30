#include "LifeGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Actor\Character\Character.h"

/*************************************
*	���C�t�Q�[�W�N���X.
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

// �������֐�.
bool CLifeGauge::Init()
{
	// �X�v���C�g�ǂݍ���.
	if (SpriteSetting() == false) return false;

	return true;
}

// �X�V�֐�.
void CLifeGauge::Update()
{
	if ( m_pSprite.size() == 0 ) return;
	// �O��̃��C�t���擾�ł��Ă��Ȃ����.
	if( m_IsOldLifeSet == false ){
		m_OldLife = m_Parameter.LifeMax;	// �����.
		m_IsOldLifeSet = true;
	}
	m_GaugeState[GAUGE_NUM].vScale.x = m_Parameter.Life / m_Parameter.LifeMax;

	// �x���p���C�t�̏���.
	if( m_OldLife - m_Parameter.Life > 0.0f ){
		m_OldLife -= LIFE_DELAY_SUB_VALUE;
		if( m_OldLife <= m_Parameter.Life ) m_OldLife = m_Parameter.Life;
	}
	m_GaugeState[GAUGEDELAY_NUM].vScale.x = m_OldLife / m_Parameter.LifeMax; 
}

// �`��֐�.
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

// �X�v���C�g�ǂݍ��݊֐�.
bool CLifeGauge::SpriteSetting()
{
	if (m_pSprite.size() != 0) return true;

	//�ǂݍ��ރX�v���C�g���ݒ�.
	const char* spriteName[] =
	{
		SPRITE_GAUGEDELAY,	//�Q�[�W�x��.
		SPRITE_GAUGE,		//�Q�[�W.
		SPRITE_GAUGE_NAME,	//HP.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �e���̐ݒ�.
	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back();
		m_pSprite[sprite] = CSpriteResource::GetSprite( spriteName[sprite] );
		m_GaugeState.emplace_back();
		m_GaugeState[sprite].AnimNum = (spriteMax - ONE) - sprite;
		if( sprite == HP_NAME_NUM ) m_GaugeState[sprite].AnimNum = 1;
		if ( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}
