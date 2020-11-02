#ifndef MAIN_H
#define MAIN_H

#include <memory>
#include <string>

#include "..\Global.h"

//-------------------------.
//	前方宣言.
class CDirectX11;
class CDirectX9;
class CFrameRate;
class CLoadManager;
class CSceneManager;
class CLightBase;
//-------------------------.

/**************************************************
*	ﾒｲﾝｸﾗｽ.
**/
class CMain
{
	std::string WND_TITLE;
	std::string APP_NAME;
	const char* WINDOW_INFO_TEXT_PATH = "Data\\WindowInfo.txt";
public:
	CMain();
	~CMain();
	// ウィンドウ初期化関数.
	HRESULT InitWindow( HINSTANCE hInstance );

	// メッセージループ.
	void Loop();

	// 初期化関数.
	HRESULT Init();
	// 解放関数.
	void Release();

private:
	// 更新処理.
	void Update();

	// 読み込み関数.
	HRESULT Load();
	// ウィンドウ情報読み込み.
	void ReadWindowInfo();

private:
	HWND m_hWnd;

	std::unique_ptr<CFrameRate>	m_pFrameRate;	// フレームレート.
	std::shared_ptr<CCamera>	m_pCamera;		// カメラクラス.
	std::shared_ptr<CLightBase>	m_pLight;		// ライトクラス.

	std::shared_ptr<CSceneManager>	m_pSceneManager;
	std::unique_ptr<CLoadManager>	m_pLoadManager;
};

#endif	// #ifndef MAIN_H.
