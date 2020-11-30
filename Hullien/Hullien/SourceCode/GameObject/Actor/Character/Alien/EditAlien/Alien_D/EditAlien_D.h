#ifndef EDIT_ALIEN_D_H
#define EDIT_ALIEN_D_H

#include "..\EditAlien.h"

class CLaserBeam;	// ���[�U�[�r�[���N���X.

class CEditAlienD : public CEditAlien
{
	const char* MODEL_NAME = "ultemate_s";
	const char* SPRITE_NAME = "AttackRange";

	const D3DXVECTOR4 ATTACK_RANGE_COLOR		= { 0.8f, 0.0f, 0.0f, 0.0f };	// �ԐF.
	const D3DXVECTOR4 ATTACK_RANGE_DANGER_COLOR	= { 0.8f, 0.8f, 0.0f, 1.0f };	// ���F.
	const float ATTACK_TIME = 1.0f;
public:
	CEditAlienD();
	virtual	~CEditAlienD();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

	// �U���̍Đ�.
	virtual void PlayAttack() override;
	// �p�����[�^�[�̐ݒ�.
	virtual void SetParamter( const SAlienParam& param ) override;

private:
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

	// �����蔻��̐ݒ�.
	bool ColliderSetting();

private:
	std::shared_ptr<CSprite>	m_pAttackRangeSprite;	// �U���͈͂̃X�v���C�g.
	std::unique_ptr<CLaserBeam>	m_pLaserBeam;			// ���[�U�[�r�[���N���X.
	std::vector<D3DXVECTOR3>	m_ControlPositions;		// �x�W�F�Ȑ��Ŏg�p���鑀����W.
	float m_AttackCount;	// �U���J�E���g.
	bool m_IsAttackStart;	// �U���J�n������.
};

#endif	// #ifndef EDIT_ALIEN_D_H.
