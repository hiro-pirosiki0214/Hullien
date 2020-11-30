#include "SceneManager.h"
#include "..\Scenes\SceneList.h"
#include "..\..\Utility\XInput\XInput.h"
#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\XAudio2\SoundManager.h"
#include "..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\Utility\Command\Command.h"

CSceneManager::CSceneManager()
	: m_hWnd					( nullptr )
	, m_pScene					( nullptr )
	, m_pCommand				( nullptr )
	, m_NowBGMName				()
	, m_NowScene				( EScene::Start )
	, m_NextScene				( EScene::Start )
	, m_IsLoadEnd				( false )
	, m_IsGameOver				( false )
	, m_IsGameEnd				( false )
	, m_IsRetry					( false )
	, m_IsChangeEditScene		( false )
	, m_IsEditSceneChangeActive	( true )
{
	NextSceneMove();
	m_pCommand = std::make_unique<CCommand>();
}

CSceneManager::~CSceneManager()
{
}

//=================================.
//	�X�V�֐�.
//=================================.
void CSceneManager::Update()
{
	if( m_pScene == nullptr ) return;

	CXInput::StatsUpdate();	// �R���g���[���[�̍X�V.

	if( m_IsLoadEnd == false ){
		// �ǂݍ��݂��I�����Ă��Ȃ��ꍇ.
		// �ǂݍ��݂��s��.
		m_IsLoadEnd = m_pScene->Load();
	} else {
		// �ǂݍ��݂��I��������.
		m_pScene->Update();	// �V�[���̍X�V.
		// �Q�[���I�����ĂȂ���Ε`�悷��.
		if( m_IsGameEnd == false ){
			if( m_IsLoadEnd == true )
			{
				CCameraManager::InitViewProj();
				m_pScene->Render();	// �V�[���̕`��.
			}
			if( m_IsLoadEnd == true ) CEditRenderer::Render();
		}
	}

	ChangeEditScene();	// �G�f�B�b�g�V�[���ɐ؂�ւ���.

	// �t�F�[�h�̕`��.
	if (CFade::Init() == false) return;
	CFade::Render();

}

//=================================.
// ���̃V�[���Ɉړ�.
//=================================.
void CSceneManager::NextSceneMove()
{
	m_IsLoadEnd = false;
	m_IsChangeEditScene = false;
	switch( m_NextScene )
	{
	case EScene::Title:
		m_pScene		= std::make_shared<CTitle>( this );
		m_IsGameOver	= false;
		m_NowScene		= m_NextScene;
		m_NextScene		= EScene::GameMain;
		break;
	case EScene::GameMain:
		m_pScene	= std::make_shared<CGame>( this );
		m_NowScene	= m_NextScene;
		m_NextScene	= EScene::ResultCheck;
		break;
	case EScene::ResultCheck:
		if( m_IsGameOver == true ){
			// �Q�[���I�[�o�[.
			m_NextScene = EScene::GameOver;
		} else {
			// �Q�[���N���A.
			m_NextScene = EScene::GameClear;
		}
		// ������x���̊֐����Ă�ŁA���̃V�[�����쐬����.
		NextSceneMove();
		break;
	case EScene::GameClear:
		m_pScene	= std::make_shared<CGameClear>( this );
		m_IsRetry	= false;
		m_NowScene	= m_NextScene;
		m_NextScene	= EScene::Title;
		break;
	case EScene::GameOver:
		m_pScene	= std::make_shared<CGameOver>( this );
		m_IsRetry	= false;
		m_NowScene	= m_NextScene;
		m_NextScene = EScene::Title;
		break;
	case EScene::Config:
		m_pScene		= std::make_shared<CTitle>( this );
		m_IsGameOver	= false;
		m_NowScene		= m_NextScene;
		m_NextScene		= EScene::GameMain;
		break;
	default:
		break;
	}
}

//=================================.
// �Q�[���I��.
//=================================.
void CSceneManager::EndGameClose()
{
	m_IsGameEnd = true;
	PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
}

//=================================.
// �ݒ�V�[���ֈړ�.
//=================================.
void CSceneManager::ConfigSceneMove()
{
	m_pScene = std::make_shared<CConfig>( this );
	m_IsLoadEnd = false;
	m_NextScene	= EScene::Title;
}

//=================================.
// �Q�[�����g���C.
//=================================.
void CSceneManager::RetryGame()
{
	m_IsLoadEnd = false;
	m_IsRetry = true;
	m_pScene = std::make_shared<CGame>( this );
}

//=================================.
// �G�f�B�b�g�V�[���ɕύX.
//=================================.
void CSceneManager::ChangeEditScene()
{
	if( m_IsEditSceneChangeActive == false ) return;
	if( m_IsChangeEditScene == false ){
		m_pCommand->Update();
	}

	if( m_pCommand->IsSuccess() && m_IsChangeEditScene == false ){
		CFade::SetFadeIn();
		CSoundManager::PlaySE( "Determination" );
		CSoundManager::FadeOutBGM( m_NowBGMName );
		m_IsChangeEditScene = true;
	}
	if( m_IsChangeEditScene == true ){
		// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
		if(CFade::GetFadeState() != CFade::EFadeState::In) return;
		if(CFade::GetIsFade() == true) return;
		if(CSoundManager::GetBGMVolume( m_NowBGMName ) > 0.0f) return;
		while(CSoundManager::StopBGMThread( m_NowBGMName )== false);
		m_pScene	= std::make_shared<CEditor>( this );
		m_IsLoadEnd	= false;
		m_IsChangeEditScene = false;
		m_NextScene	= m_NowScene;
	}
}