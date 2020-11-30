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
#include "..\Common\Shader\ShadowMap\ShadowMap.h"
#include "..\Common\Shader\TranslucentShader\TranslucentShader.h"
#include "..\Common\SceneTexRenderer\SceneTexRenderer.h"

#include "..\Common\Font\FontCreate.h"
#include "..\Common\Font\Font.h"

#include "Resource/resource.h"

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

CMain::CMain()
	: m_hWnd			( nullptr )
	, m_pFrameRate		( nullptr )
	, m_pCamera			( nullptr )
	, m_pLight			( nullptr )
	, m_pSceneManager	( nullptr )
	, m_pLoadManager	( nullptr )
{
	m_pFrameRate	= std::make_unique<CFrameRate>( FPS );
	m_pCamera		= std::make_shared<CCamera>();
	m_pLight		= std::make_shared<CLightBase>();
	m_pSceneManager	= std::make_shared<CSceneManager>();
	m_pLoadManager	= std::make_unique<CLoadManager>();

	// �J�����̏�����.
	m_pCamera->SetPosition( D3DXVECTOR3( 0.0f, 10.0f, 10.0f ) );	// ���W.
	m_pCamera->SetLookPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );	// ���_���W.
	// �J�����̃Z�b�g.
	CCameraManager::SetCamera( m_pCamera );

	// ���C�g�̐ݒ�.
	m_pLight->SetPosition( D3DXVECTOR3( 70.0f, 80.0f, -50.0f ) );
	m_pLight->SetLookPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_pLight->SetIntensity( 1.2f );
	CLightManager::SetLgiht( m_pLight );
}

CMain::~CMain()
{
}

//====================================.
//	�������֐�.
//====================================.
HRESULT CMain::Init()
{
	// DirectX9�̍\�z.
	if( FAILED( CDirectX9::Create( m_hWnd ) )) return E_FAIL;
	// DirectX11�̍\�z.
	if( FAILED( CDirectX11::Create( m_hWnd ) )) return E_FAIL;
	// �V�[���e�N�X�`���̏�����.
	if( FAILED( CSceneTexRenderer::Init() ))	return E_FAIL;
	// �V���h�E�}�b�v�V�F�[�_�[�̏�����.
	if( FAILED( CShadowMap::Init() )) return E_FAIL;
	// �������V�F�[�_�[�̏�����.
	if( FAILED( CTranslucentShader::Init() )) return E_FAIL;
	// ImGui�̏�����.
	if( FAILED( CImGuiManager::Init( m_hWnd, 
		CDirectX11::GetDevice(), 
		CDirectX11::GetContext() ))) return E_FAIL;

	m_pSceneManager->SethWnd(m_hWnd);

	return S_OK;
}

//====================================.
//	����֐�.
//====================================.
void CMain::Release()
{
	CTranslucentShader::ReleaseShader();
	CShadowMap::ReleaseShader();
	CSceneTexRenderer::Release();
	CSoundManager::Release();
	CImGuiManager::Release();
	CDirectX11::Release();
	CDirectX9::Release();
}

//====================================.
//	�ǂݍ��݊֐�.
//====================================.
HRESULT CMain::Load()
{
	CDebugText::Init(
		CDirectX11::GetDevice(),
		CDirectX11::GetContext(),
		0.25f, D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ) );
	// �e���\�[�X�̓ǂݍ���.
	m_pLoadManager->LoadResource( m_hWnd,
		CDirectX11::GetDevice(), 
		CDirectX11::GetContext(), 
		CDirectX9::GetDevice() );

	return S_OK;
}

//====================================.
//	�X�V����.
//====================================.
void CMain::Update()
{
	CSceneTexRenderer::ClearBuffer();

	CCameraManager::Update();
	m_pSceneManager->Update();
	// FPS�̕\��.
#if _DEBUG
	CDebugText::SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	CDebugText::Render( "FPS:", (int)m_pFrameRate->GetFrameTime() );
#endif
}

