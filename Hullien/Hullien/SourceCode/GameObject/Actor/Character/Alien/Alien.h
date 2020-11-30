#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"
#include "AlienParam.h"
#include "..\..\Item\ItemList.h"

#include <random>

class CArm;
class CEffectManager;

class CAlien : public CCharacter
{
protected:
	inline static const float TOLERANCE_RADIAN			= static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.
	inline static const float BARRIER_HIT_MOVE_SPEED	= -5.0f;									// �o���A�ƏՓˎ��̈ړ����x.
	inline static const float SCALE_MAX					= 1.0f;										// ���f���̍ő�l.
	inline static const float DEATH_SCALE_PI			= 6.0f*static_cast<float>(D3DX_PI);			// ���S���̃X�P�[���͈̔�.

protected:
	inline static const char* HIT_EEFECT_NAME		= "hiteffekt";		// �q�b�g�G�t�F�N�g.
	inline static const char* SPAWN_EFFECT_NAME		= "uvtest";			// �X�|�[���G�t�F�N�g.
	inline static const char* DEAD_EFFECT_NAME		= "uvtest";			// ���S�G�t�F�N�g.
	inline static const char* ATTACK_EFFECT_NAME	= "uvtest";			// �U���G�t�F�N�g.

	const SAlienParam*	pPARAMETER;	// �p�����[�^�̃|�C���^ : �O������擾.

public:
	CAlien();
	CAlien( const SAlienParam* pParam );
	virtual ~CAlien();

	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() override;

	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor ) override;
	// �x�N�g���̎擾.
	virtual void SetVector( const D3DXVECTOR3& vec ) override;
	// �X�|�[��.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) = 0;

	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ) override;
	// �A�ꋎ���Ă��邩�ǂ���.
	inline bool IsAbduct() const { return m_NowState == alien::EAlienState::Abduct; }
	// �ǂ̃A�C�e���������Ă��邩�擾.
	inline EItemList GetAnyItem() const { return m_HasAnyItem; }
	// �ق��̉F���l���A�ꋎ���Ă��邩�ݒ�.
	inline void SetOtherAbduct( bool* pisAbduct ){ m_pIsAlienOtherAbduct = pisAbduct; }
	// �A�ꋎ��UFO�̍��W�̎擾.
	inline void SetAbductUFOPosition( D3DXVECTOR3* pos ){ m_pAbductUFOPosition = pos; }
	// �A�C�e���̐ݒ�.
	inline void SetItem( const EItemList& item ){ m_HasAnyItem = item; }
	// �������邩.
	inline bool IsExplosion() const { return m_IsExplosion; }
	// ���S���ǂ���.
	inline bool IsDead() const { return m_NowState == alien::EAlienState::Death; }
	// �������邩�ǂ���.
	inline bool IsDelete() const { return m_IsDelete; }
	// �}�U�[�V�b�v�ɏ����Ă��邩.
	inline bool IsRisingMotherShip() const { return m_NowState == alien::EAlienState::RisingMotherShip; }

protected:
	// ���݂̏�Ԃ̍X�V�֐�.
	void CurrentStateUpdate();
	// ���̎q�̍��W��ݒ�.
	void SetGirlPos( CActor& actor );
	// ���W�ݒ�֐�.
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;

	// �ړ��x�N�g���ݒ�֐�.
	virtual void SetMoveVector( const D3DXVECTOR3& targetPos );
	// �ړI�̍��W�։�].
	void TargetRotation();
	// �ړ��֐�.
	virtual void VectorMove( const float& moveSpeed );
	// �ҋ@�֐�.
	virtual void WaitMove();

	// �X�|�[����.
	virtual void Spawning();
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Abduct();
	// �m�b�N�o�b�N.
	virtual void KnockBack();
	// ����.
	virtual void Fright();
	// ���S.
	virtual void Death();
	// ������.
	virtual void Escape();
	// �}�U�[�V�b�v�ɏ����Ă���.
	virtual void RisingMotherShip();

	// ���̎q�Ƃ̓����蔻��.
	void GirlCollision( CActor* pActor );
	// �o���A�Ƃ̓����蔻��.
	void BarrierCollision( CActor* pActor );

	// �A�j���[�V�����R���g���[���[�̎擾.
	bool GetAnimationController();

	// �A�j���[�V�����t���[���̐ݒ�.
	virtual bool SetAnimFrameList() override;
	// �G�t�F�N�g�̐ݒ�.
	virtual bool EffectSetting();

protected:
	std::unique_ptr<CArm>		m_pArm;			// �A�[���N���X.
	LPD3DXANIMATIONCONTROLLER	m_pAC;			// �A�j���[�V�����R���g���[���[.
	std::vector<std::shared_ptr<CEffectManager>>	m_pEffects;
	D3DXVECTOR3				m_TargetPosition;			// ���̎q�̍��W.
	D3DXVECTOR3				m_TargetRotation;			// �ڕW�̉�]���.
	D3DXVECTOR3				m_KnockBackVector;			// �m�b�N�o�b�N�̈ړ��x�N�g��.
	D3DXVECTOR3				m_BeforeMoveingPosition;	// �ړ��O�̍��W.
	D3DXVECTOR3*			m_pAbductUFOPosition;		// UFO�̍��W.
	alien::EAlienState		m_NowState;					// ���݂̏��.
	alien::EMoveState		m_NowMoveState;				// ���݂̈ړ����.
	EItemList				m_HasAnyItem;				// �ǂ̃A�C�e���������Ă��邩.
	float					m_LifePoint;				// �̗�.
	float					m_MoveSpeed;				// �ړ����x.
	float					m_DeathScale;				// ���S���̑傫��.
	float					m_DeathCount;				// ���S�J�E���g.
	int						m_KnockBackCount;			// �m�b�N�o�b�N�J�E���g.
	int						m_WaitCount;				// �ҋ@�J�E���g.
	bool*					m_pIsAlienOtherAbduct;		// ���̉F���l���A�ꋎ���Ă��邩�ǂ���.
	bool					m_IsBarrierHit;				// �o���A�ɓ������Ă��邩.
	bool					m_IsRisingMotherShip;		// �}�U�[�V�b�v�ɏ���Ă��邩.
	bool					m_IsExplosion;				// �������邩.
	bool					m_IsDelete;					// �������邩�ǂ���.

	std::shared_ptr<CDX9StaticMesh>	m_pTempStaticMesh;
};

#endif	// #ifndef ALIEN_H.