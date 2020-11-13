#ifndef STG_ENEMY_H
#define STG_ENEMY_H

#include "..\STGCharacter.h"
#include "STGEnemyParam.h"

#include <random>

class CFont;

namespace STG
{
	class CEnemy : public STG::CCharacter
	{
		inline static const float E_WND_OUT_ADJ_SIZE	= 10.0f;
		inline static const char* BULLET_MODEL_NAME		= "SpawnPoint";
		inline static const char* DEAD_SE_NAME			= "STGEnemyDead";
		inline static const float BULLET_COLOR			= 0.4f;
		inline static const float INIT_POSITION_Z		= -100.0f;	// 初期座標 : Z.
		inline static const float MOVE_SUB_VALUE		= 0.002f;	// 移動速度を引く値.
		inline static const float MOVE_SUB_POSITION_Z	= -10.0f;	// 移動速度を引いていく座標.
		inline static const float SHAKE_COUNT_MAX		= 10.0f;	// 揺れのカウント.
		inline static const float SHAKE_SUB_VALUE		= 0.4f;		// 揺れカウントを引く値.
		inline static const float SHAKE_SPEED			= 0.1f;		// 揺れの速度.
		inline static const float DEAD_SCALE_SUB_VALUE	= 0.02f;	// 死亡時のスケール減算値.
		inline static const float DEAD_ROTATION_SPEED	= 0.1f;		// 死亡時の回転速度.
		inline static const float ESCAPE_MOVE_DISTANCE	= 4.0f;		// 逃げる移動距離.
		inline static const float ESCAPE_COUNT_MAX		= 10*FPS;	// 逃げる時間.
		inline static const float		FONT_SIZE		= 2.0f;		// フォントサイズ.
		inline static const D3DXVECTOR3 FONT_ROTATION	=			// フォントの回転値.
		{
			static_cast<float>(D3DXToRadian(270)),
			0.0f, 
			static_cast<float>(D3DXToRadian(180))
		};

		const STG::SEnemyParam PARAMETER;	// パラメータ.

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
		// スポーン時間の取得.
		inline float GetSpawnTime() const { return PARAMETER.SpawnTime; }
		inline void SetPositionY( const float& posY ){ m_vPosition.y = posY; }
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

		// 当たった時の揺れ.
		void HitShake();

		// ライフ計算関数.
		virtual void LifeCalculation( const std::function<void(float&)>& ) override;

		// ランダムで移動ベクトルを検索.
		void SearchRandomMoveVector();

		// 当たり判定の作成.
		bool CollisionInit();

	private:
		std::unique_ptr<CFont>	m_pFont;				// フォントクラス.
		D3DXVECTOR3				m_FontRotation;			// フォントの回転値.
		STG::EEnemyState		m_NowState;				// 現在の状態.
		float					m_MoveSpeed;			// 移動速度.
		float					m_MoveingDistance;		// 移動距離.
		float					m_MoveingDistanceMax;	// 移動距離.
		int						m_SpawnCount;			// スポーンカウント.
		float					m_ShotAngle;			// 撃つときの角度.
		int						m_NowShotBulletCount;	// 現在の撃った弾の数.
		bool					m_IsHitShake;			// 当たった時の揺れをするか.
		float					m_ShakeCount;			// 揺れのカウント.
		float					m_EscapeCount;			// 逃げるカウント.
	};
}

#endif	// #ifndef STG_ENEMY_H.