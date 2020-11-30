#ifndef EDIT_ALIEN_B_H
#define EDIT_ALIEN_B_H

#include "..\EditAlien.h"

class CEditAlienB : public CEditAlien
{
	const char* MODEL_NAME = "b_s";
public:
	CEditAlienB();
	virtual	~CEditAlienB();

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
	float	m_RotAccValue;		// ��]�����l.
	bool	m_IsAttackSE;		// �U��SE��炷��.
};

#endif	// #ifndef EDIT_ALIEN_B_H.
