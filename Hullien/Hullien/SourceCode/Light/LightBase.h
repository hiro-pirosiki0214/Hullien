#ifndef LIGHT_BASE_H
#define LIGHT_BASE_H

#include "..\Global.h"

class CLightBase
{
	const float FLOAT_WND_W = static_cast<float>(WND_W);	// (float)画面幅.
	const float FLOAT_WND_H = static_cast<float>(WND_H);	// (float)画面幅.
	const float INITIAL_VIEWING_ANGLE	= static_cast<float>(3.141519/4.0);	// 初期視野角.
	const float SCREEN_ASPECT_RATIO		= FLOAT_WND_W/FLOAT_WND_H;			// 画面アスペクト比.
	const float INITIAL_MIN_CLIPPING_DISTANCE = 0.1f;			// 初期最小描画距離.
	const float INITIAL_MAX_CLIPPING_DISTANCE = 1000.f;			// 初期最大描画距離.
	const D3DXVECTOR3 VIEW_UP_VECTOR	= { 0.0f, 1.0f, 0.0f };	// 上方向ベクトル.

	static const int MAX_CASCADE = 4;	// 最大カスケード数.

										// バウンディングボックス(カスケードシャドウに使用).
	struct stBBox
	{
		D3DXVECTOR3 mini;
		D3DXVECTOR3 maxi;

		stBBox
		(
			D3DXVECTOR3 min,
			D3DXVECTOR3 max
		)
			: mini ( min )
			, maxi ( max )
		{}

	} typedef SBBox;

public:
	CLightBase();
	virtual ~CLightBase();

	// カスケード分のシャドウ行列を取得する.
	D3DXMATRIX* GetShadowVP();

	// カスケード分割座標を取得.
	float* GetSpritPos(){ return m_SplitPos; }

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

private:
	// 平行分割処理.
	void ComputeSplitPositions(
		const int splitCount, const float lamda, 
		const float nearClip, const float farClip, 
		float* positions );
	// 分割した視錘台の8角をもとめて，ライトのビュー射影空間でAABBを求める.
	SBBox CalculateFrustum( float nearClip, float farClip, D3DXMATRIX& viewProj );
	// クロップ行列を求める.
	D3DXMATRIX CreateCropMatrix( SBBox& box );

protected:
	D3DXMATRIX	m_ShadowMatrix[MAX_CASCADE];	// シャドウ用行列(カスケード分用意).
	float		m_SplitPos[MAX_CASCADE];		// カスケードの区切り座標.
	D3DXMATRIX	m_ProjMatrix;		// プロジェクション.
	D3DXMATRIX	m_ViewMatrix;		// ビュー.
	D3DXVECTOR3	m_vPosition;		// 位置.
	D3DXVECTOR3 m_vLookPosition;	// 新t年座標.
	D3DXVECTOR3	m_vDirection;		// 方向.
	D3DXMATRIX	m_mRotation;		// 回転行列.
	float		m_fIntensity;		// 強度(明るさ).

	float m_ViewingAngle;			// 視野角.
	float m_MaxClippingDistance;	// 最大クリッピング距離.
	float m_MinClippingDistance;	// 最小クリッピング距離.
};

#endif	// #ifndef LIGHT_BASE_H.