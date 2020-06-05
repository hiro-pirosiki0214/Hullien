/**
* @file XInput.h
* @brief This is Use Xinput Controller file.
* @author Fukuta
* @date 2019/11/09
* @details Xinputを使用するためのクラスです.
**/
#ifndef XINPUT_H
#define XINPUT_H

#include "XinputConfig.h"

static const int IDLE_THUMB_MAX		=  10000;	//!< スティックの遊び.
static const int IDLE_THUMB_MIN		= -10000;	//!< スティックの遊び.
static const int IDLE_TIGGER_MAX	=  100;		//!< トリガーボタンの遊び.

/**
* @brief XInput Class(Singleton).
*
* @details バイブレーション機能以外の機能はついてます.
*/
class CXInput : public CXInputConfig
{
	const int FOUR_LIMITED_CONTROLLER = 4;	//!< 接続コントローラの最大数.
	
	enum enBUTTON_STATE
	{
		enNOT_PUSHING		= 0,		//!< 押していない状態.

		enPRESSED_MOMENT	= 1 << 0,	//!< 押した瞬間.
		enPRESS_AND_HOLD	= 1 << 1,	//!< 押し続けている.
		enSEPARATED			= 1 << 2,	//!< 離した状態.
	};
public:
	/**
	* @brief コンストラクタ.
	*/
	CXInput();
	/**
	* @brief デストラクタ.
	*/
	~CXInput();

	/**
	* @fn static void StatsUpdate()
	* @brief コントローラーの状態を更新.
	* @details 毎フレーム事に更新すること.
	*/
	static void StatsUpdate();

	/**
	* @fn static int GetConnectCount()
	* @return int 現在のコントローラーの接続数.
	* @brief コントローラーの接続している数を取得.
	*/
	static int GetConnectCount(){ return GetInstance()->m_ConnectedCount; }
	
	/**
	* @fn static bool A_Button( const int& connectNum = 0 )
	* @brief A ボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE A_Button( const int& connectNum = 0 );

	/**
	* @fn static bool B_Button( const int& connectNum = 0 )
	* @brief B ボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE B_Button( const int& connectNum = 0 );

	/**
	* @fn static bool X_Button( const int& connectNum = 0 )
	* @brief X ボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE X_Button( const int& connectNum = 0 );

	/**
	* @fn static bool Y_Button( const int& connectNum = 0 )
	* @brief Y ボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE Y_Button( const int& connectNum = 0 );

	/**
	* @fn static bool DPadUp( const int& connectNum = 0 )
	* @brief 上ボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE DPadUp( const int& connectNum = 0 );

	/**
	* @fn static bool DPadDown( const int& connectNum = 0 )
	* @brief 下ボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE DPadDown( const int& connectNum = 0 );

	/**
	* @fn static bool DPadLEFT( const int& connectNum = 0 )
	* @brief 左ボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE DPadLEFT( const int& connectNum = 0 );

	/**
	* @fn static bool DPadRIGHT( const int& connectNum = 0 )
	* @brief 右ボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE DPadRIGHT( const int& connectNum = 0 );

	/**
	* @fn static SHORT LThumbX_Axis( const int& connectNum = 0 )
	* @brief LスティックのX軸の値を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return SHORT スティックの傾きの値.
	*/
	static SHORT LThumbX_Axis( const int& connectNum = 0 );

	/**
	* @fn static SHORT LThumbY_Axis( const int& connectNum = 0 )
	* @brief LスティックのY軸の値を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return SHORT スティックの傾きの値.
	*/
	static SHORT LThumbY_Axis( const int& connectNum = 0 );

	/**
	* @fn static SHORT RThumbX_Axis( const int& connectNum = 0 )
	* @brief RスティックのX軸の値を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return SHORT スティックの傾きの値.
	*/
	static SHORT RThumbX_Axis( const int& connectNum = 0 );

	/**
	* @fn static SHORT RThumbY_Axis( const int& connectNum = 0 )
	* @brief RスティックのY軸の値を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return SHORT スティックの傾きの値.
	*/
	static SHORT RThumbY_Axis( const int& connectNum = 0 );

