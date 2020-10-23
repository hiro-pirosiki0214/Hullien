#include "XInput.h"
#include <algorithm>

CXInput::CXInput()
	: m_State			( FOUR_LIMITED_CONTROLLER )
	, m_Vibration		( FOUR_LIMITED_CONTROLLER )
	, m_ConnectedCount	( 0 )
	, m_IsVibration		( true )
{
	std::unordered_map<INT, enBUTTON_STATE> inputState;
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
}

CXInput::~CXInput()
{
	for( int i = 0; i < GetInstance()->m_ConnectedCount; i++ ){
		SetVibration( 0, 0, i );	// バイブレーションの初期化.
	}
}

void CXInput::StatsUpdate()
{
	GetInstance()->m_ConnectedCount = 0;	// 接続数の初期化.
	DWORD dwResult;	// 接続状態.
	for( size_t i = 0; i < GetInstance()->m_State.size(); i++ ){
		dwResult = XInputGetState( i, &GetInstance()->m_State[i] );
		if( dwResult == ERROR_SUCCESS ){
			GetInstance()->m_ConnectedCount++;
		}
	}
}

//--------------------------------.
// ボタン.
//--------------------------------.
CXInput::enBUTTON_STATE CXInput::A_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[A] );
}
CXInput::enBUTTON_STATE CXInput::B_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[B] );
}
CXInput::enBUTTON_STATE CXInput::X_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[X] );
}
CXInput::enBUTTON_STATE CXInput::Y_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[Y] );
}

//--------------------------------.
// 方向キー.
//--------------------------------.
CXInput::enBUTTON_STATE CXInput::DPadUp( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );

	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_DPAD_UP );

}
CXInput::enBUTTON_STATE CXInput::DPadDown( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );

	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_DPAD_DOWN );

}
CXInput::enBUTTON_STATE CXInput::DPadLEFT( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );

	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_DPAD_LEFT );

}
CXInput::enBUTTON_STATE CXInput::DPadRIGHT( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );

	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_DPAD_RIGHT );

}

//--------------------------------.
// Lスティック.
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
// Rスティック.
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
// スティックボタン.
//--------------------------------.
CXInput::enBUTTON_STATE CXInput::LThumb_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_LEFT_THUMB );
}
CXInput::enBUTTON_STATE CXInput::RThumb_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_RIGHT_THUMB );
}

//--------------------------------.
// トリガー.
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
// ショルダー.
//--------------------------------.
CXInput::enBUTTON_STATE CXInput::L_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[Left] );
}
CXInput::enBUTTON_STATE CXInput::R_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, GetInstance()->m_Button[Right] );
}

//--------------------------------.
// スタート ボタン.
//--------------------------------.
CXInput::enBUTTON_STATE CXInput::Start_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_START );
}
//--------------------------------.
// バック ボタン.
//--------------------------------.
CXInput::enBUTTON_STATE CXInput::Back_Button( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->ButtonInputState( connectNum, XINPUT_GAMEPAD_BACK );
}

//--------------------------------.
//振動設定.
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
//			"コントローラーの数が足りません", "", MB_OK );
	}
}

bool CXInput::IsButtonInput( const int& connectNum, const DWORD& button_mask )
{
	return ( GetInstance()->m_State[connectNum].Gamepad.wButtons & button_mask );
}

CXInput::enBUTTON_STATE CXInput::ButtonInputState( const int& connectNum, const DWORD& button_mask )
{
	if( m_ButtonStateList[connectNum][button_mask] == enNOT_PUSHING ||
		m_ButtonStateList[connectNum][button_mask] == enSEPARATED ){
		if( IsButtonInput( connectNum, button_mask ) == true ){
			return m_ButtonStateList[connectNum][button_mask] = enPRESSED_MOMENT;	// 押した瞬間.
		} else{
			return m_ButtonStateList[connectNum][button_mask] = enNOT_PUSHING;		// 押してない状態.
		}
	} else{
		if( IsButtonInput( connectNum, button_mask ) == true ){
			return m_ButtonStateList[connectNum][button_mask] = enPRESS_AND_HOLD;	// 押し続けている状態.
		} else{
			return m_ButtonStateList[connectNum][button_mask] = enSEPARATED;		// 押して離した時.
		}
	}
}