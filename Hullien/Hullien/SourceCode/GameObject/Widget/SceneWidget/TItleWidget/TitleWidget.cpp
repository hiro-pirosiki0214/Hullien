#include "TitleWidget.h"

#include "..\..\Cursor\Cursor.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"

/************************************
*	�^�C�g��UI���N���X.
**/
CTitleWidget::CTitleWidget()
	: m_pSprite				()
	, m_pCursor				( nullptr )
	, m_SelectState		( CTitleWidget::ESelectState::Start )
	, m_OldSelectState	( CTitleWidget::ESelectState::Start )
{
	m_pCursor = std::make_shared<CCursor>();
}

CTitleWidget::~CTitleWidget()
{
}

// �������֐�.
bool CTitleWidget::Init()
{
	// �X�v���C�g�̓ǂݍ���.
	if( SpriteSetting() == false ) return false;

	// �J�[�\���̓ǂݍ���.
	if( m_pCursor->Init() == false ) return false;

	return true;
}

// �X�V�֐�.
void CTitleWidget::Update()
{
	// �J�[�\���̐ݒ�.
	if (m_pCursor == nullptr) return;
	CursorSetting();
	m_InputWaitTime--;
}

// �`��֐�.
void CTitleWidget::Render()
{
	if (m_pSprite.size() == 0) return;

	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		m_pSprite[sprite]->SetDeprh(false);
		m_pSprite[sprite]->SetBlend( true );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetBlend( false );
		m_pSprite[sprite]->SetDeprh(true);

		if (sprite != BACKGROUND) continue;
		// �J�[�\��.
		m_pCursor->SetPosition(m_vPosition);
		m_pCursor->Render();
	}
}

// �X�v���C�g�ǂݍ��݊֐�.
bool CTitleWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_BACKGROUND,		//�w�i.
		SPRITE_SELECTSTART,		//�J�n.
#ifndef IS_CONFIG_RENDER
		SPRITE_SELECTCONFIG,	//�ݒ�.
#endif	// #ifndef IS_CONFIG_RENDER.
		SPRITE_SELECTEXIT,		//�I��.
		SPRITE_TITLE,			//�^�C�g��.
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �������̍ő�l�ݒ�.
	m_pSprite.reserve(SpriteMax);
	for (int sprite = 0; sprite < SpriteMax; sprite++)
	{
		m_pSprite.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if (m_pSprite[sprite] == nullptr) return false;
	}

	return true;
}

// �J�[�\���ݒ�֐�.
void CTitleWidget::CursorSetting()
{
	if( m_InputWaitTime <= 0.0f ){
		int s = static_cast<int>(m_SelectState);
		if (GetAsyncKeyState( VK_UP ) & 0x8000 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX)
		{
			s--;
			m_SelectState = static_cast<CTitleWidget::ESelectState>(s);
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			if( m_SelectState <= CTitleWidget::ESelectState::Start ){
				m_SelectState = CTitleWidget::ESelectState::Start;
				m_InputWaitTime = 0.0f;
			}
		}
		if (GetAsyncKeyState( VK_DOWN ) & 0x8000 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN)
		{
			s++;
			m_SelectState = static_cast<CTitleWidget::ESelectState>(s);
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			if( m_SelectState >= CTitleWidget::ESelectState::End ){
				m_SelectState = CTitleWidget::ESelectState::End;
				m_InputWaitTime = 0.0f;
			}
		}
	}

	switch (m_SelectState)
	{
	case CTitleWidget::ESelectState::Start:
		m_vPosition = m_pSprite[START]->GetRenderPos();
		break;
#ifndef IS_CONFIG_RENDER
	case CTitleWidget::ESelectState::Config:
		m_vPosition = m_pSprite[CONFIG]->GetRenderPos();
		break;
#endif	// #ifndef IS_CONFIG_RENDER.
	case CTitleWidget::ESelectState::End:
		m_vPosition = m_pSprite[END]->GetRenderPos();
		break;
	default:
		break;
	}

	// SE��炷.
	if (m_SelectState != m_OldSelectState)
	{
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldSelectState = m_SelectState;
	}

	m_pCursor->Update();

}
