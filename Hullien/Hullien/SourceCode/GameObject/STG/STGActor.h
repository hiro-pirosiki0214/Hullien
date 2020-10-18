#ifndef STG_ACTOR_H
#define STG_ACTOR_H

#include "..\GameObject.h"

namespace STG
{
	class CActor : public CGameObject
	{
	public:
		CActor();
		virtual ~CActor();

		// メッシュの描画.
		virtual void MeshRender();

	protected:
		std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;
		D3DXVECTOR3 m_MoveVector;	// 移動用ベクトル.
	};
};

#endif	// #ifndef STG_ACTOR_H.