#ifndef INVISIBLE_WALL_EDIT_H
#define INVISIBLE_WALL_EDIT_H

#include "..\EditBase.h"

class CInvisibleWallEdit : public CEditBase
{
public:
	CInvisibleWallEdit();
	virtual ~CInvisibleWallEdit();

	// ‰Šú‰»ŠÖ”.
	virtual bool Init() override;
	// XVŠÖ”.
	virtual void Update() override;
	// •`‰æŠÖ”.
	virtual void Render() override;
	// ƒ‚ƒfƒ‹‚Ì•`‰æ.
	virtual void ModelRender() override;
};

#endif	// #ifndef INVISIBLE_WALL_EDIT_H.