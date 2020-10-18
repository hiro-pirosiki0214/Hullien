#include "STGActor.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"

STG::CActor::CActor()
	: m_pStaticMesh	( nullptr )
	, m_MoveVector	( 0.0f, 0.0f, 0.0f )
{
}

STG::CActor::~CActor()
{
}

// メッシュの描画.
void STG::CActor::MeshRender()
{
	if( m_pStaticMesh == nullptr ) return;
	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetScale( m_vSclae );
	m_pStaticMesh->Render();
}