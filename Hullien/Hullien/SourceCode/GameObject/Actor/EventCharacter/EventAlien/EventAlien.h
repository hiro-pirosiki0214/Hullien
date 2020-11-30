#ifndef EVENT_ALIEN_H
#define EVENT_ALIEN_H

#include "..\EventCharacter.h"

class CArm;

/****************************************
*	�C�x���g�p�F���l�N���X.
**/
class CEventAlien : public CEventCharacter
{
	
public:
	// �F���l���.
	enum class enEventAlienState
	{
		None,

		Spawn,		// �X�|�[��.
		Move,		// �ړ�.
		Abduct,		// ����.
		Escape,		// ������.
		BlowAway,	// �������.
		Wait,		// �ҋ@.

		Max,

	} typedef EEventAlienState;

	enum enAnimNo
	{
		EAnimNo_None = -1,

		EAnimNo_Move,
		EAnimNo_Arm,
		EAnimNo_Damage,
		EAnimNo_Dead,

		EAnimNo_Max,

		EAnimNo_Begin	= EAnimNo_Move,
		EAnimNo_End		= EAnimNo_Max,
	} typedef EAnimNo;

public:
	CEventAlien();
	virtual ~CEventAlien();

	// ������W�̐ݒ�.
	virtual void SetTargetPos(CActor& actor) override;
	// �o���A�ɓ������Ă��邩.
	inline bool IsBarrierHit() const { return m_IsBarrierHit; }
	// �F���l�̏�Ԑݒ�.
	inline void SetAlienState(const EEventAlienState& state) { m_NowState = state; }
	// �^�[�Q�b�g�ʒu�擾�֐�.
	inline D3DXVECTOR3 GetTargetPosition() const { return m_vTargetPosition; }
	// �^�[�Q�b�g������ł��邩.
	bool IsGrab() const;


private:
	// �X�|�[��.
	virtual bool Spawn(const D3DXVECTOR3& spawnPos) = 0;
	// ���f���̃A���t�@�l�̎擾.
	float GetModelAplha() const { return m_Parameter.ModelAlpha; }
	// �A�ꋎ���Ă��邩�ǂ���.
	bool IsAbduct() const { return m_NowState == EEventAlienState::Abduct; }
	// �A�ꋎ��UFO�̍��W�̎擾.
	void SetAbductUFOPosition(D3DXVECTOR3* pos) { m_pAbductUFOPosition = pos; }
	

protected:
	// ���݂̏�Ԃ̍X�V�֐�.
	void CurrentStateUpdate();
	// ���̎q�̍��W��ݒ�.
	void SetGirlPos(CActor& actor);

	// �X�|�[����.
	virtual void Spawning();

	// ���̎q�Ƃ̓����蔻��.
	void GirlCollision(CActor* pActor);
	// �o���A�Ƃ̓����蔻��.
	void BarrierCollision(CActor* pActor);

protected:
	std::shared_ptr<CArm>	m_pArm;						// �A�[���N���X.
	D3DXVECTOR3*			m_pAbductUFOPosition;		// UFO�̍��W.
	D3DXVECTOR3				m_vTargetPosition;			// �^�[�Q�b�g�ʒu.
	EEventAlienState		m_NowState;					// ���݂̏��.
	bool					m_IsBarrierHit;				// �o���A�ɓ������Ă��邩.
	float					m_Speed;					// �ړ����x,

};


#endif	//#ifndef EVENT_ALIEN_H.