#ifndef SPAWN_UFO_PARAM_H
#define SPAWN_UFO_PARAM_H

#include "..\..\Global.h"

// スポーンUFOの情報.
struct stSpawnUFOParam
{
	D3DXVECTOR3 Position;				// 座標.
	int			SpawnTime;				// スポーンする時間.
	int			MaxAlienCount;			// 宇宙人の最大数.
	int			AlienCWaveTime;			// 宇宙人Cのウェーブ時間.
	int			AlienCWaveIntervalTime;	// 宇宙人Cのウェーブの間隔時間.
	int			AlienDWaveTime;			// 宇宙人Dのウェーブ時間.
	int			AlienDWaveIntervalTime;	// 宇宙人Dのウェーブの間隔時間.

	stSpawnUFOParam()
		: Position					( 0.0f, 0.0f, 0.0f )
		, SpawnTime					( 0 )
		, MaxAlienCount				( 0 )
		, AlienCWaveTime			( 0 )
		, AlienCWaveIntervalTime	( 0 )
		, AlienDWaveTime			( 0 )
		, AlienDWaveIntervalTime	( 0 )
	{}

} typedef SSpawnUFOParam;

#endif	// #ifndef SPAWN_UFO_PARAM_H.