#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"

class CCharacter : public CActor
{
public:
	CCharacter();
	virtual ~CCharacter();

	// ˆÚ“®ŠÖ”.
	virtual void Move() = 0;

protected:
	float		m_Life;			// ‘Ì—Í.
	D3DXVECTOR3	m_MoveVector;	// ˆÚ“®ƒxƒNƒgƒ‹.
};

#endif	// #ifndef CHARACTER_H.