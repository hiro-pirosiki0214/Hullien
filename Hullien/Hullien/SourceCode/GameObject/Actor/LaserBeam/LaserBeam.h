#ifndef LASER_BEAM_H
#define LASER_BEAM_H

#include "..\Actor.h"
#include <queue>

class CLaserBeam : public CActor
{
	const float DEFAULT_MOVE_SPEED		= 0.1f;	// 移動速度.
	const float DEFAULT_PARALYSIS_TIME	= 3.0f;	// 麻痺の時間.
	const char* MODEL_NAME = "Barrier";
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

	// モデルの取得.
	bool GetModel();
	// 当たり判定の設定.
	bool CollisionSetting();

	void CreateVertex();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;	// モデル.
	float m_MoveSpeed;
	float m_ParalysisTime;
	D3DXVECTOR3	m_TargetPosition;	// 攻撃対象の座標.
	bool m_IsInAttack;		// 攻撃中.
	bool m_IsEndAttack;		// 攻撃終了.

	float m_FrameCount;		// フレームカウント.
	float m_FrameTime;		// フレーム時間.
	D3DXVECTOR3 m_InitPosition;	// 初期座標.
	std::vector<D3DXVECTOR3> m_ControlPointList;	// 制御座標.

	std::queue<std::pair<D3DXVECTOR3,D3DXVECTOR3>> m_VertexPoint;
	int m_VertexCount;
};

#endif	// #ifndef LASER_BEAM_H.