#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "..\Actor.h"

class CEffectManager;	// �G�t�F�N�g�N���X.

class CExplosion : public CActor
{
	const char* EFFECT_NAME = "c_deadEffect";	// �G�t�F�N�g�̖��O.
public:
	// �����p�����[�^�[.
	struct stExplosionParam
	{
		float		ExplosionSpeed;		// �������x.
		float		AttackPower;		// ������.
		float		SphereMaxRadius;	// �X�t�B�A�̍ő唼�a.
		D3DXVECTOR3	SphereAdjPos;		// �X�t�B�A�̒����p���W.

		stExplosionParam()
			: ExplosionSpeed	( 0.0f )
			, AttackPower		( 0.0f )
			, SphereMaxRadius	( 0.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		{}
	} typedef SExplosionParam;


public:
	CExplosion();
	virtual ~CExplosion();

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
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& pActor ) override;
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;
	
	// �Đ����Ă��邩�ǂ���.
	inline bool IsPlay() { return m_IsEffectPlay; }
	// �~�܂��Ă��邩�ǂ���.
	bool IsStop();

	// �p�����[�^�[�̐ݒ�.
	inline void SetExplosionParam( const SExplosionParam& param ){ m_Param = param; }

private:
	// �G�t�F�N�g�̐ݒ�.
	bool EffectSetting();
	// �����蔻��̐ݒ�.
	bool ColliderSetting();

private:
	std::shared_ptr<CEffectManager>	m_pEffect;	// �G�t�F�N�g.
	SExplosionParam	m_Param;	// �����p�����[�^�[.
	float m_CollSphereRadius;	// �����蔻��̔��a.
	bool m_IsEffectPlay;		// �G�t�F�N�g���Đ����ꂽ��.
#if _DEBUG
	float m_ResizeCollTime;		// �����蔻��̔��a�̃��T�C�Y���鎞��(�f�o�b�O�p).
#endif	// #if _DEBUG.
};

#endif	// #ifndef EXPLOSION_H.