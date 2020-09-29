#ifndef LASER_BEAM_H
#define LASER_BEAM_H

#include "..\Actor.h"
#include <list>

class CTrajectory;	// 軌跡.

class CLaserBeam : public CActor
{
	const float DEFAULT_MOVE_SPEED			= 0.1f;	// 移動速度.
	const float DEFAULT_PARALYSIS_TIME		= 3.0f;	// 麻痺の時間.
	const int	MAX_TRAJECTORY_COUNT		= 50;	// 軌跡の最大頂点数.
	const int	TRAJECTORY_TIME				= 1;	// 軌跡の頂点加算時間.
	const int	TRAJECTORY_END_ADD_VALUE	= 8;	// 軌跡が終了する際の加算値.
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
	void Shot( const D3DXVECTOR3& pos );
	// パラメータを初期に戻す.
	void ResetParam();
	// ベジェ曲線の制御点の設定.
	void SetControlPointList( std::vector<D3DXVECTOR3> pointList );

	// 移動速度の設定.
	void SetMoveSpped( const float& moveSpeed ){ m_MoveSpeed = moveSpeed; }
	// 麻痺時間の設定.
	void SetParalysisTime( const float& time ){ m_ParalysisTime = time; }

	// 攻撃中か.
	bool IsInAttack() const { return m_IsInAttack; }
	// 攻撃が終了したかどうか.
	bool IsEndAttack() const { return m_IsEndAttack; }

private:
	// 二次ベジェ曲線.
	void SecondaryBeziercurve();
	// 三次ベジェ曲線.
	void ThirdBezierCurve();

	// 当たり判定の設定.
	bool CollisionSetting();
	// 使用頂点の設定.
	void SetVertexPoint();
	// 頂点の作成.
	void CreateVertex();

private:
	std::unique_ptr<CTrajectory> m_pTrajectory;	// 軌跡描画クラス.
	float		m_MoveSpeed;		// 移動速度.
	float		m_ParalysisTime;	// 麻痺時間.
	D3DXVECTOR3	m_TargetPosition;	// 攻撃対象の座標.
	bool		m_IsInAttack;		// 攻撃中.
	bool		m_IsEndAttack;		// 攻撃終了.
	float		m_FrameCount;		// フレームカウント.
	float		m_FrameTime;		// フレーム時間.
	D3DXVECTOR3 m_InitPosition;		// 初期座標.
	std::vector<D3DXVECTOR3>	m_ControlPointList;		// 制御座標.
	std::vector<D3DXVECTOR3>	m_VertexPointList;		// 頂点の座標のリスト.
	int							m_VertexAddTimeCount;	// 頂点を加算する時間のカウント.
};

#endif	// #ifndef LASER_BEAM_H.