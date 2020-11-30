#include "CameraConfigWidget.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Cursor\Cursor.h"
#include "..\..\..\Slider\Slider.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

CCameraConfigWidget::CCameraConfigWidget()
	: m_pSprites			()
	, m_pCursor				( nullptr )
	, m_pSpeedSlinder		( nullptr )
	, m_ConfigState		()
	, m_SlinderPosition		( 0.0f, 0.0f, 0.0f )
	, m_IsOlsReverse		( false )
	, m_OldMoveSpeed		( 0.0f )
	, m_NowSelectState		( ESelectState_CameraControl )
	, m_OldNowSelectState	( ESelectState_CameraControl )
	, m_NowConfigState		( ESelectState_Select )
	, m_NowControlState		( ECameraControlState_Normal )
	, m_InputWaitTime		( 0.0f )
{
	m_pCursor = std::make_unique<CCursor>();
	m_pSpeedSlinder = std::make_unique<CSlinder>( 0.1f, 0.005f );
}

CCameraConfigWidget::~CCameraConfigWidget()
{
}

// �������֐�.
bool CCameraConfigWidget::Init()
{
	if( SpriteSetting()			== false ) return false;
	if( m_pCursor->Init()		== false ) return false;
	if( m_pSpeedSlinder->Init() == false ) return false;
	if( CFileManager::BinaryReading( CRotLookAtCenter::CONFIG_FILE_PATH, m_ConfigState ) == false ) return false;
	m_pSpeedSlinder->SetValue( m_ConfigState.MoveSpeed );
	
	return true;
}

// �X�V�֐�.
void CCameraConfigWidget::Update()
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
	case ESelectState_CameraControl:
		CameraControlUpdate();	// �J��������ݒ�̍X�V.
		break;
	case ESelectState_CameraSpeed:
		CameraSpeedUpdate();	// �J�������x�ݒ�̍X�V.
		break;
	default:
		break;
	}
	m_pCursor->Update();
}

