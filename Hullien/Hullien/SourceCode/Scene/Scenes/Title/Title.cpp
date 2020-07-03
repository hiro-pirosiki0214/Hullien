#include "..\SceneList.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\GameObject\Widget\SceneWidget\TItleWidget\TitleWidget.h"

CTitle::CTitle(CSceneManager* pSceneManager)
	: CSceneBase(pSceneManager)
	, m_pWidget(nullptr)
{
	m_pWidget = std::make_unique< CTitleWidget >();
}

CTitle::~CTitle()
{
}

//============================.
//	読込関数.
//============================.
bool CTitle::Load()
{
	if (m_pWidget->Init() == false) return false;

	return true;
}

//============================.
//	更新関数.
//============================.
void CTitle::Update()
{
	m_pWidget->Update();

	switch ( m_pWidget->GetSelectState() )
	{
	case CTitleWidget::ESelectState::Start:
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			m_pSceneManager->NextSceneMove();
		}
		break;
	default:
		break;
	}
}

//============================.
//	描画関数.
//============================.
void CTitle::Render()
{
	m_pWidget->Render();
}