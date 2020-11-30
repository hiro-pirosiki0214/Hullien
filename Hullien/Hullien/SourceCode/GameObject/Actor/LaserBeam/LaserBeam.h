#ifndef LASER_BEAM_H
#define LASER_BEAM_H

#include "..\Actor.h"
#include <list>

class CTrajectory;	// �O��.
class CEffectManager;

class CLaserBeam : public CActor
{
	const float DEFAULT_MOVE_SPEED			= 0.1f;	// �ړ����x.
	const float DEFAULT_PARALYSIS_TIME		= 3.0f;	// ��Ⴢ̎���.
	const int	MAX_TRAJECTORY_COUNT		= 50;	// �O�Ղ̍ő咸�_��.
	const int	TRAJECTORY_TIME				= 1;	// �O�Ղ̒��_���Z����.
	const int	TRAJECTORY_END_ADD_VALUE	= 8;	// �O�Ղ��I������ۂ̉��Z�l.
	const char*	EFFECT_NAME					= "b_attackeffkt";
public:
	CLaserBeam();
	virtual ~CLaserBeam();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& actor ) override;
	void SetTargetPos( const D3DXVECTOR3& pos );

	// �r�[����ł�.
	void Shot( const D3DXVECTOR3& pos );
	// �p�����[�^�������ɖ߂�.
	void ResetParam();
	// �x�W�F�Ȑ��̐���_�̐ݒ�.
	void SetControlPointList( std::vector<D3DXVECTOR3> pointList );

	// �ړ����x�̐ݒ�.
	inline void SetMoveSpped( const float& moveSpeed ){ m_MoveSpeed = moveSpeed; }
	// ��჎��Ԃ̐ݒ�.
	inline void SetParalysisTime( const float& time ){ m_ParalysisTime = time; }

	// �U������.
	inline bool IsInAttack() const { return m_IsInAttack; }
	// �U�����I���������ǂ���.
	inline bool IsEndAttack() const { return m_IsEndAttack; }

private:
	// �񎟃x�W�F�Ȑ�.
	void SecondaryBeziercurve();
	// �O���x�W�F�Ȑ�.
	void ThirdBezierCurve();

	// �����蔻��̐ݒ�.
	bool CollisionSetting();
	// �g�p���_�̐ݒ�.
	void SetVertexPoint();
	// ���_�̍쐬.
	void CreateVertex();

	// �G�t�F�N�g�̎擾.
	bool GetEffect();

private:
	std::unique_ptr<CTrajectory>	m_pTrajectory;	// �O�Օ`��N���X.
	std::shared_ptr<CEffectManager>	m_pEffect;		// �G�t�F�N�g.
	float		m_MoveSpeed;		// �ړ����x.
	float		m_ParalysisTime;	// ��჎���.
	D3DXVECTOR3	m_TargetPosition;	// �U���Ώۂ̍��W.
	bool		m_IsInAttack;		// �U����.
	bool		m_IsEndAttack;		// �U���I��.
	float		m_FrameCount;		// �t���[���J�E���g.
	float		m_FrameTime;		// �t���[������.
	D3DXVECTOR3 m_InitPosition;		// �������W.
	std::vector<D3DXVECTOR3>	m_ControlPointList;		// ������W.
	std::vector<D3DXVECTOR3>	m_VertexPointList;		// ���_�̍��W�̃��X�g.
	int							m_VertexAddTimeCount;	// ���_�����Z���鎞�Ԃ̃J�E���g.
};

#endif	// #ifndef LASER_BEAM_H.