#ifndef ALIEN_A_H
#define ALIEN_A_H

#include "..\Alien.h"

class CAlienA : public CAlien
{
	const char* MODEL_NAME = "GhostB_s";

public:
	CAlienA();
	virtual ~CAlienA();

	// ‰Šú‰»ŠÖ”.
	virtual bool Init() override;
	// XVŠÖ”.
	virtual void Update() override;
	// •`‰æŠÖ”.
	virtual void Render() override;

private:
	// ˆÚ“®ŠÖ”.
	virtual void Move() override;
	
};

#endif	// #ifndef ALIEN_A_H.