#include "EventUFO.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

/**************************************
*	イベント用UFOクラス.
**/
CEventUFO::CEventUFO()
	: m_pStaticMesh( nullptr )
{
}

CEventUFO::~CEventUFO()
{
}


// 初期化関数.
bool CEventUFO::Init()
{
	if (GetModel() == false) return false;
	m_vPosition.y = POS_HEIGHT;
	return true;
}

// 更新関数.
void CEventUFO::Update()
{
}

// 描画関数.
void CEventUFO::Render()
{
	if ( m_pStaticMesh == nullptr ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetScale( m_vSclae );
	m_pStaticMesh->Render();
}

// モデルの取得.
bool CEventUFO::GetModel()
{
	if ( m_pStaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic(m_pStaticMesh, MODEL_NAME);
	if ( m_pStaticMesh == nullptr ) return false;
	return true;
}
