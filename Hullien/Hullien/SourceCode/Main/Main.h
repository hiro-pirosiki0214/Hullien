#ifndef MAIN_H
#define MAIN_H

#include <memory>
#include <string>

#include "..\Global.h"

//-------------------------.
//	�O���錾.
class CDirectX11;
class CDirectX9;
class CFrameRate;
class CLoadManager;
class CSceneManager;
class CLightBase;
//-------------------------.

/**************************************************
*	Ҳݸ׽.
**/
class CMain
{
	std::string WND_TITLE;
	std::string APP_NAME;
	const char* WINDOW_INFO_TEXT_PATH = "Data\\WindowInfo.txt";
public:
	CMain();
	~CMain();
	// �E�B���h�E�������֐�.
	HRESULT InitWindow( HINSTANCE hInstance );

	// ���b�Z�[�W���[�v.
	void Loop();

	// �������֐�.
	HRESULT Init();
	// ����֐�.
	void Release();

private:
	// �X�V����.
	void Update();

	// �ǂݍ��݊֐�.
	HRESULT Load();
	// �E�B���h�E���ǂݍ���.
	void ReadWindowInfo();

private:
	HWND m_hWnd;

	std::unique_ptr<CFrameRate>	m_pFrameRate;	// �t���[�����[�g.
	std::shared_ptr<CCamera>	m_pCamera;		// �J�����N���X.
	std::shared_ptr<CLightBase>	m_pLight;		// ���C�g�N���X.

	std::shared_ptr<CSceneManager>	m_pSceneManager;
	std::unique_ptr<CLoadManager>	m_pLoadManager;
};

#endif	// #ifndef MAIN_H.
