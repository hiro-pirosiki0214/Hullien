#ifndef EVENT_GIRL_H
#define EVENT_GIRL_H

#include "..\EventCharacter.h"
#include "..\..\Character\Girl\GirlParam.h"

/****************************************
*	�C�x���g�p���̎q�N���X.
**/
class CEventGirl : public CEventCharacter
{
	const char* MODEL_NAME = "yuri-dy_s";	// ���f����.

	// �p�����[�^.
	struct stGirlParam : public SCharacterParam
	{
		float		SearchCollRadius;	// ���G�p�̃X�t�B�A�̔��a.

		stGirlParam()
			: SearchCollRadius(10.0f)
		{}
	} typedef SGirlParam;

public:
	// ���݂̏��.
	enum class enNowState
	{
		None,

		Protected,	// ����Ă���.
		Abduct,		// �A�ꋎ��Ă���.
		Move,			// �ړ�.
		Wait,			// �ҋ@.

		Max,

	} typedef ENowState;

public:
	CEventGirl();
	virtual ~CEventGirl();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision(CActor* pActor) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos(CActor& actor) override;
	// ���̎q���댯�ȏ�Ԃ�.
	inline bool GetIsDanger() const { return m_IsDanger; }
	// ���ݒ�֐�.
	virtual void SetOptionalState(const SOptionalState& state) override;

	// ���̎q�̏�Ԑݒ�֐�.
	inline void SetNowState(const ENowState& state) { m_NowState = state; }

private:
	// �ړ��֐�.
	virtual void Move() override;

	// ���G�̓����蔻��.
	void SearchCollision(CActor* pActor);

	// �����蔻��̍쐬.
	bool  ColliderSetting();

private:
	SGirlParam m_Parameter;	// ���̎q�̃p�����[�^�[.
	std::shared_ptr<CCollisionManager>	m_pSearchCollManager;	// ���G�p�̓����蔻��.
	D3DXVECTOR3	m_OldPosition;			// �O��̍��W.
	ENowState	m_NowState;				// ���݂̏��.
	bool		m_IsDanger;				// �댯���ǂ���.

};

#endif //#ifndef EVENT_GIRL_H.
