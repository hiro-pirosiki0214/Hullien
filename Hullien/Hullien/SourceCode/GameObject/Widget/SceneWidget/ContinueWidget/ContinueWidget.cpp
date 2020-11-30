#include "ContinueWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Cursor\Cursor.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"	

/********************************************
*	�R���e�j���[UI�N���X.
**/
CContinueWidget::CContinueWidget()
	: m_pSprite				()
	, m_pCursor				( nullptr )
	, m_TextAlpha			( 0.0f )
	, m_IsDrawing			( true )
	, m_SelectState			( ESelectState::Yes )
	, m_OldSelectState		( ESelectState::Yes )
	, m_DrawTurn			( EDrawTurn::BackGround )
{
	m_pCursor = std::make_shared<CCursor>();
	m_vScale = D3DXVECTOR3(0.9f, 0.9f, 0.9f);
}

CContinueWidget::~CContinueWidget()
{
}

// �������֐�.
bool CContinueWidget::Init()
{
	// �X�v���C�g�̓ǂݍ���.
	if ( SpriteSetting() == false ) return false;
	// �J�[�\���̓ǂݍ���.
	if ( m_pCursor->Init() == false ) return false;

	return true;
}

// �X�V�֐�.
void CContinueWidget::Update()
{
	switch (m_DrawingState)
	{
	case CSceneWidget::EDrawingState::NowDrawing:
		// �X�v���C�g�̕`��.
		SpriteDrawing();
		break;
	case CSceneWidget::EDrawingState::Finish:
		// �J�[�\���̐ݒ�.
		if (m_pCursor->Init() == false) return;
		CursorSetting();
		break;
	default:
		break;
	}

}

// �`��֐�.
void CContinueWidget::Render()
{
	if ( m_pSprite.size() == 0 )return;

	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		if (sprite == 0)
		{
			// �w�i�̓��ߒl�ݒ�.
			m_pSprite[sprite]->SetAlpha(m_Alpha);
		}
		else
		{
			// �����̓��ߒl�ݒ�.
			m_pSprite[sprite]->SetAlpha(m_TextAlpha);
		}

		m_pSprite[sprite]->SetBlend( true );
		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh( true );
		m_pSprite[sprite]->SetBlend( false );

		if (sprite != 0) continue;
		// �J�[�\��.
		m_pCursor->SetPosition(m_vPosition);
		m_pCursor->Render();
	}
}

// �X�v���C�g�ݒ�֐�.
bool CContinueWidget::SpriteSetting()
{
	if ( m_pSprite.size() != 0 ) return true;

	// �ǂݍ��ރX�v���C�g���̐ݒ�.
	const char* spriteName[] =
	{
		SPRITE_BACK,		//�w�i.
		SPRITE_YES,			//�C�G�X.
		SPRITE_NO,			//�m�[.
		SPRITE_CONTINUE,	//�R���e�j���[.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �X�v���C�g�̐ݒ�.
	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if ( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}

// �J�[�\���ݒ�֐�.
void CContinueWidget::CursorSetting()
{
	// �X�v���C�g�`�撆�͏������Ȃ�.
	if ( m_IsDrawing == true )return;

	if (GetAsyncKeyState(VK_UP) & 0x8000
		|| CXInput::LThumbY_Axis() > IDLE_THUMB_MAX)
	{
		m_SelectState = CContinueWidget::ESelectState::Yes;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000
		|| CXInput::LThumbY_Axis() < IDLE_THUMB_MIN)
	{
		m_SelectState = CContinueWidget::ESelectState::No;
	}
	// �J�[�\���ʒu�̐ݒ�.
	switch (m_SelectState)
	{
	case CContinueWidget::ESelectState::Yes:
		m_vPosition = m_pSprite[YES]->GetRenderPos();
		break;
	case CContinueWidget::ESelectState::No:
		m_vPosition = m_pSprite[NO]->GetRenderPos();
		break;
	}

	if (m_SelectState != m_OldSelectState)
	{
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldSelectState = m_SelectState;
	}

	m_pCursor->Update();
}

// �X�v���C�g�̕`��.
void CContinueWidget::SpriteDrawing()
{
	switch (m_DrawTurn)
	{
	case CContinueWidget::EDrawTurn::BackGround:
		m_Alpha += BACKGROUND_ALPHA_SPEED;

		if (m_Alpha >= BACKGROUND_ALPHA)
		{
			m_Alpha = BACKGROUND_ALPHA;
			m_DrawTurn = CContinueWidget::EDrawTurn::Text;
		}
		break;
	case CContinueWidget::EDrawTurn::Text:
		m_TextAlpha += TEXT_ALPHA_SPEED;

		if (m_TextAlpha >= ALPHA_MAX)
		{
			m_TextAlpha = ALPHA_MAX;
			m_DrawingState = CSceneWidget::EDrawingState::Finish;
			m_IsDrawing = false;
		}
		break;
	default:
		break;
	}
}
