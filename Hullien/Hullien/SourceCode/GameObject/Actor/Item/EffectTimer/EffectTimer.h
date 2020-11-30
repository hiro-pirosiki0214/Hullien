#ifndef EFFECT_TIMER_H
#define EFFECT_TIMER_H

class CEffectTimer
{
public:
	CEffectTimer();
	~CEffectTimer();

	// �^�C�}�[�̐ݒ�.
	void Set();

	// �^�C���̐ݒ�.
	inline void SetTime( float time ){ m_Time = time; }
	// �^�C���̎擾.
	inline float GetTime(){ return m_Time; }
	// �X�V.
	bool Update();
	//  �X�V����.
	inline bool IsUpdate(){ return m_IsUpdate; }

private:
	bool	m_IsUpdate;	// �X�V����.
	float	m_Time;		// ����.
};

#endif	// #ifndef EFFECT_TIMER_H.
