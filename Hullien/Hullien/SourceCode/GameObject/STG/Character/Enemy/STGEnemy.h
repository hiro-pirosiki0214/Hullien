#ifndef STG_ENEMY_H
#define STG_ENEMY_H

#include "..\STGCharacter.h"

class CFont;

namespace STG
{
	class CEnemy : public STG::CCharacter
	{
		const char* MODEL_NAME			= "a";
		const char* BULLET_MODEL_NAME	= "item cure";

		const float ANGLE_MAX = static_cast<float>(D3DXToRadian(360.0));	// 弾の最大.
		const float ANGLE_MIN = static_cast<float>(D3DXToRadian(0.0));		// 弾の最大.

		const float INIT_POSITION_Z		= -100.0f;
		const float MOVE_SUB_VALUE		= 0.002f;
		const float MOVE_SUB_POSITION_Z = -10.0f;

		const D3DXVECTOR3 FONT_ROTATION =	// フォントの回転値.
		{
			static_cast<float>(D3DXToRadian(270)),
			0.0f, 
			static_cast<float>(D3DXToRadian(180))
		};

		const char* RENDER_CHAR				= "FukutaReiya";
		const int	SPAWN_TIME				= 10;		// スポーンの時間.
		const float MOVE_SPEED				= 0.2f;		// 移動速度.
		const float BULLET_MOVE_SPEED		= 0.1f;		// 弾の速度.
		const float	BULLET_ANGLE			= static_cast<float>(D3DXToRadian(25.0));	// 弾の角度.
		const int	BULLET_COUNT_MAX		= 30;		// 弾の最大数.
		const int	ANY_BULLET_COUNT_MAX	= 15;		// 複数弾を撃つ際の最大数.
		const int	SHOT_INTERVAL_FRAME		= 20;		// 弾を撃つ間隔フレーム.
		const int	SHOT_BULLET_COUNT		= 2;		// 一発で出る弾の数.
		const float SHOT_ANGLE				= static_cast<float>(D3DXToRadian(5.0));	// 撃つ時の角度.
		const int	SHOT_NUMBER				= 2;	// 弾の番号 1 : 一発ずつ, 2 : 複数.

		// 現在の状態.
		enum enState
		{
			EState_None,

			EState_Spawn,	// スポーン.
			EState_Move,	// 移動.
			EState_Shot,	// 撃つ.
			EState_Escape,	// 逃げる.
			EState_Dead,	// 死亡.

			EState_Max,
		} typedef EState;
	public:
		CEnemy();
		virtual ~CEnemy();

		// 初期化関数.
		virtual bool Init() override;
		// 更新関数.
		virtual void Update() override;
		// 描画関数.
		virtual void Render() override;
		// 当たり判定.
		virtual void Collision( STG::CActor* pActor ) override;

	private:
		// スポーン.
		void Spawn();
		// 移動関数.
		virtual void Move() override;
		// 弾を撃つ.
		void Shot();
		// 逃げる.
		void Escape();
		// 死亡.
		void Dead();

		// 弾を撃つ(一発ずつ).
		virtual void BulletShot( const float& rot, const float& moveSpeed ) override;
		// 弾を撃つ(複数).
		void BulletShotAnyWay( 
			const float& rot, 
			const float& angle, 
			const float& moveSpeed,
			const int& bulletCount );

		// 当たり判定の作成.
		bool CollisionInit();

	private:
		std::unique_ptr<CFont> m_pFont;		// フォントクラス.
		EState	m_NowState;					// 現在の状態.
		float	m_MoveSpeed;				// 移動速度.
		int		m_SpawnCount;				// スポーンカウント.
		float	m_Angle;					// 角度.
		int		m_NowShotBulletCount;		// 現在の撃った弾の数.
	};
}

#endif	// #ifndef STG_ENEMY_H.