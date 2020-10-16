#ifndef BOX_STRUCT_H
#define BOX_STRUCT_H

#include "..\..\Global.h"

// ボックスの構造体.
struct stBoxWall
{
	D3DXVECTOR3 Length;			// ボックスの長さx,y,zの絶対値.
	D3DXVECTOR2	MaxPosition;	// 最大座標 : yをz座標として使用.
	D3DXVECTOR2	MinPosition;	// 最小座標 : yをz座標として使用.
	stBoxWall()
		: Length		( 0.0f, 0.0f, 0.0f )
		, MaxPosition	( 0.0f, 0.0f )
		, MinPosition	( 0.0f, 0.0f )
	{}
} typedef SBoxWall;

#endif	// #ifndef BOX_STRUCT_H.