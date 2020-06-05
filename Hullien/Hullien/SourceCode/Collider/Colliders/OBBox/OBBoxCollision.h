#ifndef OBBOX_COLLISION_H
#define OBBOX_COLLISION_H

#include "..\..\ColliderBase\ColliderBase.h"

class CBoxModel;

class COBBoxCollision : public CColliderBase
{
public: //------ 関数 -------.
	COBBoxCollision();
	virtual ~COBBoxCollision();

	// モデルのボックスの作成.
	HRESULT InitModelBox( LPD3DXMESH pMesh );
	// 当たり判定の表示.
	virtual void DebugRender() override;

	// 色を変えるかどうか.
	void SetChangeColor( const bool& changed );
	
	// 方向ベクトル取得関数.
	D3DXVECTOR3 GetDirection( const int& index );
	// 長さの取得関数.
	D3DXVECTOR3 GetLength(){ return m_vLength; }
	// ボックスの調整用長さ設定.
	void SetAdjLength( const D3DXVECTOR3& length ){ m_vAdjLength = length; }

private: //----- 変数 ------.
	D3DXVECTOR3 m_vDirection[3];// 方向ベクトル.
	D3DXVECTOR3 m_vLength;		// 長さ.
	D3DXVECTOR3 m_vAdjLength;	// 調整用の長さ.
#ifdef _DEBUG
	std::unique_ptr<CBoxModel> m_pDebugBox;	// デバッグ用ボックスモデル.
	static bool m_IsRender;	// 描画するかどうか.
#endif	// #ifdef _DEBUG.
};

#endif	// #ifndef OBBOX_COLLISION_H.