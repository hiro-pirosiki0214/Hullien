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
//	“ÇžŠÖ”.
//============================.
bool CTitle::Load()
{
	if (m_pWidget->Init() == false) return false;

	return true;
}

//============================.
//	XVŠÖ”.
//============================.
void CTitle::Update()
{
	m_pWidget->Update();

	switch ( m_pWidget->GetSelectState() )
	{
	case CTitleWidget::ESelectState::Start:
		if (GetAsyncKeyState(VK_RETURN) & 0x0001) {
			m_pSceneManager->NextSceneMove();
		}
		break;
	default:
		break;
	}
}

//============================.
//	•`‰æŠÖ”.
//============================.
void CTitle::Render()
{
	if ( m_pWidget == nullptr ) return;
	m_pWidget->Render();
}