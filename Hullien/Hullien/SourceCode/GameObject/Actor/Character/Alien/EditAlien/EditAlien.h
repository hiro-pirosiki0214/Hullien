#ifndef EDIT_ALIEN_H
#define EDIT_ALIEN_H

#include "..\Alien.h"

class CEditAlien : public CAlien
{
	inline static const float MOVE_DISTANCE = 50.0f;	// 移動距離.
public:
	CEditAlien();
	virtual ~CEditAlien();

	// スポーン.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) override;

	// 攻撃の再生.
	virtual void PlayAttack();
	// 怯みの再生.
	void PlayFright();
	// 死亡の再生.
	void PlayDeath();
	// マザーシップに上る動作の再生.
	void PlayRisingMotherShip( const D3DXVECTOR3& vPos );
	// パラメーターの設定.
	virtual void SetParamter( const SAlienParam& param ){ m_Paramter = param; }

protected:
	// 待機関数.
	virtual void WaitMove();

	// スポーン中.
	virtual void Spawning() override;
	// 移動.
	virtual void Move() override;
	// 拐う.
	virtual void Abduct() override;
	// ノックバック.
	virtual void KnockBack() override;
	// 怯み.
	virtual void Fright() override;
	// 死亡.
	virtual void Death() override;
	// 逃げる.
	virtual void Escape() override;
	// マザーシップに昇っている.
	virtual void RisingMotherShip() override;

protected:
	SAlienParam m_Paramter;
	bool		m_IsPlaying;
};

#endif	// #ifndef EDIT_ALIEN_H.