#ifndef CAMERA_H
#define CAMERA_H

#include "..\Global.h"

//===========================.
//	カメラクラス.
//===========================.
class CCamera
{
private: //------- 定数 -------.
	const float FLOAT_WND_W = static_cast<float>(WND_W);	// (float)画面幅.
	const float FLOAT_WND_H = static_cast<float>(WND_H);	// (float)画面幅.
	const D3DXVECTOR3 VIEW_UP_VECTOR	= { 0.0f, 1.0f, 0.0f };				// 上方向ベクトル.
	const float INITIAL_VIEWING_ANGLE	= static_cast<float>(D3DX_PI/4.0);	// 初期視野角.
	const float SCREEN_ASPECT_RATIO		= FLOAT_WND_W/FLOAT_WND_H;			// 画面アスペクト比.
	const float INITIAL_MAX_CLIPPING_DISTANCE = 1000.0f;					// 初期最大描画距離.
	const float INITIAL_MIN_CLIPPING_DISTANCE = 0.1f;						// 初期最小描画距離.

public: //------- 関数 -------.
	CCamera();
	virtual ~CCamera();

	// カメラの更新処理.
	virtual void Update();
	// ビュー行列、プロジェクション行列の初期化.
	void InitViewProj();

	// ビュー行列取得関数.
	D3DXMATRIX GetViewMatrix() const { return m_ViewMatrix; }
	// プロジェクション行列取得関数.
	D3DXMATRIX GetProjMatrix() const { return m_ProjMatrix; }
	// カメラ座標取得関数.
	virtual D3DXVECTOR3 GetPosition()		const { return m_vPosition; }
	// カメラ注視座標取得関数.
	D3DXVECTOR3 GetLookPosition()	const { return m_vLookPosition; }
	// カメラ座標設定関数.
	void SetPosition( const D3DXVECTOR3& vPos )			{ m_vPosition = vPos; }
	// カメラ注視座標設定関数.
	void SetLookPosition( const D3DXVECTOR3& vLookPos )	{ m_vLookPosition = vLookPos; }

	// 継承先のカメラ情報の設定関数.
	void SetCamera( std::shared_ptr<CCamera> camera );

protected: //------- 変数 -------.
	D3DXVECTOR3	m_vPosition;		// カメラ位置.
	D3DXVECTOR3	m_vLookPosition;	// カメラ注視位置.
	float m_ViewingAngle;			// 視野角.

private: //------- 変数 -------.
	D3DXMATRIX m_ViewMatrix;		// ビュー行列.
	D3DXMATRIX m_ProjMatrix;		// プロジェクション行列.

	float m_MaxClippingDistance;	// 最大クリッピング距離.
	float m_MinClippingDistance;	// 最小クリッピング距離.
};

#endif	// #ifndef CAMERA_H.