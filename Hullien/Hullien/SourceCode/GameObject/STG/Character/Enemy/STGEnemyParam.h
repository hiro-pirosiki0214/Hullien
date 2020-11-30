#ifndef STG_ENEMY_PARAM_H
#define STG_ENEMY_PARAM_H

#include <string>

namespace STG
{
	// �G�̃p�����[�^.
	struct stEnemyParam
	{
		float		SpawnTime;			// �o������.
		float		PositionX;			// ���W : X.
		std::string	Text;				// �e�L�X�g.
		float		TextSize;			// �e�L�X�g�T�C�Y.
		float		LifePoint;			// ���C�t.
		float		MoveSpeed;			// �ړ����x.
		float		BulletSpeed;		// �e�̑��x.
		int			BulletCountMax;		// �e��.
		int			AnyBulletCountMax;	// nWay��.
		float		BulletAngle;		// �p�x.
		float		ShotAngle;			// nWay�p�x.
		int			ShotIntervalFrame;	// �e�����Ԋu.
		int			BulletCollDisappear;// �e���Փ˂����ۂɏ����邩.

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

	// ���݂̏��.
	enum class enEnemyState
	{
		None,

		Spawn,	// �X�|�[��.
		Move,	// �ړ�.
		Shot,	// ����.
		Escape,	// ������.
		Dead,	// ���S.

		Max,
	} typedef EEnemyState;

	// �G�̃p�����[�^�ԍ�.
	enum enEnemyParamNo
	{
		EEnemyParamNo_None,

		EEnemyParamNo_SpawnTime,			// �o������.
		EEnemyParamNo_PositionX,			// ���W : X.
		EEnemyParamNo_Text,					// �e�L�X�g.
		EEnemyParamNo_TextSize,				// �e�L�X�g�T�C�Y.
		EEnemyParamNo_LifePoint,			// �̗�.
		EEnemyParamNo_MoveSpeed,			// �ړ����x.
		EEnemyParamNo_BulletSpeed,			// �e�̑��x.
		EEnemyParamNo_BulletCountMax,		// �e�̍ő吔.
		EEnemyParamNo_AnyBulletCountMax,	// �����e�����ۂ̍ő吔.
		EEnemyParamNo_BulletAngle,			// �e�̊p�x.
		EEnemyParamNo_ShotAngle,			// �����̊p�x.
		EEnemyParamNo_ShotIntervalFrame,	// �e�����Ԋu�t���[��.
		EEnemyParamNo_BulletCollDisappear,	// �e���Փ˂����ۂɏ����邩.

		EEnemyParamNo_Max,

		EEnemyParamNo_Start	= EEnemyParamNo_SpawnTime,
		EEnemyParamNo_End	= EEnemyParamNo_BulletCollDisappear,
	} typedef EEnemyParamNo;
};
#endif	// #ifndef STG_ENEMY_PARAM_H.