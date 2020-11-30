#ifndef INVISIBLE_WALL_EDIT_H
#define INVISIBLE_WALL_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\InvisibleWall\BoxStruct.h"

class CInvisibleWall;	// 見えない壁.

class CInvisibleWallEdit : public CEditBase
{
	const char* INVISIBLE_WALL_PARAM_FILE_NAME = "Data\\GameParam\\InvisibleWall.bin";
public:
	CInvisibleWallEdit();
	virtual ~CInvisibleWallEdit();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// モデルの描画.
	virtual void ModelRender() override;
	// エフェクトの描画.
	virtual void EffectRender() override;

private:
	std::unique_ptr<CInvisibleWall>	m_pInvisibleWall;	// 見えない壁.
	SBoxWall	m_BoxWall;
};

#endif	// #ifndef INVISIBLE_WALL_EDIT_H.