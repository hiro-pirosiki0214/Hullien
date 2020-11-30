#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"
#include "PlayerParam.h"
#include <queue>

class CRotLookAtCenter;
class CCharacterWidget;
class CEffectManager;
class CEffectTimer;

class CPlayer : public CCharacter
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME			= "kaito_s";	// ���f����.
	const char* MODEL_TEMP_NAME		= "kaito";		// �����f����.

	// �g�p�U���G�t�F�N�g��.
	const char* ATTACK_ONE_EFFECT_NAME		= "slash_effect_1";
	const char* ATTACK_TWO_EFFECT_NAME		= "slash_effect_2";
	const char* ATTACK_THREE_EFFECT_NAME	= "slash_effect_3";
	// ����\�̓G�t�F�N�g��.
	const char* SPECIAL_ABILITY_EFFECT_NAME	= "uvtest";
	// ����G�t�F�N�g��.
	const char* AVOIDANCE_EFFECT_NAME		= "sandSmoke";
	// ��ჃG�t�F�N�g��.
	const char* PARALYSIS_EFFECT_NAME		= "uvtest";

	// ���̃{�[����.
	const char* RIGHT_FOOT	= "kaito_rifa_2_R_asi_4";
	const char* LEFT_FOOT	= "kaito_rifa_2_L_asi_4";

	// ���̉���.
	const float VOICE_VOLUME = 1.5f;

	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(7.0));
	const float ROTATIONAL_SPEED = 0.2f;	// ��]���x.

	const float MOVE_SPEED_MUL_VALUE_ADD	= 0.035f;	// �|�����킹��ړ��ʂ̉��Z�l.
	const float MOVE_SPEED_MUL_VALUE_MAX	= 1.0f;		// �|�����킹��ړ��ʂ̍ő�l.

	const float TWO = 2.0f;
	const float CAMERA_COUNT_MAX			= 100.0f;
	const float CAMERA_BACK_DIRECTION_X		= 20.0f;
	const float CAMERA_BACK_DIRECTION_Z		= 10.0f;
	const float CAMERA_BACK_HEIGHT			= 8.0f;
	const float CAMERA_BACK_LERP_VALUE		= 0.1f;
	const float CAMERA_FREQUENCY_LOOKPOS	= 15.0f;	// �J���������ʒu�̎��g��.
	const float CAMERA_AMPLITUDE_LOOKPOS	= 0.01f;	// �J���������ʒu�̐U��.
	const float CAMERA_RETURN_COUNT_ADD		= 0.001f;
	const float CAMERA_RETURN_COUNT_MAX		= 0.1f;

	// �A�j���[�V�����̒����t���[��.
	const double ANIM_ADJ_FRAME_Wait	= 0.0;	// �ҋ@.
	const double ANIM_ADJ_FRAME_Walk	= 0.0;	// ����.
	const double ANIM_ADJ_FRAME_Attack1	= 0.5;	// �U��1.
	const double ANIM_ADJ_FRAME_Attack2	= 0.5;	// �U��2.
	const double ANIM_ADJ_FRAME_Attack3	= 0.5;	// �U��3.
	const double ANIM_ADJ_FRAME_Avoid	= 0.4;	// ���.
	const double ANIM_ADJ_FRAME_SP		= 0.02;	// ����\��.
	const double ANIM_ADJ_FRAME_Damage	= 0.4;	// �q�b�g��.
	const double ANIM_ADJ_FRAME_Dead	= 0.01;	// ���S.

	const double ATTACK1_ADJ_ENABLED_END_FRAME	= 0.0;	// �U��1�̒����p�L���t���[��.
	const double ATTACK2_ADJ_ENABLED_END_FRAME	= 0.0;	// �U��2�̒����p�L���t���[��.
	const double ATTACK3_ADJ_ENABLED_END_FRAME	= 0.0;	// �U��3�̒����p�L���t���[��.

	const double ATTACK1_ADJ_DRAGING_FRAME_START = 0.12;
	const double ATTACK1_ADJ_DRAGING_FRAME_END = 0.24;
	const float ATTACK1_ADJ_DRAGING_SPEED = 0.35f;

	const double ATTACK2_ADJ_DRAGING_FRAME_START = 0.01;
	const double ATTACK2_ADJ_DRAGING_FRAME_END = 0.2;
	const float ATTACK2_ADJ_DRAGING_SPEED = 0.3f;

	const double ATTACK3_ADJ_DRAGING_FRAME_START = 0.01;
	const double ATTACK3_ADJ_DRAGING_FRAME_END = 0.2;
	const float ATTACK3_ADJ_DRAGING_SPEED = 0.3f;

	const D3DXVECTOR3 ATTACK_COLLISION_INVALID_POS = { 0.0f, -10.0f, 0.0f };
	const float ATTACK1_COLLISION_RADIUS	= 6.0f;
	const float ATTACK2_COLLISION_RADIUS	= 7.0f;
	const float ATTACK3_COLLISION_RADIUS	= 8.0f;
	const float ATTACK_COLLISION_DISTANCE	= 4.0f;
	const float ATTACK_COLLISION_HEIGHT		= 4.0f;


	const double DEAD_CERTAIN_RANGE_ANIM_FRAME_MIN	= 0.18;		// ���S�A�j���[�V�����̈��͈͒l�̍ŏ�.
	const double DEAD_CERTAIN_RANGE_ANIM_FRAME_MAX	= 0.5;		// ���S�A�j���[�V�����̈��͈͒l�̍ő�.
	const float	DEAD_ANIM_DRAGING_ADJ_SPEED			= 0.05f;	// ���S�A�j���[�V�����̈�������̒������x.
	const float DAMAGE_HIT_KNOC_BACK_SPEED			= 0.3f;

