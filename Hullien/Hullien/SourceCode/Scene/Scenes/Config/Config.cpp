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

// 読込関数.
bool CConfig::Load()
{
	if( m_pConfigWidget->Init() == false ) return false;
	CSoundManager::ThreadPlayBGM("TitleBGM");
	CSoundManager::FadeInBGM("TitleBGM");
	return true;
}

// 更新関数.
void CConfig::Update()
{
	if (CFade::GetIsFade() == true) return;
	m_pConfigWidget->Update();
	if( m_pConfigWidget->IsEndConfig()== true ){
		ChangeScene();
	}
}

// 描画関数.
void CConfig::Render()
{
	m_pConfigWidget->Render();
}

// シーン切り替え関数.
void CConfig::ChangeScene()
{
	if( m_IsChangeScene == false ){
		CFade::SetFadeIn();
		m_pConfigWidget->OffVolumeSeting();
		m_IsChangeScene = true;

		CSoundManager::PlaySE("CancelDetermination");
		CSoundManager::FadeOutBGM("TitleBGM");
	}
	// フェードイン状態かつフェード中なら処理しない.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;

	if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
	while( CSoundManager::StopBGMThread("TitleBGM") == false);
	m_pSceneManager->NextSceneMove();
}