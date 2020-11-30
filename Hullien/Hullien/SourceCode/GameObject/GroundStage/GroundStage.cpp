#include "GroundStage.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

CGroundStage::CGroundStage()
	: m_pStaticMesh		( nullptr )
{
}

CGroundStage::~CGroundStage()
{
}

// �������֐�.
bool CGroundStage::Init()
{
	if( m_pStaticMesh != nullptr ) return true;

	// ���f���̓ǂݍ���.
	CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME );
	if( m_pStaticMesh == nullptr ) return false;

	return true;
}

// �X�V�֐�.
void CGroundStage::Update()
{
}

// �`��֐�.
void CGroundStage::Render()
{
	if( m_pStaticMesh == nullptr ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetScale( m_vScale );
	m_pStaticMesh->SetIsShadow( true );
	m_pStaticMesh->SetRasterizerState( ERS_STATE::Front );	// �O�ʂ�`�悵�Ȃ�.
	m_pStaticMesh->Render();	// ���f���̕`��.
	m_pStaticMesh->SetRasterizerState( ERS_STATE::None );
	m_pStaticMesh->SetIsShadow( false );

}