public:
	CPlayer();
	virtual ~CPlayer();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override; 
	// �G�t�F�N�g�`��֐�.
	virtual void EffectRender() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& actor ) override;
	// �X�v���C�g�̕`��.
	virtual void SpriteRender() override;

	// ����\�͂��g���Ă��邩.
	bool IsSpecialAbility();
	// ���S������.
	inline bool IsDead(){ return m_AnimFrameList[player::EAnimNo_Dead].IsNowFrameOver(); }
	// �J�����̕���.
	float GetCameraRadianX();

	// �x�N�g���̎擾.
	virtual void SetVector( const D3DXVECTOR3& vec ) override { m_HitVector = vec; }

private:
	// ����֐�.
	void Controller();
	// �J��������.
	void CameraController();
	// �U������֐�.
	void AttackController();
	// ����\�͑���֐�.
	void SPController();
	// ��𑀍�֐�.
	void AvoidController();

	// �ړ��֐�(�A�j���[�V�����Ƃ̂���̒���).
	virtual void Move() override;
	// �U�����̈ړ�.
	void AttackMove();
	// ��𓮍�֐�.
	void AvoidMove();
	// �m�b�N�o�b�N����֐�.
	void KnockBack();
	// ���S����֐�.
	void Dead();
	// �J�����̍X�V.
	void CameraUpdate();

	// �U���̓����蔻��.
	void AttackCollision( CActor* pActor );
	// �U���͈͂Ƃ̔���.
	void AttackRangeDecision( CActor* pActor );
	// ����\�͎��̃J��������.
	void SPCameraUpdate();

	// ����\�͉񕜍X�V�֐�.
	void SpecialAbilityUpdate();
	// �U����UP�X�V�֐�.
	void AttackUpUpdate();
	// �ړ����xUP�X�V�֐�.
	void MoveSpeedUpUpdate();
	// ��გ��̍X�V�֐�.
	void ParalysisUpdate();

	// �U���A�j���[�V����.
	void AttackAnimation();
	// �U���̒ǉ����ł�����.
	bool IsPushAttack();

	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ) override;
	// ����\�͉񕜎��ԁA���͎��Ԑݒ�֐�.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ) override;
	// �U���́A���͎��Ԑݒ�֐�.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ) override;
	// �ړ����x�A���͎��Ԑݒ�֐�.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ) override;
	// ��Ⴢ̐ݒ�.
	virtual void SetParalysisTime( const std::function<void(float&)>& ) override;

	// �����蔻��̐ݒ�.
	bool ColliderSetting();
	// �G�t�F�N�g�̐ݒ�.
	bool EffectSetting();
	// �A�j���[�V�����t���[���̐ݒ�.
	virtual bool SetAnimFrameList() override;

	// �G�f�B�b�g�p�̕`��֐�.
	void EditRender();

	// �T�E���h�̐ݒ�.
	bool SoundSetting();
	// ���ʂ̐ݒ�.
	void VolumeSetting(const char* soung, float volume);
	// �E�B�W�F�b�g�ݒ�.
	bool WidgetSetting();

