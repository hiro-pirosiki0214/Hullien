#ifndef STG_PLAYER_H
#define STG_PLAYER_H

#include "..\STGCharacter.h"

namespace STG
{
	class CPlayer : public STG::CCharacter
	{
		const char* MODEL_NAME = "ufo_mini";
		const char* BULLET_MODEL_NAME = "item cure";
		const float MOVE_SPEED = 0.2f;
		const float BULLET_MOVE_SPEED	= 0.5f;	// 弾の速度.
		const int	BULLET_COUNT_MAX	= 30;	// 弾の最大数.
		const int	SHOT_INTERVAL_FRAME	= 15;	// 弾を撃つ間隔フレーム.
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
		// 移動関数.
		virtual void Move() override;
		// 弾を撃つ操作関数.
		void ShotController();

		// 当たり判定の作成.
		bool CollisionInit();
		
	private:
		D3DXVECTOR3 m_Direction;
	};
};

#endif	// #ifndef STG_PLAYER_H.