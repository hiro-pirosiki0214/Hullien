#include "Command.h"
#include "..\XInput\XInput.h"

CCommand::CCommand()
{
}

CCommand::~CCommand()
{
}

void CCommand::Update()
{
	m_isSuccessCommand = false;
	InputKey();	// �L�[�̓���.
	Check();	// �m�F.
	m_TimeCount++;
	if( m_TimeCount >= COMMAND_RESET_FRAME ){
		m_TimeCount = 0;
		for( int i = 0; i < static_cast<int>(m_KeyQueue.size()); i++ ) m_KeyQueue.pop();
	}
}

// �L�[(button�̓���).
void CCommand::InputKey()
{
	if( GetAsyncKeyState(VK_UP)		& 0x0001 || CXInput::DPadUp()	== CXInput::enPRESSED_MOMENT )
		PushKey( EKey_Up );
	if( GetAsyncKeyState(VK_DOWN)	& 0x0001 || CXInput::DPadDown()	== CXInput::enPRESSED_MOMENT )
		PushKey( EKey_Down );
	if( GetAsyncKeyState(VK_RIGHT)	& 0x0001 || CXInput::DPadRIGHT()== CXInput::enPRESSED_MOMENT )
		PushKey( EKey_Right );
	if( GetAsyncKeyState(VK_LEFT)	& 0x0001 || CXInput::DPadLEFT()	== CXInput::enPRESSED_MOMENT )
		PushKey( EKey_Left );
	if( GetAsyncKeyState('A') & 0x0001 || CXInput::A_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_A );
	if( GetAsyncKeyState('B') & 0x0001 || CXInput::B_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_B );
	if( GetAsyncKeyState('X') & 0x0001 || CXInput::X_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_X );
	if( GetAsyncKeyState('Y') & 0x0001 || CXInput::Y_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_Y );
	if( GetAsyncKeyState('R') & 0x0001 || CXInput::R_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_RB );
	if( GetAsyncKeyState('L') & 0x0001 || CXInput::L_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_LB );
}

// �L�[�̒ǉ�.
void CCommand::PushKey( const EKey& key )
{
	m_KeyQueue.push( key );
	m_TimeCount = 0;
}

// �L���[�̊m�F.
void CCommand::Check()
{
	// �L���[�̃T�C�Y�����X�g�̃T�C�Y���r.
	if( COMMAND_LIST.size() != m_KeyQueue.size() ) return;

	for( int i = 0; i < static_cast<int>(COMMAND_LIST.size()); i++ ){
		if( COMMAND_LIST[i] == m_KeyQueue.front() ){
			m_isSuccessCommand = true;
		} else {
			m_isSuccessCommand = false;
		}
		m_KeyQueue.pop();
	}
}