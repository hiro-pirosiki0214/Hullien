#ifndef BARRIER_H
#define BARRIER_H

#include "..\Actor.h"
class CEffectManager;	// �G�t�F�N�g�N���X.
class CEventActor;

class CBarrier : public CActor
{
	const char* MODEL_NAME = "Barrier";
	const char* EFFECT_NAME = "barrier_effect";	// �G�t�F�N�g�̖��O.

	const float COLLISION_SPHERE_RDIUS_MAX = 20.0f;
	const float COLLISION_SPHERE_RDIUS_ADD_VALUE = 1.0f;
	const float ACTIVE_TIME = 5.0f;

public:
	CBarrier();
	virtual ~CBarrier();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �G�t�F�N�g�`��֐�.
	virtual void EffectRender() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& pActor ) override;
	// �����Ԃ�.
	inline bool IsActive() const { return m_IsActive; }

private:
	// ���f���̎擾.
	bool GetModel();
	// �����蔻��̐ݒ�.
	bool ColliderSetting();

private:
	std::shared_ptr<CDX9StaticMesh>	m_StaticMesh;	// �X�^�e�B�b�N���b�V��.
	std::shared_ptr<CEffectManager>	m_pEffect;	// �G�t�F�N�g.
	bool m_IsEffectPlay;		// �G�t�F�N�g���Đ����ꂽ��.
	bool m_IsActive;			// �����Ԃ�.
	float m_ActiveCount;		// ����J�E���g.
	float m_CollSphereRadius;	// �����蔻��̔��a.
#if _DEBUG
	float m_ResizeCollTime;		// �����蔻��̔��a�̃��T�C�Y���鎞��(�f�o�b�O�p).
#endif	// #if _DEBUG.
};

#endif	// #ifndef BARRIER_H.