#ifndef ALIEN_A_H
#define ALIEN_A_H

#include "..\Alien.h"

class CAlienA : public CAlien
{
	const char* MODEL_NAME = "a_s";

public:
	CAlienA();
	CAlienA( const SAlienParam* pParam );
	virtual ~CAlienA();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// �X�|�[��.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) override;

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

	// �����蔻��̐ݒ�.
	bool ColliderSetting();

};

#endif	// #ifndef ALIEN_A_H.