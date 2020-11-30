#ifndef PLAYER_PARAM_H
#define PLAYER_PARAM_H

#include "..\Character.h"

// �v���C���[�p�����[�^.
struct stPlayerParam : public CCharacter::SParameter
{
	float		SpecialAbilityMax;	// ����\�͍ő�l.
	float		SpecialAbilityValue;// ����\�͉񕜒l.
	int			AttackComboMax;		// �U���̍ő吔.
	int			AttackQueueMax;		// �U���L���[�̍ő吔.
	float		AvoidMoveDistance;	// ����̈ړ�����.
	float		AvoidMoveSpeed;		// ���p�̈ړ����x.
	float		CameraMoveSpeed;	// �J�����̈ړ����x.
	float		CameraDistance;		// �J�����̋���.
	float		CameraHeight;		// �J�����̍���.
	float		CameraLookHeight;	// �J�����̒����_�̍���.
	float		CameraLerpValue;	// �J�����ړ��̕⊮�l.
	D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
	float		SphereAdjRadius;	// �X�t�B�A�̒������a.

	stPlayerParam()
		: SpecialAbilityMax	( 0.0f )
		, AttackComboMax	( 0 )
		, AttackQueueMax	( 0 )
		, AvoidMoveDistance	( 0.0f )
		, AvoidMoveSpeed	( 0.0f )
		, CameraMoveSpeed	( 0.01f )
		, CameraDistance	( 7.0f )
		, CameraHeight		( 4.0f )
		, CameraLookHeight	( 0.0 )
		, CameraLerpValue	( 1.0f )
		, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		, SphereAdjRadius	( 0.0f )
	{}
} typedef SPlayerParam;

namespace player
{
// �A�j���[�V�����ԍ�.
enum enAnimNo : char
{
	EAnimNo_None = -1,

	EAnimNo_Wait,		// �ҋ@.
	EAnimNo_Walk,		// ����.
	EAnimNo_Attack1,	// �U��1.
	EAnimNo_Attack2,	// �U��2.
	EAnimNo_Attack3,	// �U��3.
	EAnimNo_Avoid,		// ���.
	EAnimNo_SP,			// ����\��.
	EAnimNo_Damage,		// �q�b�g��.
	EAnimNo_Dead,		// ���S.

	EAnimNo_Max,		// �ő�.

	EAnimNo_Begin	= EAnimNo_Wait,	// ����.
	EAnimNo_End		= EAnimNo_Max,	// �I��.

} typedef EAnimNo;

// �U���ԍ�,
enum enAttackNo : unsigned char
{
	EAttackNo_None,

	EAttackNo_One,
	EAttackNo_Two, 
	EAttackNo_Three,

	EAttackNo_Max = EAttackNo_Three,
} typedef EAttackNo;

// ���ʎ��Ԍv���ԍ�.
enum enEffectTimerNo : unsigned char
{
	EEffectTimerNo_None,

	EEffectTimerNo_SPRecovery = 0,	// ����\�͉�.
	EEffectTimerNo_Attack,			// �U����.
	EEffectTimerNo_MoveSpeedUp,		// �ړ����x.
	EEffectTimerNo_Paralysis,		// ���.

	EEffectTimerNo_Max,
} typedef EEffectTimerNo;

// �g�p�G�t�F�N�g�ԍ�.
enum enEffectNo : unsigned char
{
	EEffectNo_AttackOne,		// �U��1.
	EEffectNo_AttackTwo,		// �U��2.
	EEffectNo_AttackThree,		// �U��3.
	EEffectNo_SP,				// ����\��.
	EEffectNo_Avoidance,		// ���.
	EEffectNo_Paralysis,		// ���.
	
	EEffectNo_Max,
} typedef EEffectNo;

// ����\�͂̃J�������.
enum enSPCameraState : unsigned char
{
	ESPCameraState_None,

	ESPCameraState_TargetRotation,	// �ړI�̍��W�։�].
	ESPCameraState_PlayerBack,		// �J�������v���C���[�̌��Ɉړ�.
	ESPCameraState_CameraShake,		// �J������h�炷.
	ESPCameraState_CameraReturn,	// �J���������̈ʒu�ɖ߂�.
	
	ESPCameraState_Max,

	ESPCameraState_Start	= ESPCameraState_TargetRotation,	// �n�܂�.
	ESPCameraState_End		= ESPCameraState_CameraReturn,		// �I��.

} typedef ESPCameraState;

// ���݂̏�Ԃ̃t���O(�r�b�g�t���O�p).
enum enStatusFlag : unsigned char
{
	EStatusFlag_None,

	EStatusFlag_DuringAvoid		= 1 << 0,	// ��𒆂�.
	EStatusFlag_EndSPCameraMove	= 1 << 1,	// ����\�͂̃J�����̈ړ����I��������.
	EStatusFlag_AttackRange		= 1 << 2,	// �U���͈͂ɓG�����邩.
	EStatusFlag_UsableSP		= 1 << 3,	// ����\�͂��g���邩.
	EStatusFlag_KnockBack		= 1 << 4,	// �m�b�N�o�b�N����.
	EStatusFlag_Dead			= 1 << 5,	// ���S����.
	EStatusFlag_AttackSE		= 1 << 6,	// �U��SE��炷��.

	EStatusFlag_Max,

} typedef EStatusFlag;

// �U���p�f�[�^.
struct stAttackData
{
	EAnimNo		AnimNo;				// �A�j���[�V�����ԍ�.
	double		Frame;				// �o�߃t���[��.
	double		EnabledEndFrame;	// �L���I���t���[��.
	double		EndFrame;			// �I���t���[��.

	stAttackData()
		: AnimNo			( EAnimNo_None )
		, Frame				( 0.0 )
		, EnabledEndFrame	( 1.0 )
		, EndFrame			( 2.0 )
	{}
} typedef SAttackData;

};	// namespace PLAYER.

#endif	// #ifndef PLAYER_PARAM_H.