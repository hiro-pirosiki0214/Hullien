#ifndef INVISIBLE_WALL_EDIT_H
#define INVISIBLE_WALL_EDIT_H

#include "..\EditBase.h"

class CInvisibleWallEdit : public CEditBase
{
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
};

#endif	// #ifndef INVISIBLE_WALL_EDIT_H.