#include "XInput.h"
#include "..\FileManager\FileManager.h"
#include <algorithm>

CXInput::CXInput()
	: m_State			( FOUR_LIMITED_CONTROLLER )
	, m_Vibration		( FOUR_LIMITED_CONTROLLER )
	, m_ConnectedCount	( 0 )
	, m_IsVibration		( false )
{
	std::unordered_map<INT, ButtonState> inputState;
	inputState[XINPUT_GAMEPAD_DPAD_UP]			= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_DPAD_DOWN]		= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_DPAD_LEFT]		= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_DPAD_RIGHT]		= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_START]			= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_BACK]				= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_LEFT_THUMB]		= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_RIGHT_THUMB]		= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_LEFT_SHOULDER]	= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_RIGHT_SHOULDER]	= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_A]				= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_B]				= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_X]				= enNOT_PUSHING;
	inputState[XINPUT_GAMEPAD_Y]				= enNOT_PUSHING;
	for( int i = 0; i < FOUR_LIMITED_CONTROLLER; i++ ){
		m_ButtonStateList.emplace_back(inputState);
	}
	CFileManager::BinaryReading( VIBRATION_CONFIG_FILE_PATH, m_IsVibration );
}

CXInput::~CXInput()
{
	for( int i = 0; i < GetInstance()->m_ConnectedCount; i++ ){
		SetVibration( 0, 0, i );	// �o�C�u���[�V�����̏�����.
	}
}

void CXInput::StatsUpdate()
{
	GetInstance()->m_ConnectedCount = 0;	// �ڑ����̏�����.
	DWORD dwResult;	// �ڑ����.
	for( size_t i = 0; i < GetInstance()->m_State.size(); i++ ){
		dwResult = XInputGetState( i, &GetInstance()->m_State[i] );
		if( dwResult == ERROR_SUCCESS ){
			GetInstance()->m_ConnectedCount++;
		}
	}
}

//--------------------------------.
// �{�^��.
//--------------------------------.
CXInput::ButtonState CXInput::A_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[A] );
}
CXInput::ButtonState CXInput::B_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[B] );
}
CXInput::ButtonState CXInput::X_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[X] );
}
CXInput::ButtonState CXInput::Y_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[Y] );
}

//--------------------------------.
// �����L�[.
//--------------------------------.
CXInput::ButtonState CXInput::DPadUp( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );

	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_DPAD_UP );

}
CXInput::ButtonState CXInput::DPadDown( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );

	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_DPAD_DOWN );

}
CXInput::ButtonState CXInput::DPadLEFT( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );

	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_DPAD_LEFT );

}
CXInput::ButtonState CXInput::DPadRIGHT( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );

	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_DPAD_RIGHT );

}

//--------------------------------.
// L�X�e�B�b�N.
//--------------------------------.
SHORT CXInput::LThumbX_Axis( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.sThumbLX;
}
SHORT CXInput::LThumbY_Axis( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.sThumbLY;
}
//--------------------------------.
// R�X�e�B�b�N.
//--------------------------------.
SHORT CXInput::RThumbX_Axis( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.sThumbRX;
}
SHORT CXInput::RThumbY_Axis( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.sThumbRY;
}

//--------------------------------.
// �X�e�B�b�N�{�^��.
//--------------------------------.
CXInput::ButtonState CXInput::LThumb_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_LEFT_THUMB );
}
CXInput::ButtonState CXInput::RThumb_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_RIGHT_THUMB );
}

//--------------------------------.
// �g���K�[.
//--------------------------------.
INT CXInput::LTrigger( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.bLeftTrigger;
}
INT CXInput::RTrigger( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.bRightTrigger;
}

//--------------------------------.
// �V�����_�[.
//--------------------------------.
CXInput::ButtonState CXInput::L_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[Left] );
}
CXInput::ButtonState CXInput::R_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[Right] );
}

//--------------------------------.
// �X�^�[�g �{�^��.
//--------------------------------.
CXInput::ButtonState CXInput::Start_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_START );
}
//--------------------------------.
// �o�b�N �{�^��.
//--------------------------------.
CXInput::ButtonState CXInput::Back_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_BACK );
}

//--------------------------------.
//�U���ݒ�.
//--------------------------------.
bool CXInput::SetVibration( WORD LMotorSpd, WORD RMotorSpd, const int& connectNum  )
{
	if( GetInstance()->m_IsVibration == false ) return false;
	GetInstance()->m_Vibration[connectNum].wLeftMotorSpeed = std::clamp<WORD>( LMotorSpd, INPUT_VIBRATION_MIN, INPUT_VIBRATION_MAX );
	GetInstance()->m_Vibration[connectNum].wRightMotorSpeed = std::clamp<WORD>( RMotorSpd, INPUT_VIBRATION_MIN, INPUT_VIBRATION_MAX );

	if( ERROR_SUCCESS == XInputSetState(
		connectNum, &GetInstance()->m_Vibration[connectNum] ) ){
		return true;
	}
	return false;
}

void CXInput::ConnectCheck( const int& connectNum )
{
	if( connectNum >= m_ConnectedCount ){
//		MessageBox( nullptr,
//			"�R���g���[���[�̐�������܂���", "", MB_OK );
	}
}

bool CXInput::IsButtonInput( const int& connectNum, const DWORD& button_mask )
{
	return ( GetInstance()->m_State[connectNum].Gamepad.wButtons & button_mask );
}

CXInput::ButtonState CXInput::ButtonInputState( const int& connectNum, const DWORD& button_mask )
{
	if( m_ButtonStateList[connectNum][button_mask] == enNOT_PUSHING ||
		m_ButtonStateList[connectNum][button_mask] == enSEPARATED ){
		if( IsButtonInput( connectNum, button_mask ) == true ){
			return m_ButtonStateList[connectNum][button_mask] = enPRESSED_MOMENT;	// �������u��.
		} else{
			return m_ButtonStateList[connectNum][button_mask] = enNOT_PUSHING;		// �����ĂȂ����.
		}
	} else{
		if( IsButtonInput( connectNum, button_mask ) == true ){
			return m_ButtonStateList[connectNum][button_mask] = enPRESS_AND_HOLD;	// ���������Ă�����.
		} else{
			return m_ButtonStateList[connectNum][button_mask] = enSEPARATED;		// �����ė�������.
		}
	}
}