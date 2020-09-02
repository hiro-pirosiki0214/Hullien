#include "Main.h"

#include <fstream>
#include <sstream>

#include "..\Common\D3DX\D3DX11.h"
#include "..\Common\D3DX\D3DX9.h"
#include "..\Utility\FrameRate\FrameRate.h"
#include "..\Camera\Camera.h"
#include "..\Camera\CameraManager\CameraManager.h"
#include "..\Light\LightBase.h"
#include "..\Light\LightManager\LightManager.h"
#include "..\Common\DebugText\DebugText.h"
#include "..\Scene\Scenes\SceneList.h"
#include "..\Resource\LoadManager\LoadManager.h"
#include "..\Utility\FileManager\FileManager.h"
#include "..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\XAudio2\SoundManager.h"

#include "..\Common\Font\FontCreate.h"
#include "..\Common\Font\Font.h"

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

CMain::CMain()
	: m_hWnd			( nullptr )
	, m_pDirectX11		( nullptr )
	, m_pDirectX9		( nullptr )
	, m_pFrameRate		( nullptr )
	, m_pCamera			( nullptr )
	, m_pLight			( nullptr )
	, m_pSceneManager	( nullptr )
	, m_pLoadManager	( nullptr )
{
	m_pDirectX11	= std::make_unique<CDirectX11>();
	m_pDirectX9		= std::make_unique<CDirectX9>();
	m_pFrameRate	= std::make_unique<CFrameRate>( FPS );
	m_pCamera		= std::make_shared<CCamera>();
	m_pLight		= std::make_shared<CLightBase>();
	m_pSceneManager	= std::make_shared<CSceneManager>();
	m_pLoadManager	= std::make_unique<CLoadManager>();

	// カメラの初期化.
	m_pCamera->SetPosition( D3DXVECTOR3( 0.0f, 1.0f, 30.0f ) );		// 座標.
	m_pCamera->SetLookPosition( D3DXVECTOR3( 0.0f, 2.0f, 0.0f ) );	// 視点座標.
	// カメラのセット.
	CCameraManager::SetCamera( m_pCamera );

	m_pLight->SetPosition( D3DXVECTOR3( 0.0f, 5.0f, 0.0f ) );
	m_pLight->SetDirection( D3DXVECTOR3( 1.5f, 1.0f, -1.0f ) );
	m_pLight->SetIntensity( 1.0f );
	CLightManager::SetLgiht( m_pLight );
}

CMain::~CMain()
{
}

//====================================.
//	初期化関数.
//====================================.
HRESULT CMain::Init()
{
	// DirectX9の構築.
	if( FAILED( m_pDirectX9->Create( m_hWnd ) )) return E_FAIL;
	// DirectX11の構築.
	if( FAILED( m_pDirectX11->Create( m_hWnd ) )) return E_FAIL;
	// ImGuiの初期化.
	CImGuiManager::Init(
		m_hWnd, 
		m_pDirectX11->GetDevice(), 
		m_pDirectX11->GetContext() );

	return S_OK;
}

//====================================.
//	解放関数.
//====================================.
void CMain::Release()
{
	CImGuiManager::Release();
	m_pDirectX11->Release();
	m_pDirectX9->Release();
}

//====================================.
//	読み込み関数.
//====================================.
HRESULT CMain::Load()
{
	CDebugText::Init(
		m_pDirectX11->GetDevice(),
		m_pDirectX11->GetContext(),
		38.0f, D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ) );
	// 各リソースの読み込み.
	m_pLoadManager->LoadResource( m_hWnd,
		m_pDirectX11->GetDevice(), 
		m_pDirectX11->GetContext(), 
		m_pDirectX9->GetDevice() );

	return S_OK;
}

