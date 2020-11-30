#ifndef ALIEN_D_H
#define ALIEN_D_H

#include "..\Alien.h"

class CLaserBeam;	// ���[�U�[�r�[���N���X.

class CAlienD : public CAlien
{
	const char* MODEL_NAME = "ultemate_s";
	const char* SPRITE_NAME = "AttackRange";

	const D3DXVECTOR4 ATTACK_RANGE_COLOR		= { 0.8f, 0.0f, 0.0f, 0.0f };	// �ԐF.
	const D3DXVECTOR4 ATTACK_RANGE_DANGER_COLOR	= { 0.8f, 0.8f, 0.0f, 1.0f };	// ���F.
	const float ATTACK_TIME = 1.0f;

public:
	CAlienD();
	CAlienD( const SAlienParam* pParam );
	virtual ~CAlienD();

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
	// �X�v���C�g�̕`��.
	virtual void SpriteRender() override;

private:
	// ���f���̕`��.
	void ModelRender();
	// �U���͈͂̃X�v���C�g�`��.
	void AttackRangeSpriteRender();
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
	// �U���֐�.
	void Attack();
	// �ړ��֐�.
	virtual void VectorMove( const float& moveSpeed ) override;
	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor ) override;
	// �v���C���[���W�̐ݒ�.
	void SetPlayerPos( CActor& actor );

	// �U���̓����蔻��.
	void AttackCollision( CActor* pActor  );

	// �X�v���C�g�̎擾.
	bool GetSprite( const char* spriteName );

	// �����蔻��̐ݒ�.
	bool ColliderSetting();

private:
	std::shared_ptr<CSprite>	m_pAttackRangeSprite;	// �U���͈͂̃X�v���C�g.
	std::unique_ptr<CLaserBeam>	m_pLaserBeam;			// ���[�U�[�r�[���N���X.
	std::vector<D3DXVECTOR3>	m_ControlPositions;		// �x�W�F�Ȑ��Ŏg�p���鑀����W.
	float m_AttackCount;	// �U���J�E���g.
	bool m_IsAttackStart;	// �U���J�n������.
};

#endif	// #ifndef ALIEN_D_H.