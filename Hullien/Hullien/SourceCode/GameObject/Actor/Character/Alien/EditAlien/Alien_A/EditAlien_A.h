#ifndef EDIT_ALIEN_A_H
#define EDIT_ALIEN_A_H

#include "..\EditAlien.h"

class CEditAlienA : public CEditAlien
{
	const char* MODEL_NAME = "a_s";
public:
	CEditAlienA();
	virtual	~CEditAlienA();

	// ‰Šú‰»ŠÖ”.
	virtual bool Init() override;
	// XVŠÖ”.
	virtual void Update() override;
	// •`‰æŠÖ”.
	virtual void Render() override;
	// “–‚½‚è”»’èŠÖ”.
	virtual void Collision( CActor* pActor ) override;
};

#endif	// #ifndef EDIT_ALIEN_A_H.
