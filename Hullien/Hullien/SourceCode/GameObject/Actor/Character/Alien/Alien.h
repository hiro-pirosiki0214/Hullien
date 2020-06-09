#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"

class CAlien : public CCharacter
{
	const float ROTATIONAL_SPEED = 0.05f;	// 回転速度.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));
public:
	CAlien();
	virtual ~CAlien();

	// 相手座標の設定.
	virtual void SetOpponentPos( CActor& actor ) override;
protected:
	// 移動ベクトル設定関数.
	void SetMoveVector( const D3DXVECTOR3& targetPos );
	// 目的の座標へ回転.
	void TargetRotation();

protected:
	D3DXVECTOR3	m_GirlPosition;		// 女の子の座標.
	D3DXVECTOR3	m_TargetRotation;	// 目標の回転情報.
};

#endif	// #ifndef ALIEN_H.