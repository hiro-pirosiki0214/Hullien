#include "PlayerEdit.h"

CPlayerEdit::CPlayerEdit()	
	: m_pPlayer	( nullptr )
{
	m_pPlayer = std::make_unique<CPlayer>();
}

CPlayerEdit::~CPlayerEdit()
{
}

// ‰Šú‰»ŠÖ”.
bool CPlayerEdit::Init() 
{
	if( m_pPlayer->Init() == false ) return false;
	return true;
}

// XVŠÖ”.
void CPlayerEdit::Update()
{
	m_pPlayer->Update();
}

// •`‰æŠÖ”.
void CPlayerEdit::Render() 
{
}

// ƒ‚ƒfƒ‹‚Ì•`‰æ.
void CPlayerEdit::ModelRender() 
{
	m_pPlayer->Render();
}