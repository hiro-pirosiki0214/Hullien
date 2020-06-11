#ifndef COLLSION_MANAGER_H
#define COLLSION_MANAGER_H

#include "..\..\Global.h"

class CSphereCollision;
class COBBoxCollision;
class CCapsuleCollision;

//----------------------------------.
//	当たり判定マネージャークラス.
//	 各オブジェクトはこのクラスを使用する.
//----------------------------------.
class CCollisionManager
{
public: //------ 関数 -------.
	CCollisionManager();
	~CCollisionManager();

	// 初期化.
	HRESULT Init( LPD3DXMESH pMesh, D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, float* pScale );
	// スフィアの初期化.
	HRESULT InitSphere( 
		LPD3DXMESH pMesh, D3DXVECTOR3* pPos, 
		D3DXVECTOR3* pRot, float* pScale,
		const D3DXVECTOR3& adjPos, const float& adjRadius );
	// ボックスの初期化.
	HRESULT InitBox( 
		LPD3DXMESH pMesh, D3DXVECTOR3* pPos, 
		D3DXVECTOR3* pRot, float* pScale,
		const D3DXVECTOR3& adjPos, const D3DXVECTOR3& adjlength );
	// カプセルの初期化.
	HRESULT InitCapsule( 
		LPD3DXMESH pMesh, D3DXVECTOR3* pPos, 
		D3DXVECTOR3* pRot, float* pScale,
		const float& adjRadius, const float& adjHeight );

	// デバッグ用描画.
	void DebugRender();

	/*
	//- 当たり判定関数 -.
	*/

	// スフィア同士の当たり判定.
	bool IsShereToShere( CCollisionManager* pManager );
	// OBB同士の当たり判定.
	bool IsOBBToOBB( CCollisionManager* pManager );
	// カプセル同士の当たり判定.
	bool IsCapsuleToCapsule( CCollisionManager* pManager );

	/*
	//- 取得関数 -.
	*/

	// スフィア取得関数.
	CSphereCollision* GetSphere() const { return m_pSphere; }
	// ボックス取得関数.
	COBBoxCollision* GetOBB() const { return m_pBox; }
	// カプセル取得関数.
	CCapsuleCollision* GetCapsule() const { return m_pCapsule; }

	/*
	//- 調整用関数 -.
	*/

	// スフィアの調整用座標設定.
	void SetSphereAdjPosition( const D3DXVECTOR3& pos );
	// スフィアの調整用半径設定.
	void SetSphereAdjRadius( const float& radius );
	// ボックスの調整用座標設定.
	void SetBoxAdjPosition( const D3DXVECTOR3& pos );
	// ボックスの調整用長さ設定.
	void SetBoxAdjLength( const D3DXVECTOR3& length );

private:
	// スフィアの描画.
	void SphereRender();
	// ボックスの描画.
	void BoxRender();
	// カプセルの描画.
	void CapsuleRender();

private: //------ 変数 -------.
	CSphereCollision*	m_pSphere;	// スフィア.
	COBBoxCollision*	m_pBox;		// ボックス.
	CCapsuleCollision*	m_pCapsule;	// カプセル.
};

#endif	// #ifndef COLLSION_MANAGER_H.