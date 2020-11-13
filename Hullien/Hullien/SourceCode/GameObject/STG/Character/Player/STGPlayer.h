#ifndef STG_PLAYER_H
#define STG_PLAYER_H

#include "..\STGCharacter.h"

namespace STG
{
	class CPlayer : public STG::CCharacter
	{
		const char* MODEL_NAME			= "ufo_mini";
		const char* BULLET_MODEL_NAME	= "item cure";
		const char* SHOT_SE_NAME		= "STGshot";
		const float LIFE_POINT_MAX		= 5.0f;						// ライフの最大数.
		const float MOVE_SPEED			= 0.4f;						// 移動速度.
		const float BULLET_MOVE_SPEED	= 0.8f;						// 弾の速度.
		const int	BULLET_COUNT_MAX	= 30;						// 弾の最大数.
		const int	SHOT_INTERVAL_FRAME	= 15;						// 弾を撃つ間隔フレーム.
		const D3DXVECTOR3 BULLET_COLOR	= { 0.0f, 1.0f, 1.0f };		// 弾の色.
		const D3DXVECTOR3 INIT_POSITION = { 0.0f, 0.0f, 70.0f };	// 初期座標.

		const float SPAWN_END_POS_Z		= 40.0f;	// スポーンの終了座標.
		const float SPAWN_SPEED_SUB		= 0.005f;	// スポーンの移動速度の減算値.

		const float OUT_POSITION_X		= 100.0f;	// 画面外の座標 : X.
		const float OUT_POSITION_Z		= 55.0f;	// 画面外の座標 : Z.
		const float DEAD_SPEED			= 0.02f;	// 死亡速度.
	public:
		CPlayer();
		virtual ~CPlayer();

		// 初期化関数.
		virtual bool Init() override;
		// 更新関数.
		virtual void Update() override;
		// 描画関数.
		virtual void Render() override;
		// 当たり判定.
		virtual void Collision( STG::CActor* pActor ) override;
		// 操作関数.
		void Controller();

	private:
		// スポーン移動.
		void SpawnMove();
		// 移動関数.
		virtual void Move() override;
		// 死亡後処理.
		void DeadUpdate();
		// 弾を撃つ操作関数.
		void ShotController();

		// ライフ計算関数.
		virtual void LifeCalculation( const std::function<void(float&)>& ) override;

		// 画面外に行った時の処理.
		void OutDispMove();

		// 当たり判定の作成.
		bool CollisionInit();
		
	private:
		D3DXVECTOR3 m_Direction;		// 方向.
		float		m_SpawnMoveSpeed;	// スポーン時の移動速度.
		bool		m_IsDead;			// 死亡したか.
	};
};

#endif	// #ifndef STG_PLAYER_H.