#ifndef STG_ACTOR_H
#define STG_ACTOR_H

#include "..\GameObject.h"
#include <vector>

class CCollisionManager;	// 当たり判定クラス.

namespace STG
{
	class CBullet;	// 弾クラス.
	class CActor : public CGameObject
	{
	public:
		CActor();
		virtual ~CActor();

		// メッシュの描画.
		virtual void MeshRender();
		// 当たり判定.
		virtual void Collision( STG::CActor* pActor ) = 0;

		// 当たり判定の取得.
		CCollisionManager* GetColl(){ return m_pCollManager.get(); }

		// 目的の座標の設定.
		void SetTargetPos( const D3DXVECTOR3& pos ){ m_TargetPositon = pos; }

		// 動作するかどうか設定.
		void SetActive( const float& isActive ){ m_IsActive = isActive; }
		// 動作しているか取得.
		bool GetActive() const { return m_IsActive; }

		// 弾の取得.
		virtual std::vector<std::shared_ptr<CBullet>> GetBullets(){ return std::vector<std::shared_ptr<CBullet>>(); }

	protected:
		std::shared_ptr<CDX9StaticMesh>		m_pStaticMesh;	// スタティックメッシュ.
		std::shared_ptr<CCollisionManager>	m_pCollManager;	// 当たり判定クラス.
		D3DXVECTOR3 m_MoveVector;		// 移動用ベクトル.
		D3DXVECTOR3 m_TargetPositon;	// 目的の座標.
		bool		m_IsActive;			// 動作しているかどうか.
	};
};

#endif	// #ifndef STG_ACTOR_H.