#ifndef STG_ENEMY_H
#define STG_ENEMY_H

#include "..\STGCharacter.h"
#include "STGEnemyParam.h"

class CFont;

namespace STG
{
	class CEnemy : public STG::CCharacter
	{
		inline static const char* BULLET_MODEL_NAME		= "item cure";
		inline static const float INIT_POSITION_Z		= -100.0f;	// 初期座標 : Z.
		inline static const float MOVE_SUB_VALUE		= 0.002f;	// 移動速度を引く値.
		inline static const float MOVE_SUB_POSITION_Z	= -10.0f;	// 移動速度を引いていく座標.

		inline static const D3DXVECTOR3 FONT_ROTATION =	// フォントの回転値.
		{
			static_cast<float>(D3DXToRadian(270)),
			0.0f, 
			static_cast<float>(D3DXToRadian(180))
		};

		const STG::SEnemyParam PARAMETER;	// パラメータ.

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
		CEnemy( const STG::SEnemyParam& param );
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