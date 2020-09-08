#include "SkyDome.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

CSkyDome::CSkyDome()
	: m_StaticMesh	( nullptr )
{
	m_vPosition.y = -10.0f;
}

CSkyDome::~CSkyDome()
{
}

// 初期化関数.
bool CSkyDome::Init()
{
	if( GetModel() == false ) return false;
	return true;
}

// 更新関数.
void CSkyDome::Update()
{
}

// 描画関数.
void CSkyDome::Render()
{
	if( m_StaticMesh == nullptr ) return;

	m_StaticMesh->SetPosition( m_vPosition );
	m_StaticMesh->Render();

}

// モデルの取得.
bool CSkyDome::GetModel()
{
	if( m_StaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic( m_StaticMesh, MODEL_NAME );
	if( m_StaticMesh == nullptr ) return false;

	return true;
}