#include "Cursor.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/********************************************
*	�J�[�\���N���X.
**/
CCursor::CCursor()
	:	m_vOldPosition(D3DXVECTOR3( 0.0f, 0.0f ,0.0f ))
	,	m_Acceleration( 0.0f )
{
	m_vSclae.x = 0.0f;
}

CCursor::~CCursor()
{
}

// �������֐�.
bool CCursor::Init()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_NAME);
	if (m_pSprite == nullptr) return false;

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
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetScale( m_vSclae );

	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );
}

// �X�P�[�������֐�.
void CCursor::MoveScale()
{
	// �ړ��O�̍��W�ƌ��݂̍��W���Ⴄ�ꍇ.
	if (m_vPosition != m_vOldPosition)
	{
		// ���l��������.
		m_vSclae.x = 0.0f;
		m_Acceleration = 0.0f;
	}

	// �g��l���W���Ȃ�Ώ������Ȃ�.
	if (m_vSclae.x >= SCALE_MAX) return;
	// �g��.
	IncreaseScale();
}

// �g��֐�.
void CCursor::IncreaseScale()
{
	m_vOldPosition = m_vPosition;	// ���ݒn��ێ�����.

	m_vSclae.x += SCALE_SPEED - m_Acceleration;
	m_Acceleration += ACC_SPEED;

	// �g��l���W���l�ɂȂ�����.
	if ( m_vSclae.x >= SCALE_MAX)
	{
		m_vSclae.x = SCALE_MAX;
		m_Acceleration = 0.0f;	// �����x�̏�����.
	}
}
