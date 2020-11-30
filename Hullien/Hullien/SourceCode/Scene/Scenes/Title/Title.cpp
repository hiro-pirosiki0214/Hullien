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
	, m_IsDecision		( false )
{
	m_pWidget	= std::make_unique< CTitleWidget >();
	CFade::SetFadeOut();
}

CTitle::~CTitle()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CTitle::Load()
{
	if ( m_pWidget->Init() == false ) return false;

	CSoundManager::ThreadPlayBGM("TitleBGM");
	CSoundManager::FadeInBGM("TitleBGM");
	m_pSceneManager->SetNowBGMName("TitleBGM");
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CTitle::Update()
{
	if (CFade::GetIsFade() == true) return;
	// ���肵�ĂȂ��ꍇUI�̍X�V.
	if( m_IsDecision == false ) m_pWidget->Update();
	//�V�[���؂�ւ�.
	ChangeScene();
}

//============================.
//	�`��֐�.
//============================.
void CTitle::Render()
{
	if ( m_pWidget == nullptr ) return;
	m_pWidget->Render();
}

//============================.
	// �V�[���؂�ւ��֐�.
//============================.
void CTitle::ChangeScene()
{
	// �{�^���������ꂽ.
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
#ifndef IS_CONFIG_RENDER
		case CTitleWidget::ESelectState::Config:
			CSoundManager::PlaySE("Determination");
			break;
#endif	// #ifndef IS_CONFIG_RENDER.
		case CTitleWidget::ESelectState::End:
			CSoundManager::PlaySE("CancelDetermination");
			break;
		default:
			break;
		}
		CSoundManager::FadeOutBGM("TitleBGM");
		m_IsChangeScene = true;
		m_IsDecision = true;
	}

	if (m_IsChangeScene == false) return;
	// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;
	// �ǂ̃V�[���ɑJ�ڂ��邩.
	switch (m_pWidget->GetSelectState())
	{
	case CTitleWidget::ESelectState::Start:
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while( CSoundManager::StopBGMThread("TitleBGM") == false);
		m_pSceneManager->NextSceneMove();
		break;
#ifndef IS_CONFIG_RENDER
	case CTitleWidget::ESelectState::Config:
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while( CSoundManager::StopBGMThread("TitleBGM") == false);
		m_pSceneManager->ConfigSceneMove();
		break;
#endif	// #ifndef IS_CONFIG_RENDER.
	case CTitleWidget::ESelectState::End:
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while (CSoundManager::StopBGMThread("TitleBGM") == false);
		// �E�B���h�E�����.
		m_pSceneManager->EndGameClose();
		break;
	default:
		break;
	}
}

