#ifndef GIRL_H
#define GIRL_H

#include "..\Character.h"

class CGirl : public CCharacter
{
	const char* MODEL_NAME = "Towa_s";	// ���f����.
	const char* MODEL_TEMP_NAME = "yuri-dysize";	// �����f����.

	// �v���C���[�p�����[�^.
	struct stGirlParam : public stParameter
	{
		float		SearchCollRadius;	// ���G�p�̃X�t�B�A�̔��a.
		D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
		float		SphereAdjRadius;	// �X�t�B�A�̒������a.

		stGirlParam()
			: SearchCollRadius	( 10.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius	( 0.0f )
		{}
	} typedef SGirlParam;

	// ���݂̏��.
	enum class enNowState
	{
		None,

		Protected,	// ����Ă���.
		Abduct,		// �A�ꋎ��Ă���.
		Move,		// �ړ�.

		Max,

	} typedef ENowState;

	// �ړ����.
	enum class enMoveState
	{
		None,

		Rotation,	// ��].
		Move,		// �ړ�.
		Wait,		// �ҋ@.

		Max,
	} typedef EMoveState;
public:
	CGirl();
	~CGirl();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& actor ) override;
	// �댯�ȏ�Ԃ�.
	bool IsDanger(){ return m_IsDanger; }

private:
	// �ړ��֐�.
	virtual void Move() override;
	// �ړI�̏ꏊ�Ɍ����ĉ�].
	void TargetRotation();
	// �ړI�̏ꏊ�Ɍ����Ĉړ�.
	void TargetMove();

	// ���G�̓����蔻��.
	void SearchCollision( CActor* pActor );

	// �����蔻��̍쐬.
	bool  ColliderSetting();

private:
	SGirlParam m_Parameter;	// ���̎q�̃p�����[�^�[.
	std::shared_ptr<CCollisionManager>	m_pSearchCollManager;	// ���G�p�̓����蔻��.
	D3DXVECTOR3	m_OldPosition;	// �O��̍��W.
	ENowState	m_NowState;		// ���݂̏��.
	EMoveState	m_NowMoveState;	// ���݂̈ړ����.
	bool		m_IsDanger;		// �댯���ǂ���.
};

#endif	// #ifndef GIRL_H.