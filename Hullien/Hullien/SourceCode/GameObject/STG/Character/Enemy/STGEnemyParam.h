#ifndef STG_ENEMY_PARAM_H
#define STG_ENEMY_PARAM_H

#include <string>

namespace STG
{
	// 敵のパラメータ.
	struct stEnemyParam
	{
		float		SpawnTime;			// 出現時間.
		float		PositionX;			// 座標 : X.
		std::string	Text;				// テキスト.
		float		MoveSpeed;			// 移動速度.
		float		BulletSpeed;		// 弾の速度.
		int			BulletCollDisappear;// 弾が衝突した際に消えるか.
		float		BulletAngle;		// 弾の角度.
		int			BulletCountMax;		// 弾の最大数.
		int			AnyBulletCountMax;	// 複数弾を撃つ際の最大数.
		int			ShotIntervalFrame;	// 弾を撃つ間隔フレーム.
		int			ShotBulletCount;	// 一発で出る弾数.
		float		ShotAngle;			// 撃つ時の角度.
		int			ShotNumber;			// 弾の番号.
	} typedef SEnemyParam;

	// 敵のパラメータ番号.
	enum enEnemyParamNo
	{
		EEnemyParamNo_None,

		EEnemyParamNo_SpawnTime,			// 出現時間.
		EEnemyParamNo_PositionX,			// 座標 : X.
		EEnemyParamNo_pText,				// テキスト.
		EEnemyParamNo_MoveSpeed,			// 移動速度.
		EEnemyParamNo_BulletSpeed,			// 弾の速度.
		EEnemyParamNo_BulletCollDisappear,	// 弾が衝突した際に消えるか.
		EEnemyParamNo_BulletAngle,			// 弾の角度.
		EEnemyParamNo_BulletCountMax,		// 弾の最大数.
		EEnemyParamNo_AnyBulletCountMax,	// 複数弾を撃つ際の最大数.
		EEnemyParamNo_ShotIntervalFrame,	// 弾を撃つ間隔フレーム.
		EEnemyParamNo_ShotBulletCount,		// 一発で出る弾数.
		EEnemyParamNo_ShotAngle,			// 撃つ時の角度.
		EEnemyParamNo_ShotNumber,			// 弾の番号.

		EEnemyParamNo_Max,

		EEnemyParamNo_Start	= EEnemyParamNo_SpawnTime,
		EEnemyParamNo_End	= EEnemyParamNo_ShotNumber,
	} typedef EEnemyParamNo;
};
#endif	// #ifndef STG_ENEMY_PARAM_H.