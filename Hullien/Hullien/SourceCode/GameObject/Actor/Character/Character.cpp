#include "Character.h"

CCharacter::CCharacter()
	: m_Life		( 1.0f )
	, m_MoveSpeed	( 0.05f )
	, m_MoveVector	( 0.0f, 0.0f, 0.0f )
{
}

CCharacter::~CCharacter()
{
}