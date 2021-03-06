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

// 初期化関数.
bool CGroundStage::Init()
{
	if( m_pStaticMesh != nullptr ) return true;

	// モデルの読み込み.
	CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME );
	if( m_pStaticMesh == nullptr ) return false;

	return true;
}

// 更新関数.
void CGroundStage::Update()
{
}

// 描画関数.
void CGroundStage::Render()
{
	if( m_pStaticMesh == nullptr ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetScale( m_vScale );
	m_pStaticMesh->SetIsShadow( true );
	m_pStaticMesh->SetRasterizerState( ERS_STATE::Front );	// 前面を描画しない.
	m_pStaticMesh->Render();	// モデルの描画.
	m_pStaticMesh->SetRasterizerState( ERS_STATE::None );
	m_pStaticMesh->SetIsShadow( false );

}