#ifndef SPAWN_UFO_PARAM_H
#define SPAWN_UFO_PARAM_H

#include "..\..\Global.h"

// スポーンUFOの情報.
struct stSpawnUFOParam
{
	D3DXVECTOR3 Position;	// 座標.
	int			SpawnTime;	// スポーンする時間.

	stSpawnUFOParam()
		: Position	( 0.0f, 0.0f, 0.0f )
		, SpawnTime	( 0 )
	{}

} typedef SSpawnUFOParam;

#endif	// #ifndef SPAWN_UFO_PARAM_H.