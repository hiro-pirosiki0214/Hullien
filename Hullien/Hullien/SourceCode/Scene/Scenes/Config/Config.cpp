#include "Config.h"
#include "..\..\SceneManager\SceneManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ConfigWidget\ConfigWidget.h"
#include "..\..\..\XAudio2\SoundManager.h"

CConfig::CConfig( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_pConfigWidget	( nullptr )
	, m_IsChangeScene	( false )
{
	m_pConfigWidget = std::make_unique<CConfigWidget>();
	CFade::SetFadeOut();
}

CConfig::~CConfig()
{
}

// �Ǎ��֐�.
bool CConfig::Load()
{
	if( m_pConfigWidget->Init() == false ) return false;
	CSoundManager::ThreadPlayBGM("TitleBGM");
	CSoundManager::FadeInBGM("TitleBGM");
	m_pSceneManager->SetNowBGMName("TitleBGM");
	return true;
}

// �X�V�֐�.
void CConfig::Update()
{
	if (CFade::GetIsFade() == true) return;
	m_pConfigWidget->Update();
	if( m_pConfigWidget->IsEndConfig()== true ){
		ChangeScene();
	}
}

// �`��֐�.
void CConfig::Render()
{
	m_pConfigWidget->Render();
}

// �V�[���؂�ւ��֐�.
void CConfig::ChangeScene()
{
	if( m_IsChangeScene == false ){
		CFade::SetFadeIn();
		m_pConfigWidget->OffVolumeSeting();
		m_IsChangeScene = true;

		CSoundManager::PlaySE("CancelDetermination");
		CSoundManager::FadeOutBGM("TitleBGM");
	}
	if( m_IsChangeScene == false ) return;
	// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;

	if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
	while( CSoundManager::StopBGMThread("TitleBGM") == false);
	m_pSceneManager->NextSceneMove();
}