private:
	std::shared_ptr<CRotLookAtCenter>				m_pCamera;				// �J�����N���X.
	std::shared_ptr<CCamera>						m_pSPCamera;			// ����\�̓J�����N���X.
	std::shared_ptr<CCollisionManager>				m_pAttackCollManager;	// �U���p�̓����蔻��.
	std::vector<std::shared_ptr<CEffectManager>>	m_pEffects;				// �G�t�F�N�g.
	std::vector<std::shared_ptr<CEffectTimer>>		m_pEffectTimers;		// ���ʎ��Ԍv���N���X.
	std::vector<std::shared_ptr<CCharacterWidget>>	m_pWidget;				// Widget�N���X.

	int								m_AttackComboCount;			// �U���R���{�J�E���g.
	std::queue<player::SAttackData>	m_AttackDataQueue;			// �U���f�[�^�̃L���[.
	D3DXVECTOR3						m_AttackPosition;			// �U���p�����蔻����W.

	D3DXVECTOR3		m_GirlPosition;		// ���̎q�̍��W.
	D3DXVECTOR3		m_AttackVector;		// �U�����̃x�N�g��.
	D3DXVECTOR3		m_AvoidVector;		// ����x�N�g��.
	D3DXVECTOR3		m_HitVector;		// �Փˎ��̃x�N�g��.
	D3DXVECTOR3		m_TargetVector;		// �ړI�̃x�N�g��.

	SPlayerParam	m_Parameter;			// �p�����[�^�[.
	float			m_LifePoint;			// �̗�.
	float			m_SpecialAbility;		// ����\��.
	unsigned char	m_StatusFlag;			// ���݂̏�Ԃ̃t���O.

	float			m_AttackRangeLenght;		// �U���͈͂̃^�[�Q�b�g�Ƃ̋���(��r�p).
	float			m_SpecialAbilityValue;		// ����\�͉񕜗�.
	float			m_ItemSpecialAbilityValue;	// �A�C�e������\�͉񕜒l.
	float			m_AttackPower;				// �U����.
	float			m_MoveSpeed;				// �ړ����x.
	float			m_MoveSpeedMulValue;		// �ړ����x�Ɋ|�����킹��l.

	D3DXVECTOR3		m_CameraNextPosition;		// �J�����̍��W.
	D3DXVECTOR3		m_CameraPosition;			// �J�����̍��W.
	D3DXVECTOR3		m_CameraLookPosition;		// �J�����̎��_���W.
	float			m_CameraCount;				// �J�����̃J�E���g.
	float			m_CameraReturnCount;		// �J�����̖߂�J�E���g.
	float			m_CameraLerp;				// �J�����ړ��̕�Ԓl.
	int				m_NowSPCameraStete;			// ����\�͂̃J�������.
	
	bool			m_OneStepCmaeraSeting;		// �J�����̐ݒ�(��񂾂��g�p).
};

#endif	// #ifndef PLAYER_H.