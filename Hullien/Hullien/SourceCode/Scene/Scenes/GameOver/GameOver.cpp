#include "..\SceneList.h"

CGameOver::CGameOver( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
{
}

CGameOver::~CGameOver()
{
}

//============================.
//	“ÇŠÖ”.
//============================.
bool CGameOver::Load()
{
	return true;
}

//============================.
//	XVŠÖ”.
//============================.
void CGameOver::Update()
{
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
}

//============================.
//	•`‰æŠÖ”.
//============================.
void CGameOver::Render()
{
}