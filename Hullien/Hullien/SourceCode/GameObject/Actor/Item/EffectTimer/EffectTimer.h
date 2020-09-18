#ifndef EFFECT_TIMER_H
#define EFFECT_TIMER_H

class CEffectTimer
{
public:
	CEffectTimer();
	~CEffectTimer();

	// タイマーの設定.
	void Set();

	// タイムの設定.
	void SetTime( float time ){ m_Time = time; }
	// タイムの取得.
	float GetTime(){ return m_Time; }
	// 更新.
	bool Update();
	//  更新中か.
	bool IsUpdate(){ return m_IsUpdate; }

private:
	bool	m_IsUpdate;	// 更新中か.
	float	m_Time;		// 時間.
};

#endif	// #ifndef EFFECT_TIMER_H.
