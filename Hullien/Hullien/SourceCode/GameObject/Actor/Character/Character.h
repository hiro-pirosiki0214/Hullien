#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"
#include "..\Item\ItemList.h"

class CCharacter : public CActor
{
public:
	CCharacter();
	virtual ~CCharacter();

protected:
	float	m_Life;			// 体力.
	float	m_MoveSpeed;	// 移動速度.
	int		m_HasAnyItem;	// どのアイテムを持っているか.
};

#endif	// #ifndef CHARACTER_H.