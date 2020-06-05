#ifndef XINPUT_CONFIG_H
#define XINPUT_CONFIG_H

#include <Windows.h>
#include <Xinput.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

#pragma comment( lib, "xinput.lib" )

/*********************************:
*	Xinput設定クラス.
*		(継承させて使用).
*/
class CXInputConfig
{
public:
	inline static const char* FILE_PATH = "Data\\Config\\controller.txt";	// コントローラーのファイルパス.

public:
	// ボタンリスト.
	enum enBUTTON_LIST
	{
		None,
		
		A = 0,
		B,
		X,
		Y,
		Left,
		Right,

		Max,
	};

public:
	CXInputConfig();
	virtual ~CXInputConfig();

	// ボタンの設定.
	void SetButton( enBUTTON_LIST before, enBUTTON_LIST after );

	// ファイルの読み込み.
	std::vector<std::string> FileReading();

protected:
	std::unordered_map<enBUTTON_LIST, DWORD> m_Button;	// 決定されたボタン.
	std::vector<DWORD>	m_ButtonList;	// ボタンのリスト.
};

#endif	// #ifndef XINPUT_CONFIG_H.