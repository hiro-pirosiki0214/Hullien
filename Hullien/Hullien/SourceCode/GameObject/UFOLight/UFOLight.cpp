#include "UFOLight.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

CUFOLight::CUFOLight()
	: CUFOLight	( DEFAULT_SCALE_HEIGHT_MAX, DEFAULT_SCALE_WEDTH_MAX )
{
}

CUFOLight::CUFOLight( const float& height, const float& wedht )
	: SCALE_HEIGHT_MAX	( height )
	, SCALE_WEDTH_MAX	( wedht )
	, m_pStaticMesh		( nullptr )
	, m_NowState		( EUFOLightState::None )
{
	m_vScale = { 0.0f, 0.0f, 0.0f };
}

CUFOLight::~CUFOLight()
{
}

// �������֐�.
bool CUFOLight::Init()
{
	if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME ) == false ) return false;

	return true;
}

// �X�V�֐�.
void CUFOLight::Update()
{
	switch( m_NowState )
	{
	case EUFOLightState::Discharge:
		if( m_vScale.y < SCALE_HEIGHT_MAX ){
			m_vScale.y += SCALE_UP_HEIGHT_SPEED;
			if( m_vScale.y >= SCALE_HEIGHT_MAX ){
				m_vScale.y = SCALE_HEIGHT_MAX;
			}
		}
		if( m_vScale.x < SCALE_WEDTH_MAX ){
			m_vScale.x += SCALE_UP_WEDTH_SPEED;
			m_vScale.z += SCALE_UP_WEDTH_SPEED;
			if( m_vScale.x >= SCALE_WEDTH_MAX ){
				m_vScale.x = SCALE_WEDTH_MAX;
				m_vScale.z = SCALE_WEDTH_MAX;
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

// �`��֐�.
void CUFOLight::Render()
{
	if( m_pStaticMesh == nullptr ) return;
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetScale( m_vScale );
	m_pStaticMesh->SetColor( { 2.0f, 2.0f, 2.0f, 0.5f } );
	m_pStaticMesh->SetCoverage( true );
	m_pStaticMesh->SetRasterizerState( ERS_STATE::Back );
	m_pStaticMesh->Render( true );
	m_pStaticMesh->SetRasterizerState( ERS_STATE::None );
	m_pStaticMesh->SetCoverage( false );
}

// ������o����.
void CUFOLight::Discharge()
{
	m_NowState = EUFOLightState::Discharge;
}

// ����ЂÂ���.
void CUFOLight::CleanUP()
{
	m_NowState = EUFOLightState::CleanUP;
}

// �������S�ɕ��o����.
void CUFOLight::DischargePreparation()
{
	m_vScale = { SCALE_WEDTH_MAX, SCALE_HEIGHT_MAX, SCALE_WEDTH_MAX };
}

// ����ЂÂ���.
void CUFOLight::CleanUPPreparation()
{
	m_vScale = { 0.0f, 0.0f, 0.0f };
}