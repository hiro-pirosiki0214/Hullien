#ifndef COMMAND_H
#define COMMAND_H

#include <queue>
#include <vector>

class CCommand
{
	// キー(button)のリスト.
	enum enKey : unsigned char
	{
		EKey_Up,	// 上.
		EKey_Down,	// 下.
		EKey_Right,	// 右.
		EKey_Left,	// 左.
		EKey_A,		// A.
		EKey_B,		// B.
		EKey_X,		// X.
		EKey_Y,		// Y.
		EKey_RB,	// RB.
		EKey_LB,	// LB.
	} typedef EKey;
	
	// コマンドのリスト.
 	const std::vector<EKey> COMMAND_LIST = 
	{
		EKey_Up,
		EKey_Up,
		EKey_Down,
		EKey_Down,
		EKey_Left,
		EKey_Right,
		EKey_Left,
		EKey_Right,
		EKey_RB,
		EKey_LB,
	};

	const int COMMAND_RESET_FRAME = 30;	// コマンドのリセットフレーム.

public:
	CCommand();
	~CCommand();

	void Update();

	// 成功したか.
	inline bool IsSuccess() { return m_isSuccessCommand; }

private:
	// キー(buttonの入力).
	void InputKey();
	// キーの追加.
	void PushKey( const EKey& key );
	// キューの確認.
	void Check();

private:
	std::queue<EKey>	m_KeyQueue;			// 入力されたキーのキュー.
	int					m_TimeCount;		// 入力されてからのタイムカウント.
	bool				m_isSuccessCommand;	// コマンドが成功したか.
};

#endif	// #ifndef COMMAND_H.