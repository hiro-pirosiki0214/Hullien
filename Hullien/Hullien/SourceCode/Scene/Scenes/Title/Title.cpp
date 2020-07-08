#include "..\SceneList.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\DebugText\DebugText.h"


CTitle::CTitle( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
	, m_pCTest		(nullptr)
{
}

CTitle::~CTitle()
{
}

//============================.
//	読込関数.
//============================.
bool CTitle::Load()
{
	m_pCTest = CSpriteResource::GetSprite( "pokemon" );

	return true;
}

//============================.
//	更新関数.
//============================.
void CTitle::Update()
{
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
}

//============================.
//	描画関数.
//============================.
void CTitle::Render()
{
	if (m_pCTest == nullptr) return;

	m_pCTest->RenderUI();
}