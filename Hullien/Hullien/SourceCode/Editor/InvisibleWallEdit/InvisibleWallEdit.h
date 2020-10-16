#ifndef INVISIBLE_WALL_EDIT_H
#define INVISIBLE_WALL_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\InvisibleWall\BoxStruct.h"

class CInvisibleWall;	// Œ©‚¦‚È‚¢•Ç.

class CInvisibleWallEdit : public CEditBase
{
	const char* INVISIBLE_WALL_PARAM_FILE_NAME = "Data\\GameParam\\InvisibleWall.bin";
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

private:
	std::unique_ptr<CInvisibleWall>	m_pInvisibleWall;	// Œ©‚¦‚È‚¢•Ç.
	SBoxWall	m_BoxWall;
};

#endif	// #ifndef INVISIBLE_WALL_EDIT_H.