	/**
	* @fn static bool LThumb_Button( const int& connectNum = 0 )
	* @brief Lスティックボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE LThumb_Button( const int& connectNum = 0 );

	/**
	* @fn static bool RThumb_Button( const int& connectNum = 0 )
	* @brief Rスティックボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE RThumb_Button( const int& connectNum = 0 );

	/**
	* @fn static INT LTrigger( const int& connectNum = 0 )
	* @brief Lトリガーの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return INT トリガーの押し込み値.
	*/
	static INT LTrigger( const int& connectNum = 0 );

	/**
	* @fn static INT RTrigger( const int& connectNum = 0 )
	* @brief Rトリガーの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return INT トリガーの押し込み値.
	*/
	static INT RTrigger( const int& connectNum = 0 );

	/**
	* @fn static bool L_Button( const int& connectNum = 0 )
	* @brief L ボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE L_Button( const int& connectNum = 0 );

	/**
	* @fn static bool R_Button( const int& connectNum = 0 )
	* @brief R ボタンの状態を取得.
	* @param connectNum  使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE R_Button( const int& connectNum = 0 );

	/**
	* @fn static bool Start_Button( const int& connectNum = 0 )
	* @brief スタート ボタンの状態を取得.
	* @param connectNum 使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE Start_Button( const int& connectNum = 0 );

	/**
	* @fn static bool Back_Button( const int& connectNum = 0 )
	* @brief バック ボタンの状態を取得.
	* @param connectNum 使用したいコントローラーの指定 何も指定しない場合は 0 固定.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	static enBUTTON_STATE Back_Button( const int& connectNum = 0 );

	// ボタンの設定.
	static void SetButton( enBUTTON_LIST before, enBUTTON_LIST after )
	{ GetInstance()->CXInputConfig::SetButton( before, after ); }

	// ボタンファイルの読み込み.
	static std::vector<std::string> FileReading()
	{ return GetInstance()->CXInputConfig::FileReading(); }

private:
	/**
	* @fn void ConnectCheck( const int& connectNum )
	* @brief 接続の数を確認 クラス内で使用.
	* @param connectNum 指定したコントローラー.
	*/
	void ConnectCheck( const int& connectNum );

	/**
	* @fn bool IsButtonInput( const int& connectNum, const DWORD& button_mask )
	* @brief ボタン系の状態を取得 クラス内で使用.
	* @param connectNum 指定したコントローラー.
	* @param button_mask 指定したいボタン.
	* @return bool 押されていたら true。押されていなかったら false.
	*/
	bool IsButtonInput( const int& connectNum, const DWORD& button_mask );

	/**
	* @fn enBUTTON_STATE ButtonInputState( const int& connectNum, const DWORD& button_mask )
	* @brief ボタン系の入力状態を取得 クラス内で使用.
	* @param connectNum 指定したコントローラー.
	* @param button_mask 指定したいボタン.
	* @return enBUTTON_STATE ボタンの入力状態(押した瞬間など)を返す.
	*/
	enBUTTON_STATE ButtonInputState( const int& connectNum, const DWORD& button_mask );

	/**
	* @fn static CXInput* GetInstance()
	* @brief 自身のクラスを作成、取得.
	* @return CXInput* 作成された自身を返す.
	*/
	static CXInput* GetInstance()
	{
		static std::unique_ptr<CXInput> pInstance = std::make_unique<CXInput>();
		return pInstance.get();
	}

private:
	std::vector<XINPUT_STATE> m_State;	//!< 接続したコントローラーの状態.
	int m_ConnectedCount;				//!< 接続したコントローラーの数.

	std::vector<std::unordered_map<INT, enBUTTON_STATE>> m_ButtonStateList;	//!< 入力されたボタンの状態.

	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CXInput( const CXInput & )				= delete;
	CXInput( CXInput && )					= delete;
	CXInput& operator = ( const CXInput & )	= delete;
	CXInput& operator = ( CXInput && )		= delete;
};


#endif	// #ifndef XINPUT_H.