#ifndef EDIT_ALIEN_C_H
#define EDIT_ALIEN_C_H

#include "..\EditAlien.h"

class CEditAlienC : public CEditAlien
{
	const char* MODEL_NAME = "c_s";
public:
	CEditAlienC();
	virtual	~CEditAlienC();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

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

#endif	// #ifndef EDIT_ALIEN_C_H.
