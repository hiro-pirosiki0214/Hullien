#ifndef ALIEN_B_H
#define ALIEN_B_H

#include "..\Alien.h"

class CAlienB : public CAlien
{
	const char* MODEL_NAME = "b_s";
	const char* ATTACK_MODEL_NAME	= "b_attack";

public:
	CAlienB();
	CAlienB( const SAlienParam* pParam );
	virtual ~CAlienB();

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
	// �X�|�[��.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) override;
	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor ) override;
	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ) override;
private:
	// �v���C���[���W�̐ݒ�.
	void SetPlayerPos( CActor& actor );
	// �X�|�[��.
	virtual void Spawning() override;
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Abduct() override;
	// ����.
	virtual void Fright() override;
	// ���S.
	virtual void Death() override;
	// ������.
	virtual void Escape() override;

	// �ړ��֐�.
	virtual void VectorMove( const float& moveSpeed ) override;
	// �U���֐�.
	void Attack();
	// �v���C���[�Ƃ̓����蔻��.
	void PlayerCollison( CActor* pActor );

	// �v���C���[��_��������.
	void AimPlayerDecision();

	// �����蔻��̐ݒ�.
	bool ColliderSetting();
	// �U�����f���̎擾.
	bool GetAttackModel();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pAttackMesh;
	D3DXVECTOR3 m_vPlayerPos;	// �v���C���[�̍��W.
	bool	m_HasAimPlayer;		// �v���C���[��_�����ǂ���.
	bool	m_OldHasAimPlayer;	// �O��v���C���[��_���Ă�����.
	float	m_RotAccValue;		// ��]�����l.
	bool	m_IsAttackSE;		// �U��SE��炷��.
};

#endif	// #ifndef ALIEN_B_H.