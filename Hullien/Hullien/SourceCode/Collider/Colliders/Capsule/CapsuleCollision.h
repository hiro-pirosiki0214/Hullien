#ifndef CAPSULE_COLLISION_H
#define CAPSULE_COLLISION_H

#include "..\..\ColliderBase\ColliderBase.h"
#include "..\..\..\Utility\MyVector\MyVector.h"

class CCapsuleModel;

class CCapsuleCollision : public CColliderBase
{
public:
	CCapsuleCollision();
	virtual ~CCapsuleCollision();

	// モデルの半径の作成.
	HRESULT InitModelCapsule( LPD3DXMESH pMesh );
	// 当たり判定の表示.
	virtual void DebugRender() override;

	// 色を変えるかどうか.
	void SetChangeColor( const bool& changed );

	// 半径の取得.
	float GetRadius() const { return m_Radius + m_AdjRadius; }

	// セグメント取得関数.
	Segment GetSegment();

	// 調整用の球体半径の設定.
	void SetAdjRadius( const float& radius ){ m_Radius = radius; }
	// 調整用の球体半径の設定.
	void SetAdjHeight( const float& height ){ m_AdjHeight = height; }

	// 2線分の最短距離.
	// s1 : S1(線分1).
	// s2 : S2(線分2).
	// p1 : S1側の垂線の足（戻り値）.
	// p2 : S2側の垂線の足（戻り値）.
	// t1 : S1側のベクトル係数（戻り値）.
	// t2 : S2側のベクトル係数（戻り値）.
	// 戻り値: 最短距離.
	float calcSegmentSegmentDist( 
		const Segment &s1, const Segment &s2, 
		Point &p1, Point &p2, 
		float &t1, float &t2 );

private:
	// 点と直線の最短距離.
	// p : 点.
	// l : 直線.
	// h : 点からおろした垂直の足(返り値).
	// t : ベクトル係数(戻り値).
	// 戻り値 : 最短距離.
	float calcPointLineDist( const Point& p, const Line& l, Point& h, float& t );

	// ∠p1p2p3は鋭角？.
	bool isSharpAngle( const Point& p1, const Point& p2, const Point& p3 );

	// 点と線分の最短距離.
	// p : 点.
	// seg : 線分.
	// h : 最短距離となる端点(戻り値).
	// t : 端点位置( t < 0 : 始点の外, 0 <= t <= 1 : 線分内, t > 1 : 線分の外 ).
	float calcPointSegmentDist( const Point& p, const Segment& seg, Point& h, float& t ); 

	// 2直線の最短距離.
	// l1 : L1.
	// l2 : L2.
	// p1 : L1側の垂直の足(戻り値).
	// p2 : L2側の垂直の足(戻り値).
	// t1 : L1側のベクトル係数(戻り値).
	// t2 : L2側のベクトル係数(戻り値).
	// 戻り値 : 最短距離.
	float calcLineLineDist( 
		const Line& l1, const Line& l2, 
		Point& p1, Point& p2, 
		float& t1, float& t2 );


	// 0～1の間にクランプ.
	void clamp01( float &v ) {
		if ( v < 0.0f )
			v = 0.0f;
		else if ( v > 1.0f )
			v = 1.0f;
	}

private:
	Segment m_Segment;	// セグメント.
	float m_Radius;		// カプセルの半径.
	float m_AdjRadius;	// 調整用のカプセルの半径.
	float m_Height;		// カプセルの高さ.
	float m_AdjHeight;	// 調整用のカプセルの高さ.

#ifdef _DEBUG
	std::unique_ptr<CCapsuleModel> m_pDebugCapsule;	// デバッグ用の球体.
	static bool m_IsRender;	// 描画するかどうか.
#endif	// #ifdef _DEBUG.
};

#endif	// #ifndef CAPSULE_COLLISION_H.