#include "EditAlien_A.h"
#include "..\..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"

CEditAlienA::CEditAlienA()
{}

CEditAlienA::~CEditAlienA()
{}

// ‰Šú‰»ŠÖ”.
bool CEditAlienA::Init()
{
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( EffectSetting()				== false ) return false;
	m_pSkinMesh->ChangeAnimSet_StartPos( alien::EAnimNo_Move, 0.0f, m_pAC );
	return true;
}

// XVŠÖ”.
void CEditAlienA::Update()
{}

// •`‰æŠÖ”.
void CEditAlienA::Render()
{}

// “–‚½‚è”»’èŠÖ”.
void CEditAlienA::Collision( CActor* pActor )
{}