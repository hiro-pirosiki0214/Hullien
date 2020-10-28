#include "UFOLight.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

CUFOLight::CUFOLight()
	: m_pStaticMesh	( nullptr )
{
}

CUFOLight::~CUFOLight()
{
}

// ‰Šú‰»ŠÖ”.
bool CUFOLight::Init()
{
	if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME ) == false ) return false;

	return true;
}

// XVŠÖ”.
void CUFOLight::Update()
{
}

// •`‰æŠÖ”.
void CUFOLight::Render()
{
	if( m_pStaticMesh == nullptr ) return;
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetScale( m_vScale );
	m_pStaticMesh->SetColor( { 5.0f, 5.0f, 5.0f, 0.3f } );
	m_pStaticMesh->SetCoverage( true );
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pStaticMesh->Render( true );
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pStaticMesh->SetCoverage( false );
}