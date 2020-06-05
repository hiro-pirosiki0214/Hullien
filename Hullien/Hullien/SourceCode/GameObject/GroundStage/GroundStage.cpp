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

// ‰Šú‰»ŠÖ”.
bool CGroundStage::Init()
{
	if( m_pStaticMesh != nullptr ) return true;

	// ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý.
	CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME );
	if( m_pStaticMesh == nullptr ) return false;

	return true;
}

// XVŠÖ”.
void CGroundStage::Update()
{
}

// •`‰æŠÖ”.
void CGroundStage::Render()
{
	if( m_pStaticMesh == nullptr ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetScale( m_vSclae );
	m_pStaticMesh->Render();	// ƒ‚ƒfƒ‹‚Ì•`‰æ.

}