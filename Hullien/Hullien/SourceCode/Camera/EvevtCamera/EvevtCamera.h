#ifndef EVENT_CAMERA_H
#define EVENT_CAMERA_H

#include "..\Camera.h"

/****************************************
* イベント用カメラ.
**/
class CEventCamera : public CCamera
{
	const float INITIAL_VIEWING_ANGLE = static_cast<float>(D3DX_PI / 4.0);	// 初期視野角.

public:
	// カメラのパラメータ.
	struct stCameraState
	{
		D3DXVECTOR3 vPosition;		// 位置.
		D3DXVECTOR3 vRotation;		// 回転値.
		D3DXVECTOR3 vLookPosition;	// 注視位置.
		D3DXVECTOR3 vLenght;		// ターゲットへの長さ.
		float		ViewingAngle;	// 視野角.
		float		MoveSpeed;		// 移動速度.

		stCameraState()
			: vPosition			({0.0f,0.0f,0.0f})
			, vRotation			({0.0f,0.0f,0.0f})
			, vLookPosition		({0.0f,0.0f,0.0f})
			, vLenght			({10.0f,0.0f,7.0f})
			, ViewingAngle		(static_cast<float>(D3DX_PI / 4.0))
			, MoveSpeed			(0.0f)
		{}

	}typedef SCameraState;

public:
	CEventCamera();
	virtual ~CEventCamera();

	// 視野角取得関数.
	float GetViewingAngle() const { return m_ViewingAngle; }
	// 視野角リセット.
	float ResetViewingAngle() { return INITIAL_VIEWING_ANGLE; }
	// 目的地に移動.
	D3DXVECTOR3 MoveToTargetPosition(const D3DXVECTOR3& vTargetPos, const float& speed);
	// オブジェクトを中心に回転.
	void RotationLookAtObject(const D3DXVECTOR3& vLookPos, const float& Degree);
	// カメラの情報設定.
	void SetState(const SCameraState& param);

private:
	SCameraState m_Parameter;

};

#endif //#ifndef EVENT_CAMERA_H.