//====================================.
//	���b�Z�[�W���[�v.
//====================================.
void CMain::Loop()
{
	Load();	// �ǂݍ��݊֐�.

	// ���b�Z�[�W���[�v.
	MSG msg = { 0 };
	ZeroMemory( &msg, sizeof(msg) );

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE )){
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		} else {
			// ��ʂ̃N���A.
			CDirectX11::ClearBackBuffer();
			if( m_pLoadManager->ThreadRelease() == true ){
				Update();
			} else {
				m_pLoadManager->Render();
			}
			CDirectX11::SwapChainPresent();
			// �t���[�����[�g�̑ҋ@����.
			m_pFrameRate->Wait();
			if( m_pLoadManager->IsLoadFailed() == true ) return;
		}
	}
}

//====================================.
// �E�B���h�E�������֐�.
//====================================.
HRESULT CMain::InitWindow( HINSTANCE hInstance )
{
	ReadWindowInfo();

	// �E�B���h�E�̒�`.
	WNDCLASSEX wc = {};
	UINT windowStyle = CS_HREDRAW | CS_VREDRAW;

	wc.cbSize			= sizeof(wc);
	wc.style			= windowStyle;
	wc.lpfnWndProc		= WndProc;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON) );
	wc.hCursor			= LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)RGB(0, 0, 0);
	wc.lpszClassName	= APP_NAME.c_str();
	wc.hIconSm			= LoadIcon( hInstance, IDI_APPLICATION );

	// �E�B���h�E�N���X��Windows�ɓo�^.
	if( !RegisterClassEx( &wc ) ){
		ERROR_MESSAGE( "Registration of window class failed" );
		CLog::Print( "Registration of window class failed" );
		return E_FAIL;
	}

	// �E�B���h�E�̈�̒���.
	RECT rect = { 0, 0, WND_W, WND_H }; // ��`�\����.
	DWORD dwStyle;	// �E�B���h�E�X�^�C��.
	dwStyle		= WS_OVERLAPPEDWINDOW;

	if( AdjustWindowRect( &rect, dwStyle, FALSE ) == 0 ){
		MessageBox( nullptr, "�E�B���h�E�̈�̒����Ɏ��s",
			"�G���[���b�Z�[�W", MB_OK );
		return E_FAIL;
	}
	RECT deskRect = {};	// ��ʃT�C�Y.
	SystemParametersInfo( SPI_GETWORKAREA, 0, &deskRect, 0 );
	int pos_x = 0, pos_y = 0;
	// ��ʒ����ɔz�u�ł���悤�Ɍv�Z.
	pos_x = (((deskRect.right - deskRect.left) - (rect.right - rect.left)) / 2 + deskRect.left);
	pos_y = (((deskRect.bottom - deskRect.top) - (rect.bottom - rect.top)) / 2 + deskRect.top);

	
	// �E�B���h�E�̍쐬.
	m_hWnd = CreateWindow(
		APP_NAME.c_str(),		// �A�v����.
		WND_TITLE.c_str(),		// �E�B���h�E�^�C�g��.
		WS_OVERLAPPEDWINDOW,	// �E�B���h�E���(����).
		pos_x, pos_y,			// �\���ʒux,y���W.
		rect.right - rect.left,	// �E�B���h�E��.
		rect.bottom - rect.top,	// �E�B���h�E����.
		nullptr,				// �e�E�B���h�E�n���h��.
		nullptr,				// ���j���[�ݒ�.
		hInstance,				// �C���X�^���X�ԍ�.
		nullptr );				// �E�B���h�E�쐬���ɔ�������C�x���g�ɓn���f�[�^.
	if( !m_hWnd ){
		ERROR_MESSAGE( "Window class creation failed" );
		return E_FAIL;
	}

	// �E�B���h�E�𔼓���������悤.
	/*
	m_hWnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOPMOST, 
		APP_NAME.c_str(), 
		WND_TITLE.c_str(), 
		WS_POPUP, 
		pos_x, pos_y, 
		rect.right - rect.left, 
		rect.bottom - rect.top,
		NULL,
		NULL, 
		hInstance,
		NULL);
	if( SetLayeredWindowAttributes(m_hWnd, 0, 255, LWA_COLORKEY | LWA_ALPHA) == false ){
		return E_FAIL;
	}
	*/
	

	// �E�B���h�E�̕\��.
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	return S_OK;
}

//====================================.
// �E�B���h�E���ǂݍ���.
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