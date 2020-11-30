#ifndef EDIT_ALIEN_H
#define EDIT_ALIEN_H

#include "..\Alien.h"

class CEditAlien : public CAlien
{
	inline static const float MOVE_DISTANCE = 50.0f;	// �ړ�����.
public:
	CEditAlien();
	virtual ~CEditAlien();

	// �X�|�[��.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) override;

	// �U���̍Đ�.
	virtual void PlayAttack();
	// ���݂̍Đ�.
	void PlayFright();
	// ���S�̍Đ�.
	void PlayDeath();
	// �}�U�[�V�b�v�ɏ�铮��̍Đ�.
	void PlayRisingMotherShip( const D3DXVECTOR3& vPos );
	// �p�����[�^�[�̐ݒ�.
	virtual void SetParamter( const SAlienParam& param ){ m_Paramter = param; }
	// �v���C����.
	inline bool IsPlaying(){ return m_IsPlaying; }

protected:
	// �ҋ@�֐�.
	virtual void WaitMove();

	// �X�|�[����.
	virtual void Spawning() override;
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Abduct() override;
	// �m�b�N�o�b�N.
	virtual void KnockBack() override;
	// ����.
	virtual void Fright() override;
	// ���S.
	virtual void Death() override;
	// ������.
	virtual void Escape() override;
	// �}�U�[�V�b�v�ɏ����Ă���.
	virtual void RisingMotherShip() override;

protected:
	SAlienParam m_Paramter;
	bool		m_IsPlaying;
};

#endif	// #ifndef EDIT_ALIEN_H.