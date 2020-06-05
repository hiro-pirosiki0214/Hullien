#ifndef LIGHT_BASE_H
#define LIGHT_BASE_H

#include "..\Global.h"

class CLightBase
{
public:
	CLightBase();
	virtual ~CLightBase();


	// 座標の設定.
	void SetPosition( const D3DXVECTOR3& pos ){ m_vPosition = pos; }
	// 座標の取得.
	D3DXVECTOR3 GetPosition() const { return m_vPosition; }

	// 方向の設定.
	void SetDirection( const D3DXVECTOR3& dir ){ m_vDirection = dir; }
	// 方向の取得.
	D3DXVECTOR3 GetDirection() const { return m_vDirection; }

	// 回転行列の設定.
	void SetRorarionMatrix( const D3DXMATRIX& rot ){ m_mRotation = rot; }
	// 回転行列の取得.
	D3DXMATRIX GetRorarionMatrix() const { return m_mRotation; }

	// 強さの設定.
	void SetIntensity( const float& intensity ){ m_fIntensity = intensity; }
	// 強さの取得.
	float GetIntensity() const { return m_fIntensity; }
	
protected:
	D3DXVECTOR3	m_vPosition;	// 位置.
	D3DXVECTOR3	m_vDirection;	// 方向.
	D3DXMATRIX	m_mRotation;	// 回転行列.
	float		m_fIntensity;	// 強度(明るさ).
};

#endif	// #ifndef LIGHT_BASE_H.