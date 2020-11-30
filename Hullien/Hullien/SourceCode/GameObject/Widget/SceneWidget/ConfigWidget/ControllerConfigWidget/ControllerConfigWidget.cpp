#include "ControllerConfigWidget.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Cursor\Cursor.h"
#include "..\..\..\Slider\Slider.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Utility\XInput\XInput.h"

CControllerConfigWidget::CControllerConfigWidget()
	: m_pSprites			()
	, m_pCursor				( nullptr )
	, m_IsVibration			( true )
	, m_NowSelectState		( ESelectState_Vibration )
	, m_OldNowSelectState	( ESelectState_Vibration )
	, m_NowConfigState		( ESelectState_Select )
	, m_NowControlState		( EControlllerState_VibrationOn )
	, m_InputWaitTime		( 0.0f )
{
	m_pCursor = std::make_unique<CCursor>();
}

CControllerConfigWidget::~CControllerConfigWidget()
{
}

// �������֐�.
bool CControllerConfigWidget::Init()
{
	if( SpriteSetting()		== false ) return false;
	if( m_pCursor->Init()	== false ) return false;

	return true;
}

// �X�V�֐�.
void CControllerConfigWidget::Update()
{
	m_InputWaitTime--;

	switch( m_NowConfigState )
	{
	case ESelectState_Select:
		SelectType();
		Determination();
		if( GetAsyncKeyState(VK_BACK) & 0x0001 ||
			CXInput::A_Button() == CXInput::enPRESSED_MOMENT ){
			CSoundManager::PlaySE("CancelDetermination");
			m_NowConfigState = ESelectState_None;
		}
		break;
	case ESelectState_Vibration:
		VibChoiceUpdate();	// �o�C�u�I���̍X�V.
		break;
	default:
		break;
	}
	m_pCursor->Update();
}

// �`��֐�.
void CControllerConfigWidget::Render()
{
	switch( m_NowConfigState )
	{
	case ESelectState_Select:
		// �J�[�\���̍��W�����ݑI�����Ă���ꏊ�ɐݒ�.
		m_pCursor->SetPosition( m_pSprites[m_NowSelectState-2]->GetRenderPos() );
		m_pCursor->Render();
		break;
	case ESelectState_Vibration:
		// �J�[�\���̍��W�����ݑI�����Ă���ꏊ�ɐݒ�.
		m_pCursor->SetPosition( m_pSprites[m_NowControlState+1]->GetRenderPos() );
		m_pCursor->Render();
		break;
	default:
		break;
	}

	int no = 0;
	for( auto& s : m_pSprites ){
		if( no == ESpriteNo_Choise ){
			// �ʏ킩���]���̑I�����Ă���J�[�\���̍��W�𒲐�����.
			D3DXVECTOR3 pos;
			if( m_IsVibration == true ){
				pos = m_pSprites[ESpriteNo_On]->GetRenderPos();
			} else {
				pos = m_pSprites[ESpriteNo_Off]->GetRenderPos();
			}
			pos.y += s->GetSpriteSize().y/2.3f;
			s->SetPosition( pos );
		}
		s->SetDeprh(false);
		s->RenderUI();
		s->SetDeprh(true);
		no++;
	}
}

// �I��������.
bool CControllerConfigWidget::IsEnd()
{
	if( m_NowConfigState != ESelectState_None ) return false;
	CFileManager::BinaryReading( CXInput::VIBRATION_CONFIG_FILE_PATH, m_IsVibration );
	m_NowConfigState = ESelectState_Select;
	return true;
}

// ���ʂ̑I��.
void CControllerConfigWidget::SelectType()
{
	if( m_InputWaitTime > 0.0f ) return;

	// ��Ɉړ�.
	if( GetAsyncKeyState(VK_UP) & 0x0001 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX ){
		m_NowSelectState--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState <= ESelectState_Vibration ? ESelectState_Vibration : m_NowSelectState;
	} 
	// ���Ɉړ�.
	if( GetAsyncKeyState(VK_DOWN) & 0x0001 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN ){
		m_NowSelectState++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState >= ESelectState_Vibration ? ESelectState_Vibration : m_NowSelectState;
	}
	// SE��炷.
	if( m_NowSelectState != m_OldNowSelectState ){
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldNowSelectState = m_NowSelectState;
	}
}

// ����.
void CControllerConfigWidget::Determination()
{
	if(!(GetAsyncKeyState(VK_RETURN) & 0x0001 ) && CXInput::B_Button() != CXInput::enPRESSED_MOMENT ) return;
	switch( m_NowSelectState )
	{
	case ESelectState_Vibration:
		m_NowConfigState = ESelectState_Vibration;
		break;
	default:
		break;
	}
	CSoundManager::PlaySE("Determination");
}

// �o�C�u�I���̍X�V.
void CControllerConfigWidget::VibChoiceUpdate()
{
	// �E�Ɉړ�.
	if( GetAsyncKeyState(VK_RIGHT) & 0x0001 || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX ){
		m_NowControlState++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowControlState = m_NowControlState >= EControlllerState_VibrationOff ? EControlllerState_VibrationOff : m_NowControlState;
	}
	// ���Ɉړ�.
	if( GetAsyncKeyState(VK_LEFT) & 0x0001 || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN ){
		m_NowControlState--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowControlState = m_NowControlState <= EControlllerState_VibrationOn ? EControlllerState_VibrationOn : m_NowControlState;
	} 

	// ����.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 || CXInput::B_Button() == CXInput::enPRESSED_MOMENT ){
		m_IsVibration = m_NowControlState == EControlllerState_VibrationOn ? true : false;
		CXInput::SetVibration( m_IsVibration );
		CSoundManager::PlaySE("Determination");
		m_NowConfigState = ESelectState_Select;
	}
	// �L�����Z��.
	if( GetAsyncKeyState(VK_BACK) & 0x0001 || CXInput::A_Button() == CXInput::enPRESSED_MOMENT ){
		CSoundManager::PlaySE("CancelDetermination");
		m_NowConfigState = ESelectState_Select;
	}
}

// �X�v���C�g�ݒ�֐�.
bool CControllerConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_CONTROL_NAME,
		SPRITE_CHOICE_NAME,
		SPRITE_ON_NAME,
		SPRITE_OFF_NAME,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �������̍ő�l�ݒ�.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
	}
	return true;
}