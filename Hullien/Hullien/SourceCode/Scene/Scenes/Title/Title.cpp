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

CTitle::CTitle(CSceneManager* pSceneManager)
	: CSceneBase(pSceneManager)
	, m_pWidget				( nullptr )
	, m_IsChangeScene		( false )
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
	return true;
}

//============================.
//	更新関数.
//============================.
void CTitle::Update()
{
	CSoundManager::ThreadPlayBGM("TestBGM");

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
		CSoundManager::PlaySE("DeterminationSE");
		m_IsChangeScene = true;
	}

	// フェードイン状態かつフェード中なら処理しない.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;
	// どのシーンに遷移するか.
	switch (m_pWidget->GetSelectState())
	{
	case CTitleWidget::ESelectState::Start:
		CSoundManager::StopBGMThread("TestBGM");
		m_pSceneManager->NextSceneMove();
		break;
	case CTitleWidget::ESelectState::End:
		CSoundManager::StopBGMThread("TestBGM");
		// ウィンドウを閉じる.
		PostMessage(m_pSceneManager->m_hWnd, WM_CLOSE, 0, 0);
		break;
	default:
		break;
	}
}

