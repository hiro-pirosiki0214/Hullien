#ifndef ALIEN_C_H
#define ALIEN_C_H

#include "..\Alien.h"

class CAlienC : public CAlien
{
	const char* MODEL_NAME = "c_s";	// ���f����.

public:
	CAlienC();
	CAlienC( const SAlienParam* pParam );
	virtual ~CAlienC();

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

#endif	// #ifndef ALIEN_C_H.