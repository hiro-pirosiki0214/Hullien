#ifndef STG_BULLET_H
#define STG_BULLET_H

#include "..\STGActor.h"

namespace STG
{
	class CBullet : public STG::CActor
	{
		const char* MODEL_NAME;	// モデル名 : 外部から取得.
	public:
		CBullet();
		CBullet( const char* modelName );
		virtual ~CBullet();

		// 初期化関数.
		virtual bool Init() override;
		// 更新関数.
		virtual void Update() override;
		// 描画関数.
		virtual void Render() override;
		// 撃つ.
		virtual bool Shoot( 
			const D3DXVECTOR3& pos,
			const float& rot, 
			const float& moveSpeed );
		// 撃ったか.
		bool IsShot() const { return m_IsShot; }

	private:
		float	m_MoveSpeed;	// 移動速度.
		bool	m_IsShot;		// 撃ったか.
	};
};

#endif	// #ifndef STG_BULLET_H