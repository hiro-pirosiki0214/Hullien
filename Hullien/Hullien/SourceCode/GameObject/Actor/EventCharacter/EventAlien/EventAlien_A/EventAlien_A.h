#ifndef EVENTALIEN_A_H
#define EVENTALIEN_A_H

#include "..\EventAlien.h"

/*************************************
*	�C�x���g�p�F���lA.
**/
class CEventAlienA : public CEventAlien
{
	const char* MODEL_NAME = "a_s";

public:
	CEventAlienA();
	virtual ~CEventAlienA();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision(CActor* pActor) override;
	// �X�|�[��.
	virtual bool Spawn(const D3DXVECTOR3& spawnPos) override;
	// �F���l�̏�Ԏ擾�֐�.
	inline CEventCharacter::SCharacterParam GetParameter() const { return m_Parameter; }

	// �A�[�������S�ɂƂ肾��.
	void SetArmAppearance();

private:
	// �X�|�[��.
	virtual void Spawning() override;
	// �ړ�.
	virtual void Move() override;

	// �����蔻��̐ݒ�.
	bool ColliderSetting();

	// �A�j���[�V�����t���[���̐ݒ�.
	bool SetAnimFrameList();

};

#endif	//#ifndef EVENTALIEN_A_H.