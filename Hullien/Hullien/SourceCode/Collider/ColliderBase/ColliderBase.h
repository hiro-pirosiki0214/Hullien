#ifndef COLLIDER_BASE_H
#define COLLIDER_BASE_H

#include "..\..\Global.h"

class CColliderBase
{
public: //------- 関数 -------.
	CColliderBase();
	virtual ~CColliderBase();

	// 当たり判定の表示.
	virtual void DebugRender() = 0;

	// 座標の取得.
	D3DXVECTOR3 GetPosition() const { return (*m_pvPosition) + m_vAdjPosition; }
	// 座標ポインターの設定.
	void SetPosition( D3DXVECTOR3* pvPosition ){ m_pvPosition = pvPosition; }
	// 調整用の座標の設定.
	void SetAdjPosition( const D3DXVECTOR3& pos ){ m_vAdjPosition = pos; }

	// 回転情報の取得.
	D3DXVECTOR3 GetRotation() const { return (*m_pvRotation); }
	// 回転情報ポインターの設定.
	void SetRotation( D3DXVECTOR3* pvRotation ){ m_pvRotation = pvRotation; }

	// サイズの取得.
	float GetScale() const { return *m_pScale; }
	// サイズポインターの設定.
	void SetScale( float* pScale ){ m_pScale = pScale; }

	// メッシュの頂点の最大・最小を取得する.
	HRESULT GetBoundingBox( LPD3DXMESH pInMesh, D3DXVECTOR3& vOutMax, D3DXVECTOR3& vOutMin );
	// メッシュの頂点の最大・最小を取得する.
	HRESULT GetBoundingSphere( LPD3DXMESH pInMesh, float& fOutRadius );

protected: //------- 変数 -------.
	D3DXVECTOR3*	m_pvPosition;	// 現在の座標.
	D3DXVECTOR3		m_vAdjPosition;	// 調整用の座標.
	D3DXVECTOR3*	m_pvRotation;	// 現在の回転情報.
	float*			m_pScale;		// サイズ.
};

#endif	// #ifndef COLLIDER_BASE_H.