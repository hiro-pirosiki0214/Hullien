#include "UFOLight.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

CUFOLight::CUFOLight()
	: m_pStaticMesh	( nullptr )
	, m_NowState	( EUFOLightState::None )
{
	m_vScale = { 0.0f, 0.0f, 0.0f };
}

CUFOLight::~CUFOLight()
{
}

// èâä˙âªä÷êî.
bool CUFOLight::Init()
{
	if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME ) == false ) return false;

	return true;
}

// çXêVä÷êî.
void CUFOLight::Update()
{
	switch( m_NowState )
	{
	case EUFOLightState::Discharge:
		if( m_vScale.y < SCALE_MAX ){
			m_vScale.y += SCALE_UP_HEIGHT_SPEED;
			if( m_vScale.y >= SCALE_MAX ){
				m_vScale.y = SCALE_MAX;
			}
		}
		if( m_vScale.x < SCALE_MAX ){
			m_vScale.x += SCALE_UP_WEDTH_SPEED;
			m_vScale.z += SCALE_UP_WEDTH_SPEED;
			if( m_vScale.x >= SCALE_MAX ){
				m_vScale.x = SCALE_MAX;
				m_vScale.z = SCALE_MAX;
				m_NowState = EUFOLightState::EndDischarge;
			}
		}
		break;
	case EUFOLightState::CleanUP:
		if( m_vScale.y > 0.0f ){
			m_vScale.y -= SCALE_UP_WEDTH_SPEED;
			if( m_vScale.y <= 0.0f ){
				m_vScale.y = 0.0f;
			}
		}
		if( m_vScale.x > 0.0f ){
			m_vScale.x -= SCALE_UP_HEIGHT_SPEED;
			m_vScale.z -= SCALE_UP_HEIGHT_SPEED;
			if( m_vScale.x <= 0.0f ){
				m_vScale.x = 0.0f;
				m_vScale.z = 0.0f;
				m_NowState = EUFOLightState::EndCleanUP;
			}
		}
		break;
	case EUFOLightState::EndDischarge:
		break;
	case EUFOLightState::EndCleanUP:
		break;
	default:
		break;
	}
}

// ï`âÊä÷êî.
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

// åıÇï˙èoÇ∑ÇÈ.
void CUFOLight::Discharge()
{
	m_NowState = EUFOLightState::Discharge;
}

// åıÇï–Ç√ÇØÇÈ.
void CUFOLight::CleanUP()
{
	m_NowState = EUFOLightState::CleanUP;
}