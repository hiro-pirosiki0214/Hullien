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
		float		TextSize;			// テキストサイズ.
		float		LifePoint;			// ライフ.
		float		MoveSpeed;			// 移動速度.
		float		BulletSpeed;		// 弾の速度.
		int			BulletCountMax;		// 弾数.
		int			AnyBulletCountMax;	// nWay数.
		float		BulletAngle;		// 角度.
		float		ShotAngle;			// nWay角度.
		int			ShotIntervalFrame;	// 弾を撃つ間隔.
		int			BulletCollDisappear;// 弾が衝突した際に消えるか.

		stEnemyParam()
			: SpawnTime				( 0.0f )
			, PositionX				( 0.0f )
			, Text					( "" )
			, TextSize				( 0.0f )
			, LifePoint				( 0.0f )
			, MoveSpeed				( 0.0f )
			, BulletSpeed			( 0.0f )
			, BulletCountMax		( 0 )
			, AnyBulletCountMax		( 0 )
			, BulletAngle			( 0.0f )
			, ShotAngle				( 0.0f )
			, ShotIntervalFrame		( 0 )
			, BulletCollDisappear	( 0 )
		{}
	} typedef SEnemyParam;

	// 敵のパラメータ番号.
	enum enEnemyParamNo
	{
		EEnemyParamNo_None,

		EEnemyParamNo_SpawnTime,			// 出現時間.
		EEnemyParamNo_PositionX,			// 座標 : X.
		EEnemyParamNo_Text,					// テキスト.
		EEnemyParamNo_TextSize,				// テキストサイズ.
		EEnemyParamNo_LifePoint,			// 体力.
		EEnemyParamNo_MoveSpeed,			// 移動速度.
		EEnemyParamNo_BulletSpeed,			// 弾の速度.
		EEnemyParamNo_BulletCountMax,		// 弾の最大数.
		EEnemyParamNo_AnyBulletCountMax,	// 複数弾を撃つ際の最大数.
		EEnemyParamNo_BulletAngle,			// 弾の角度.
		EEnemyParamNo_ShotAngle,			// 撃つ時の角度.
		EEnemyParamNo_ShotIntervalFrame,	// 弾を撃つ間隔フレーム.
		EEnemyParamNo_BulletCollDisappear,	// 弾が衝突した際に消えるか.

		EEnemyParamNo_Max,

		EEnemyParamNo_Start	= EEnemyParamNo_SpawnTime,
		EEnemyParamNo_End	= EEnemyParamNo_BulletCollDisappear,
	} typedef EEnemyParamNo;
};
#endif	// #ifndef STG_ENEMY_PARAM_H.