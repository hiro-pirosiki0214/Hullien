#include "..\SceneList.h"

CGameClear::CGameClear( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
{
}

CGameClear::~CGameClear()
{
}

//============================.
//	“ÇŠÖ”.
//============================.
bool CGameClear::Load()
{
	return true;
}

//============================.
//	XVŠÖ”.
//============================.
void CGameClear::Update()
{
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
}

//============================.
//	•`‰æŠÖ”.
//============================.
void CGameClear::Render()
{
}