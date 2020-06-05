#ifndef SPHERE_COLLISION_H
#define SPHERE_COLLISION_H

#include "..\..\ColliderBase\ColliderBase.h"

class CSphereModel;

class CSphereCollision : public CColliderBase
{
public:  //------- 関数 -------.
	CSphereCollision();
	virtual ~CSphereCollision();

	// モデルの半径の作成.
	HRESULT InitModelRadius( LPD3DXMESH pMesh );
	// 当たり判定の表示.
	virtual void DebugRender() override;

	// 色を変えるかどうか.
	void SetChangeColor( const bool& changed );
	// 半径の取得.
	float GetRadius() const { return m_Radius + m_AdjRadius; }
	// 調整用の球体半径の設定.
	void SetAdjRadius( const float& radius ){ m_Radius = radius; }

private: //------- 変数 -------.
	float m_Radius;		// 球体の半径.
	float m_AdjRadius;	// 調整用の球体半径.
#ifdef _DEBUG
	std::unique_ptr<CSphereModel> m_pDebugSphere;	// デバッグ用の球体.
	static bool m_IsRender;	// 描画するかどうか.
#endif	// #ifdef _DEBUG.
};

#endif	// #ifndef SPHERE_COLLISION_H.