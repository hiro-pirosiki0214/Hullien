#include "..\SceneList.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\GameObject\Widget\SceneWidget\TItleWidget\TitleWidget.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"

CTitle::CTitle( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_pWidget			( nullptr )
	, m_IsChangeScene	( false )
{
	m_pWidget	= std::make_unique< CTitleWidget >();
	CFade::SetFadeOut();
}

CTitle::~CTitle()
{
}

//============================.
//	読込関数.
//============================.
bool CTitle::Load()
{
	if ( m_pWidget->Init() == false ) return false;

	CSoundManager::ThreadPlayBGM("TitleBGM");
	CSoundManager::FadeInBGM("TitleBGM");
	return true;
}

//============================.
//	更新関数.
//============================.
void CTitle::Update()
{
	if (CFade::GetIsFade() == true) return;

	m_pWidget->Update();
	//シーン切り替え.
	ChangeScene();
}

//============================.
//	描画関数.
//============================.
void CTitle::Render()
{
	if ( m_pWidget == nullptr ) return;
	m_pWidget->Render();
}

//============================.
	// シーン切り替え関数.
//============================.
void CTitle::ChangeScene()
{
	// ボタンが押された.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enSEPARATED)
	{
		if (m_IsChangeScene == true) return;
		CFade::SetFadeIn();
		switch (m_pWidget->GetSelectState())
		{
		case CTitleWidget::ESelectState::Start:
			CSoundManager::PlaySE("Determination");
			break;
		case CTitleWidget::ESelectState::Config:
			CSoundManager::PlaySE("Determination");
			break;
		case CTitleWidget::ESelectState::End:
			CSoundManager::PlaySE("CancelDetermination");
			break;
		default:
			break;
		}
		CSoundManager::FadeOutBGM("TitleBGM");
		m_IsChangeScene = true;
	}

	// フェードイン状態かつフェード中なら処理しない.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;
	// どのシーンに遷移するか.
	switch (m_pWidget->GetSelectState())
	{
	case CTitleWidget::ESelectState::Start:
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while( CSoundManager::StopBGMThread("TitleBGM") == false);
		m_pSceneManager->NextSceneMove();
		break;
	case CTitleWidget::ESelectState::Config:
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while( CSoundManager::StopBGMThread("TitleBGM") == false);
		m_pSceneManager->ConfigSceneMove();
		break;
	case CTitleWidget::ESelectState::End:
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while (CSoundManager::StopBGMThread("TitleBGM") == false);
		// ウィンドウを閉じる.
		m_pSceneManager->EndGameClose();
		break;
	default:
		break;
	}
}

