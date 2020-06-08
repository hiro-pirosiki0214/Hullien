#include "XinputConfig.h"
#include "..\FileManager\FileManager.h"

CXInputConfig::CXInputConfig()
	: m_Button		()
	, m_ButtonList	( Max )
{
	// 各ボタンの初期化.
	m_ButtonList[A] = m_Button[A] = XINPUT_GAMEPAD_A;
	m_ButtonList[B] = m_Button[B] = XINPUT_GAMEPAD_B;
	m_ButtonList[X] = m_Button[X] = XINPUT_GAMEPAD_X;
	m_ButtonList[Y] = m_Button[Y] = XINPUT_GAMEPAD_Y;
	m_ButtonList[Left]	= m_Button[Left]	= XINPUT_GAMEPAD_LEFT_SHOULDER;
	m_ButtonList[Right]	= m_Button[Right]	= XINPUT_GAMEPAD_RIGHT_SHOULDER;

	// ファイルから読み込んだボタンの設定.
	FileReading();
}

CXInputConfig::~CXInputConfig()
{
	m_ButtonList.clear();
	m_Button.clear();
}

//------------------------------.
// ボタンの設定.
//------------------------------.
void CXInputConfig::SetButton( enBUTTON_LIST before, enBUTTON_LIST after )
{
	// 各引数がボタンの最大より多ければ終了.
	if( before >= Max ) return;
	if( after >= Max ) return;
	// 決定ボタンに設定.
	m_Button[before] = m_ButtonList[after];
}

//------------------------------.
// ファイルの読み込み.
//------------------------------.
std::vector<std::string> CXInputConfig::FileReading()
{
	std::vector<std::string> readList;	// 読込用のリスト.
	// ファイルの読込.
	readList = CFileManager::TextLoading( FILE_PATH );

	// 読み込んだサイズがなければ終了.
	if( readList.empty() == true ) return readList;

	// 各ボタンに読み込んだボタンを設定.
	for( size_t i = 0; i < readList.size(); i+=2 ){
		// 読み込んだサイズがボタンのリストより多いので割ってあげる.
		int index = (i+1)/2;
		enBUTTON_LIST no	= static_cast<enBUTTON_LIST>(i/2);
		enBUTTON_LIST temp	= static_cast<enBUTTON_LIST>(std::stoi(readList[i+1]));
//		m_ButtonList[index] =  = temp;
		m_Button[no] = m_ButtonList[temp];
	}

	return readList;
}