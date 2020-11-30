#include "Cursor.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/********************************************
*	�J�[�\���N���X.
**/
CCursor::CCursor()
	:	m_pSelectSprite(nullptr)
	,	m_vOldPosition(D3DXVECTOR3( 0.0f, 0.0f ,0.0f ))
	,	m_Acceleration( 0.0f )
{
	m_vScale.x = 0.0f;
}

CCursor::~CCursor()
{
}

// �������֐�.
bool CCursor::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CCursor::Update()
{
	if (m_pSprite == nullptr) return;
	MoveScale();	// �X�P�[���̓���.
}

// �`��֐�.
void CCursor::Render()
{
	if (m_pSprite == nullptr) return;

	m_vPosition.x -= 67.0f;
	m_vPosition.y -= 5.0f;
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetScale( m_vScale );

	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );

	m_SlectPosition = m_vPosition;
	m_SlectPosition.x -= 10.0f;
	m_pSelectSprite->SetPosition( m_SlectPosition );
	m_pSelectSprite->SetDeprh( false );
	m_pSelectSprite->SetBlend( true );
	m_pSelectSprite->RenderUI();
	m_pSelectSprite->SetBlend( false );
	m_pSelectSprite->SetDeprh( true );
}

// �X�v���C�g�ݒ�֐�.
bool CCursor::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_NAME);
	if (m_pSprite == nullptr) return false;
	m_pSelectSprite = CSpriteResource::GetSprite(SPRITE_NAME2);
	if (m_pSelectSprite == nullptr) return false;
	m_SlectPosition = m_pSelectSprite->GetRenderPos();
	return true;
}

// �X�P�[�������֐�.
void CCursor::MoveScale()
{
	// �ړ��O�̍��W�ƌ��݂̍��W���Ⴄ�ꍇ.
	if (m_vPosition != m_vOldPosition)
	{
		// ���l��������.
		m_vScale.x = 0.0f;
		m_Acceleration = 0.0f;
	}

	// �g��l���W���Ȃ�Ώ������Ȃ�.
	if (m_vScale.x >= SCALE_MAX) return;
	// �g��.
	IncreaseScale();
}

// �g��֐�.
void CCursor::IncreaseScale()
{
	m_vOldPosition = m_vPosition;	// ���ݒn��ێ�����.

	m_vScale.x += SCALE_SPEED - m_Acceleration;
	m_Acceleration += ACC_SPEED;

	// �g��l���W���l�ɂȂ�����.
	if ( m_vScale.x >= SCALE_MAX)
	{
		m_vScale.x = SCALE_MAX;
		m_Acceleration = 0.0f;	// �����x�̏�����.
	}
}

