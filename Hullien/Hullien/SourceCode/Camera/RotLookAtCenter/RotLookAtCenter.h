#ifndef ROTATION_LOOK_AT_CENTER_CAMERA_H
#define ROTATION_LOOK_AT_CENTER_CAMERA_H

#include "..\Camera.h"

class CRotLookAtCenter : public CCamera
{
private: //------ 定数 -------.
	const float DEFAULT_LENGTH			= 20.0f;	// カメラ位置と注視位置の距離.
	const float DEFAULT_HORIZONTAL_MAX	= 360.0f;	// 横回転の最大サイズ.
	const float DEFAULT_HORIZONTAL_MIN	= 0.0f;		// 横回転の最小サイズ.
	const float DEFAULT_VERTICAL_MAX	= 90.0f;	// 縦回転の最大サイズ.
	const float DEFAULT_VERTICAL_MIN	= 0.0f;		// 縦回転の最小サイズ.

public:  //------ 関数 -------.
	CRotLookAtCenter();
	virtual ~CRotLookAtCenter();

	// 更新.
	virtual void Update() override;

	// オブジェクトをを注視に回転.
	void RotationLookAtObject( const D3DXVECTOR3& vLookPos, const float& attenRate );

	// 横方向の回転.
	void DegreeHorizontalMove( const float& movePower );
	// 縦方向の回転.
	void DegreeVerticalMove( const float& movePower );
	// ラジアンの取得関数.
	float GetRadianX() const { return m_vDegree.x; }
	// カメラと注視点の長さ設定.
	void SetLength( const float& length );
	// 高さの設定.
	void SetHeight( const float& height );
	// 横方向の最大、最小の設定.
	void SetHorizontalDegree( const float& max, const float& min );
	// 縦方向の最大、最小の設定.
	void SetVerticalDegree( const float& max, const float& min );

private: //------ 変数 -------.
	D3DXVECTOR2 m_vDegree;	// カメラの角度.
	float m_Length;			// カメラと注視点の長さ.
	float m_HorizontalMax;	// 横方向の最大サイズ.
	float m_HorizontalMin;	// 横方向の最小サイズ.
	float m_VerticalMax;	// 縦方向の最大サイズ.
	float m_VerticalMin;	// 縦方向の最小サイズ.
};

#endif	// #ifndef ROTATION_LOOK_AT_CENTER_CAMERA_H.