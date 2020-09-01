#include "..\SceneList.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\GameObject\Widget\SceneWidget\TItleWidget\TitleWidget.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"

CTitle::CTitle(CSceneManager* pSceneManager)
	: CSceneBase(pSceneManager)
	, m_pWidget				(nullptr)
	, m_pFade				(nullptr)
	, m_IsChangeScene	( false )
{
	m_pWidget	= std::make_unique< CTitleWidget >();
	m_pFade		= std::make_unique<CFade>();
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
	if ( m_pFade->Init() == false ) return false;
	m_pFade->SetFadeOut(ALPHA_SPEED);

	return true;
}

//============================.
//	更新関数.
//============================.
void CTitle::Update()
{
	if (m_pFade->GetFadeState() == CWidget::EFadeState::Out
		&& m_pFade->GetIsFade() == true)
	{
		m_pFade->Update();
	}
	else
	{
		m_pWidget->Update();
		//シーン切り替え.
		ChangeScene();
	}

}

//============================.
//	描画関数.
//============================.
void CTitle::Render()
{
	if ( m_pWidget == nullptr ) return;
	m_pWidget->Render();
	m_pFade->Render();
}

//============================.
	// シーン切り替え関数.
//============================.
void CTitle::ChangeScene()
{
	// ボタンが押された.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		m_pFade->SetFadeIn(ALPHA_SPEED);
		m_IsChangeScene = true;
	}

	// シーン切り替えフラグが立っていなければ処理しない.
	if (m_IsChangeScene == false) return;
	m_pFade->Update();

	// フェードイン状態かつフェード中なら処理しない.
	if (m_pFade->GetFadeState() == CWidget::EFadeState::In
		&& m_pFade->GetIsFade() == true) return;
	// どのシーンに遷移するか.
	switch (m_pWidget->GetSelectState())
	{
	case CTitleWidget::ESelectState::Start:
		m_pSceneManager->NextSceneMove();
		break;
	case CTitleWidget::ESelectState::End:
			// ウィンドウを閉じる処理を追加する.
		break;
	default:
		break;
	}
}