//====================================.
//	更新処理.
//====================================.
void CMain::Update()
{
	// 画面のクリア.
	m_pDirectX11->ClearBackBuffer();
	CCameraManager::InitViewProj();

	CCameraManager::Update();
	m_pSceneManager->Update();
	// FPSの表示.
	CDebugText::SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	CDebugText::Render( "FPS:", (int)m_pFrameRate->GetFrameTime() );

	m_pDirectX11->SwapChainPresent();
}

//====================================.
//	メッセージループ.
//====================================.
void CMain::Loop()
{
	Load();	// 読み込み関数.

	// メッセージループ.
	MSG msg = { 0 };
	ZeroMemory( &msg, sizeof(msg) );

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE )){
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		} else {
			if( m_pLoadManager->ThreadRelease() == true ){
				Update();
			}
			// フレームレートの待機処理.
			m_pFrameRate->Wait();
		}
	}
}

//====================================.
// ウィンドウ初期化関数.
//====================================.
HRESULT CMain::InitWindow( HINSTANCE hInstance )
{
	ReadWindowInfo();

	// ウィンドウの定義.
	WNDCLASSEX wc = {};
	UINT windowStyle = CS_HREDRAW | CS_VREDRAW;

	wc.cbSize			= sizeof(wc);
	wc.style			= windowStyle;
	wc.lpfnWndProc		= WndProc;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( nullptr, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject( LTGRAY_BRUSH );
	wc.lpszClassName	= APP_NAME.c_str();
	wc.hIconSm			= LoadIcon( nullptr, IDI_APPLICATION );

	// ウィンドウクラスをWindowsに登録.
	if( !RegisterClassEx( &wc ) ){
		ERROR_MESSAGE( "Registration of window class failed" );
		CLog::Print( "Registration of window class failed" );
		return E_FAIL;
	}

	// ウィンドウ領域の調整.
	RECT rect = { 0, 0, WND_W, WND_H }; // 矩形構造体.
	DWORD dwStyle;	// ウィンドウスタイル.
	dwStyle		= WS_OVERLAPPEDWINDOW;

	if( AdjustWindowRect( &rect, dwStyle, FALSE ) == 0 ){
		MessageBox( nullptr, "ウィンドウ領域の調整に失敗",
			"エラーメッセージ", MB_OK );
		return E_FAIL;
	}
	RECT deskRect = {};	// 画面サイズ.
	SystemParametersInfo( SPI_GETWORKAREA, 0, &deskRect, 0 );
	int pos_x = 0, pos_y = 0;
	// 画面中央に配置できるように計算.
	pos_x = (((deskRect.right - deskRect.left) - (rect.right - rect.left)) / 2 + deskRect.left);
	pos_y = (((deskRect.bottom - deskRect.top) - (rect.bottom - rect.top)) / 2 + deskRect.top);

	// ウィンドウの作成.
	m_hWnd = CreateWindow(
		APP_NAME.c_str(),		// アプリ名.
		WND_TITLE.c_str(),		// ウィンドウタイトル.
		WS_OVERLAPPEDWINDOW,	// ウィンドウ種別(普通).
		pos_x, pos_y,			// 表示位置x,y座標.
		rect.right - rect.left,	// ウィンドウ幅.
		rect.bottom - rect.top,	// ウィンドウ高さ.
		nullptr,				// 親ウィンドウハンドル.
		nullptr,				// メニュー設定.
		hInstance,				// インスタンス番号.
		nullptr );				// ウィンドウ作成時に発生するイベントに渡すデータ.
	if( !m_hWnd ){
		ERROR_MESSAGE( "Window class creation failed" );
		return E_FAIL;
	}

	// ウィンドウの表示.
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	return S_OK;
}

//====================================.
// ウィンドウ情報読み込み.
//====================================.
void CMain::ReadWindowInfo()
{
	std::vector<std::string> readList = CFileManager::TextLoading( WINDOW_INFO_TEXT_PATH );

	if( readList.empty() == true ){
		ERROR_MESSAGE( "The list of WindowInfo was empty" );
		return;
	}

	WND_TITLE = readList[0];
	APP_NAME = readList[1];
}