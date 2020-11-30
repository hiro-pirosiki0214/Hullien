#ifndef ALIEN_PARAM_H
#define ALIEN_PARAM_H

#include "..\Character.h"

// �F���l�p�����[�^.
struct stAlienParam : public CCharacter::SParameter
{
	// ���ʃp�����[�^---------------------.
	float		RotationalSpeed;			// ��]���x.

	float		SpawnScaleAddValue;			// �X�|�[�����̃X�P�[���̉��Z�l.
	float		SpawnDownSpeed;				// �X�|�[�����̍~�����x.
	float		MotherShipUpScaleSubValue;	// �}�U�[�V�b�v�ɏ㏸���鎞�̃X�P�[���̌��Z�l.

	int			WaitTime;					// �ҋ@����.
	int			KnockBackTime;				// �m�b�N�o�b�N�̎���.

	float		DeadCountAddValue;			// ���S�J�E���g�̉��Z�l.
	float		DeadScaleSubValue;			// ���S���̃X�P�[���̌��Z�l.

											// �F���lB�p�����[�^------------------.
	float		PlayerAimLenght;			// �v���C���[��_���͈�.

	float		ResearchLenght;				// �Č�������ۂ̋���.

	float		AttackLenght;				// �U�����鋗��.
	float		AttackRotInitPower;			// �U�����̏�����]��.
	float		AttackRotPower;				// �U�����̉�]��.
	float		AttackRotAddValue;			// �U�����̉�]���Z�l.
	float		AttackMoveSpeed;			// �U���ړ����x.
	float		AttackMoveRange;			// �U���ړ��͈�.

											// �F���lD�p�����[�^------------------.
	float		LaserMoveSpeed;				// ���[�U�[�̈ړ����x.
	float		ParalysisTime;				// ��Ⴢ̎���.

	float		AttackRangeSpritePosY;		// �U�����͈͕̔\���̒����p.
	float		AttackRangeSpriteScale;		// �U�����͈͕̔\���̉摜�T�C�Y.
	float		AttackRangeAddValue;		// �U�����͈͕̔\���̃A���t�@���������l.
	float		AttackRangeSubValue;		// �U�����͈͕̔\���̃A���t�@���������l.

	float		ControlPointOneLenght;		// �x�W�F�Ȑ��̈�ڑ�����W�̋���.
	float		ControlPointOneLenghtY;		// �x�W�F�Ȑ��̈�ڑ�����W��y���W�̋���.
	float		ControlPointTwoLenght;		// �x�W�F�Ȑ��̓�ڑ�����W�̋���.
	float		ControlPointTwoLenghtY;		// �x�W�F�Ȑ��̓�ڑ�����W��y���W�̋���.

	D3DXVECTOR3 SphereAdjPos;				// �X�t�B�A�̒������W.
	float		SphereAdjRadius;			// �X�t�B�A�̒������a.

	stAlienParam()
		: RotationalSpeed			( 0.0f )

		, SpawnScaleAddValue		( 0.0f )
		, SpawnDownSpeed			( 0.0f )
		, MotherShipUpScaleSubValue	( 0.0f )

		, WaitTime					( 0 )
		, KnockBackTime				( 0 )

		, DeadCountAddValue			( 0.0f )
		, DeadScaleSubValue			( 0.0f )

		, PlayerAimLenght			( 0.0f )
		, ResearchLenght			( 0.0f )

		, AttackLenght				( 0.0f )
		, AttackRotInitPower		( 0.0f )
		, AttackRotPower			( 0.0f )
		, AttackRotAddValue			( 0.0f )
		, AttackMoveSpeed			( 0.0f )
		, AttackMoveRange			( 0.0f )

		, LaserMoveSpeed			( 0.0f )
		, ParalysisTime				( 0.0f )

		, AttackRangeSpritePosY		( 0.0f )
		, AttackRangeSpriteScale	( 1.0f )
		, AttackRangeSubValue		( 0.0f )

		, ControlPointOneLenght		( 0.0f )
		, ControlPointOneLenghtY	( 0.0f )
		, ControlPointTwoLenght		( 0.0f )
		, ControlPointTwoLenghtY	( 0.0f )

		, SphereAdjPos				( 0.0f, 0.0f, 0.0f )
		, SphereAdjRadius			( 0.0f )
	{}
} typedef SAlienParam;

namespace alien
{
	// �F���l���.
	enum class enAlienState : unsigned char
	{
		None,

		Spawn,				// �X�|�[��.
		Move,				// �ړ�.
		Abduct,				// ����.
		KnockBack,			// �m�b�N�o�b�N.
		Fright,				// ����.
		Death,				// ���S.
		Escape,				// ������.
		RisingMotherShip,	// ��ɏオ��(�}�U�[�V�b�v��).

		Max,

	} typedef EAlienState;

	// �ړ����.
	enum class enMoveState : unsigned char
	{
		None,

		Rotation,
		Move,
		Attack,
		Wait,

		Max,
	} typedef EMoveState;

	// �A�j���[�V�����ԍ�.
	enum enAnimNo : char
	{
		EAnimNo_None = -1,

		EAnimNo_Move,		// �ړ�.
		EAnimNo_Arm,		// �A�[��.
		EAnimNo_Damage,		// �_���[�W.
		EAnimNo_Dead,		// ���S.

		EAnimNo_Max,

		EAnimNo_Attack	= EAnimNo_Arm,	// �U��.

		EAnimNo_Begin	= EAnimNo_Move,
		EAnimNo_End		= EAnimNo_Max,
	} typedef EAnimNo;

	// �G�t�F�N�g�ԍ�.
	enum enEffectNo : char
	{
		EEffectNo_None = -1,

		EEffectNo_Hit,			// �q�b�g.
		EEffectNo_Spawn,		// �X�|�[��.
		EEffectNo_Dead,			// ���S.
		EEffectNo_Attack,		// �U��.

		EEffectNo_Max,
	} typedef EEffectNo;
};

#endif	// #ifndef ALIEN_PARAM_H.