// �`��֐�.
void CCameraConfigWidget::Render()
{
	m_pSpeedSlinder->SetPosition( m_SlinderPosition );
	m_pSpeedSlinder->Render();

	switch( m_NowConfigState )
	{
	case ESelectState_Select:
		// �A�C�R���̍��W����ʊO�ɐݒ�.
		m_pSprites[ESpriteNo_Icon]->SetPosition( { -500.0f, 0.0f, 0.0f } );

		// �J�[�\���̍��W�����ݑI�����Ă���ꏊ�ɐݒ�.
		m_pCursor->SetPosition( m_pSprites[m_NowSelectState-2]->GetRenderPos() );
		m_pCursor->Render();
		break;
	case ESelectState_CameraControl:
		// �J�[�\���̍��W�����ݑI�����Ă���ꏊ�ɐݒ�.
		m_pCursor->SetPosition( m_pSprites[ESpriteNo_Choise+m_NowControlState]->GetRenderPos() );
		m_pCursor->Render();
		break;
	case ESelectState_CameraSpeed:
		// �A�C�R���̍��W���X���C�_�[�̃A�C�R���̍��W�ɐݒ肷��.
		m_pSprites[ESpriteNo_Icon]->SetPosition( m_pSpeedSlinder->GetIconPosition() );
		break;
	default:
		break;
	}

	int no = 0;
	for( auto& s : m_pSprites ){
		if( no == 2 ){
			// �ʏ킩���]���̑I�����Ă���J�[�\���̍��W�𒲐�����.
			D3DXVECTOR3 pos;
			if( m_ConfigState.IsReverse == false ){
				pos = m_pSprites[ESpriteNo_Normal]->GetRenderPos();
			} else {
				pos = m_pSprites[ESpriteNo_Inversoin]->GetRenderPos();
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
bool CCameraConfigWidget::IsEnd()
{
	if( m_NowConfigState != ESelectState_None ) return false;
	m_NowConfigState = ESelectState_Select;
	CFileManager::BinaryWriting( CRotLookAtCenter::CONFIG_FILE_PATH, m_ConfigState );
	return true;
}

// ���ʂ̑I��.
void CCameraConfigWidget::SelectType()
{
	if( m_InputWaitTime > 0.0f ) return;

	// ��Ɉړ�.
	if( GetAsyncKeyState(VK_UP) & 0x0001 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX ){
		m_NowSelectState--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState <= ESelectState_CameraControl ? ESelectState_CameraControl : m_NowSelectState;
	} 
	// ���Ɉړ�.
	if( GetAsyncKeyState(VK_DOWN) & 0x0001 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN ){
		m_NowSelectState++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState >= ESelectState_CameraSpeed ? ESelectState_CameraSpeed : m_NowSelectState;
	}
	// SE��炷.
	if( m_NowSelectState != m_OldNowSelectState ){
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldNowSelectState = m_NowSelectState;
	}
}

// ����.
void CCameraConfigWidget::Determination()
{
	if(!(GetAsyncKeyState(VK_RETURN) & 0x0001 ) && CXInput::B_Button() != CXInput::enPRESSED_MOMENT ) return;
	switch( m_NowSelectState )
	{
	case ESelectState_CameraControl:
		m_NowConfigState = ESelectState_CameraControl;
		m_IsOlsReverse = m_ConfigState.IsReverse;
		break;
	case ESelectState_CameraSpeed:
		m_NowConfigState = ESelectState_CameraSpeed;
		m_OldMoveSpeed = m_ConfigState.MoveSpeed;
		break;
	default:
		break;
	}
	CSoundManager::PlaySE("Determination");
}

// �J��������̍X�V.
void CCameraConfigWidget::CameraControlUpdate()
{
	// �E�Ɉړ�.
	if( GetAsyncKeyState(VK_RIGHT) & 0x0001 || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX ){
		m_NowControlState++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowControlState = m_NowControlState >= ECameraControlState_Inversion ? ECameraControlState_Inversion : m_NowControlState;
		m_ConfigState.IsReverse = true;
	}
	// ���Ɉړ�.
	if( GetAsyncKeyState(VK_LEFT) & 0x0001 || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN ){
		m_NowControlState--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowControlState = m_NowControlState <= ECameraControlState_Normal ? ECameraControlState_Normal : m_NowControlState;
		m_ConfigState.IsReverse = false;
	} 

	// ����.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 || CXInput::B_Button() == CXInput::enPRESSED_MOMENT ){
		CSoundManager::PlaySE("Determination");
		m_NowConfigState = ESelectState_Select;
	}
	// �L�����Z��.
	if( GetAsyncKeyState(VK_BACK) & 0x0001 || CXInput::A_Button() == CXInput::enPRESSED_MOMENT ){
		CSoundManager::PlaySE("CancelDetermination");
		m_NowConfigState = ESelectState_Select;
		m_ConfigState.IsReverse = m_IsOlsReverse;
	}
}

// �J�������x�̍X�V.
void CCameraConfigWidget::CameraSpeedUpdate()
{
	// �l�𑝂₷.
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX )
		m_pSpeedSlinder->AddValue( 0.001f );
	// �l�����炷.
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN )
		m_pSpeedSlinder->SubValue( 0.001f );

	// ����.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ||
		CXInput::B_Button() == CXInput::enPRESSED_MOMENT ){
		CSoundManager::PlaySE("Determination");
		m_NowConfigState = ESelectState_Select;
		m_ConfigState.MoveSpeed = m_pSpeedSlinder->GetValue();
	}

	// �L�����Z��.
	if( GetAsyncKeyState(VK_BACK) & 0x0001 || CXInput::A_Button() == CXInput::enPRESSED_MOMENT ){
		CSoundManager::PlaySE("CancelDetermination");
		m_NowConfigState = ESelectState_Select;
		m_ConfigState.MoveSpeed = m_OldMoveSpeed;
		m_pSpeedSlinder->SetValue( m_OldMoveSpeed );
	}

	// �X���C�_�[�̍X�V.
	m_pSpeedSlinder->Update();
}

// �X�v���C�g�ݒ�֐�.
bool CCameraConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_CONTROL_NAME,
		SPRITE_SPEED_NAME,
		SPRITE_CHOICE_NAME,
		SPRITE_NORMAL_NAME,
		SPRITE_INVERSION_NAME,
		SPRITE_ICON_NAME,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �������̍ő�l�ݒ�.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
		if( sprite != ESpriteNo_Speed ) continue;
		m_SlinderPosition = m_pSprites[sprite]->GetRenderPos();
		m_SlinderPosition.x += 200.0f;
		m_SlinderPosition.y += m_pSprites[sprite]->GetSpriteSize().y/2.0f;
	}
	return true;
}