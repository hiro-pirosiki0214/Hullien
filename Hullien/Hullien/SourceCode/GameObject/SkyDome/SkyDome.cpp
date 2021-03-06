#include "SkyDome.h"
#include "..\..\Common\SkyDomeModel\SkyDomeModel.h"
#include "..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

CSkyDome::CSkyDome()
	: m_pDomeModel	( nullptr )
{
	m_pDomeModel = std::make_unique<CSkyDomeModel>();
}

CSkyDome::~CSkyDome()
{
	m_pDomeModel->Release();
}

// 初期化関数.
bool CSkyDome::Init()
{
	if( FAILED( m_pDomeModel->Init(MODEL_NAME) )) return false;
	return true;
}

// 更新関数.
void CSkyDome::Update()
{
}

// 描画関数.
void CSkyDome::Render()
{
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ) return;

	m_pDomeModel->SetRotation( m_vRotation );
	m_pDomeModel->Render();
}