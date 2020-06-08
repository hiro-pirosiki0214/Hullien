#ifndef ALIEN_A_H
#define ALIEN_A_H

#include "..\Alien.h"

class CAlienA : public CAlien
{
public:
	CAlienA();
	virtual ~CAlienA();

	// ‰Šú‰»ŠÖ”.
	virtual bool Init() override;
	// XVŠÖ”.
	virtual void Update() override;
	// •`‰æŠÖ”.
	virtual void Render() override;
};

#endif	// #ifndef ALIEN_A_H.