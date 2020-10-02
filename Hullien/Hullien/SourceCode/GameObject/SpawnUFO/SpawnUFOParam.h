#ifndef SPAWN_UFO_PARAM_H
#define SPAWN_UFO_PARAM_H

#include "..\..\Global.h"

// スポーンUFOの情報.
struct stSpawnUFOParam
{
	D3DXVECTOR3 Position;				// 座標.
	float		SpawnPointHight;		// スポーンポイントの高さ.
	int			SpawnTime;				// スポーンする時間.
	int			MaxAlienCount;			// 宇宙人の最大数.
	int			ProbabilityC;			// 宇宙人Cの出現率.
	int			ProbabilityD;			// 宇宙人Dの出現率.
	int			ProbabilityItem;		// アイテムを落とす確率.

	stSpawnUFOParam()
		: Position					( 0.0f, 0.0f, 0.0f )
		, SpawnPointHight			( 0.0f )
		, SpawnTime					( 0 )
		, MaxAlienCount				( 0 )
		, ProbabilityC				( 0 )
		, ProbabilityD				( 0 )
		, ProbabilityItem			( 0 )
	{}

} typedef SSpawnUFOParam;

#endif	// #ifndef SPAWN_UFO_PARAM_H.