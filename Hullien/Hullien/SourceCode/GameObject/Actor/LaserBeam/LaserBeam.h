#ifndef LASER_BEAM_H
#define LASER_BEAM_H

#include "..\Actor.h"

class CLaserBeam : public CActor
{
public:
	CLaserBeam();
	virtual ~CLaserBeam();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& actor ) override;

	// ビームを打つ.
	void Shot();
	// パラメータを初期に戻す.
	void ResetParam();

	// 攻撃中か.
	bool IsInAttack() const { return m_IsInAttack; }
	// 攻撃が終了したかどうか.
	bool IsEndAttack() const { return m_IsEndAttack; }

private:
	// 当たり判定の設定.
	bool CollisionSetting();

private:
	D3DXVECTOR3	m_TargetPosition;	// 攻撃対象の座標.
	bool m_IsInAttack;		// 攻撃中.
	bool m_IsEndAttack;		// 攻撃終了.
};

#endif	// #ifndef LASER_BEAM_H.