#include "LightBase.h"
#include "..\Camera\CameraManager\CameraManager.h"

static const D3DXMATRIX SHADOW_BIAS = 
{
	0.5f,  0.0f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.0f, 0.0f,
	0.0f,  0.0f, 1.0f, 0.0f,
	0.5f,  0.5f, 0.0f, 1.0f 
};

CLightBase::CLightBase()
	: m_ShadowMatrix		()
	, m_SplitPos			()
	, m_ProjMatrix			()
	, m_ViewMatrix			()
	, m_vPosition			( 0.0f, 0.0f, -1.0f )
	, m_vLookPosition		( 0.0f, 0.5f, 0.0f )
	, m_vDirection			( 0.0f, 0.0f, 0.0f )
	, m_mRotation			()
	, m_fIntensity			( 0.0f )
	, m_ViewingAngle		( INITIAL_VIEWING_ANGLE )
	, m_MaxClippingDistance	( INITIAL_MAX_CLIPPING_DISTANCE )
	, m_MinClippingDistance	( INITIAL_MIN_CLIPPING_DISTANCE )
{
}

CLightBase::~CLightBase()
{
}

// カスケード分のシャドウ行列を取得する.
D3DXMATRIX* CLightBase::GetShadowVP()
{
	// ライト方向を取得.
	m_vDirection = m_vPosition - m_vLookPosition;
	// ビュー(カメラ)変換.
	D3DXMatrixLookAtLH( 
		&m_ViewMatrix,		// (out)viewMarix.
		&m_vPosition,		// カメラ座標.
		&m_vLookPosition,	// カメラ注視座標.
		&VIEW_UP_VECTOR );	// カメラベクトル.

	// プロジェクション(射影)変換.
	D3DXMatrixOrthoLH(
		&m_ProjMatrix,				// (out)ProjMatrix.
		600.0f,						// 視野角.
		600.0f,						// 画面アスペクト比.
		m_MinClippingDistance,		// 最小描画距離.
		m_MaxClippingDistance );	// 最大描画距離.

	float m_SplitPositions[MAX_CASCADE+1];

	// 平行分割処理.
	ComputeSplitPositions( MAX_CASCADE, 0.84f, m_MinClippingDistance, m_MaxClippingDistance, m_SplitPositions );

	// カスケード処理.
	for( int i=0; i<MAX_CASCADE; ++i )
	{
		// ライトのビュー射影行列.
		m_ShadowMatrix[i] = m_ViewMatrix * m_ProjMatrix * SHADOW_BIAS;

		// 分割した視錘台の8角をもとめて，ライトのビュー射影空間でAABBを求める.
		SBBox box = CalculateFrustum(
			m_SplitPositions[ i + 0 ],
			m_SplitPositions[ i + 1 ],
			m_ShadowMatrix[ i ] );

		// クロップ行列を求める.
		D3DXMATRIX crop = CreateCropMatrix( box );

		// シャドウマップ行列と分割位置を設定.
		m_ShadowMatrix[i] = m_ShadowMatrix[i] * crop;
		m_SplitPos[i]     = m_SplitPositions[i+1];
	}

	return m_ShadowMatrix;
}

// 平行分割処理.
void CLightBase::ComputeSplitPositions(
	const int splitCount, const float lamda, 
	const float nearClip, const float farClip, 
	float* positions )
{
	// 分割数が１の場合は，普通のシャドウマップと同じ.
	if ( splitCount == 1 )
	{
		positions[0] = nearClip;
		positions[1] = farClip;
		return;
	}

	float inv_m = 1.0f / float( splitCount );    // splitCountがゼロでないことは保証済み.

												 // ゼロ除算対策.
	_ASSERT( nearClip != 0.0f );

	// (f/n)を計算.
	float f_div_n = farClip / nearClip;

	// (f-n)を計算.
	float f_sub_n = farClip - nearClip;

	// 実用分割スキームを適用.
	// ※ GPU Gems 3, Chapter 10. Parallel-Split Shadow Maps on Programmable GPUs.
	//    http://http.developer.nvidia.com/GPUGems3/gpugems3_ch10.html を参照.
	for( int i=1; i<splitCount + 1; ++i )
	{
		// 対数分割スキームで計算.
		float Ci_log = nearClip * powf( f_div_n, inv_m * i );

		// 一様分割スキームで計算.
		float Ci_uni = nearClip + f_sub_n * i * inv_m;

		// 上記の２つの演算結果を線形補間する.
		positions[i] = lamda * Ci_log + Ci_uni * ( 1.0f - lamda );
	}

	// 最初は, ニア平面までの距離を設定.
	positions[ 0 ] = nearClip;

	// 最後は, ファー平面までの距離を設定.
	positions[ splitCount ] = farClip;
}

// 分割した視錘台の8角をもとめて，ライトのビュー射影空間でAABBを求める.
CLightBase::SBBox CLightBase::CalculateFrustum( float nearClip, float farClip, D3DXMATRIX& viewProj )
{
	D3DXVECTOR3 Dir = CCameraManager::GetLookPosition() - CCameraManager::GetPosition();
	D3DXVECTOR3 vZ;
	D3DXVec3Normalize( &vZ,&Dir );
	D3DXVECTOR3 vX;
	D3DXVec3Cross( &vX, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &vZ );
	D3DXVec3Normalize( &vX, &vX );
	D3DXVECTOR3 vY;
	D3DXVec3Cross( &vY, &vZ, &vX );
	D3DXVec3Normalize( &vY, &vY );

	float aspect = SCREEN_ASPECT_RATIO;
	float fov    = INITIAL_VIEWING_ANGLE;

	float nearPlaneHalfHeight = tanf( fov * 0.5f ) * nearClip;
	float nearPlaneHalfWidth  = nearPlaneHalfHeight * aspect;

	float farPlaneHalfHeight = tanf( fov * 0.5f ) * farClip;
	float farPlaneHalfWidth  = farPlaneHalfHeight * aspect;

	D3DXVECTOR3 nearPlaneCenter = CCameraManager::GetPosition() + vZ * nearClip;
	D3DXVECTOR3 farPlaneCenter  = CCameraManager::GetPosition() + vZ * farClip;;

	D3DXVECTOR3 corners[8];

	corners[0] = D3DXVECTOR3( nearPlaneCenter - vX * nearPlaneHalfWidth - vY * nearPlaneHalfHeight );
	corners[1] = D3DXVECTOR3( nearPlaneCenter - vX * nearPlaneHalfWidth + vY * nearPlaneHalfHeight );
	corners[2] = D3DXVECTOR3( nearPlaneCenter + vX * nearPlaneHalfWidth + vY * nearPlaneHalfHeight );
	corners[3] = D3DXVECTOR3( nearPlaneCenter + vX * nearPlaneHalfWidth - vY * nearPlaneHalfHeight );

	corners[4] = D3DXVECTOR3( farPlaneCenter - vX * farPlaneHalfWidth - vY * farPlaneHalfHeight );
	corners[5] = D3DXVECTOR3( farPlaneCenter - vX * farPlaneHalfWidth + vY * farPlaneHalfHeight );
	corners[6] = D3DXVECTOR3( farPlaneCenter + vX * farPlaneHalfWidth + vY * farPlaneHalfHeight );
	corners[7] = D3DXVECTOR3( farPlaneCenter + vX * farPlaneHalfWidth - vY * farPlaneHalfHeight );


	D3DXVECTOR3 point;
	D3DXVec3TransformCoord( &point, &corners[0], &viewProj );
	D3DXVECTOR3 mini = point;
	D3DXVECTOR3 maxi = point;
	for( int i=1; i<8; ++i )
	{
		D3DXVec3TransformCoord( &point, &corners[i], &viewProj );
		D3DXVec3Minimize( &mini, &point, &mini );
		D3DXVec3Maximize( &maxi, &point, &maxi );
	}

	return SBBox( mini, maxi );
}

// クロップ行列を求める.
D3DXMATRIX CLightBase::CreateCropMatrix( SBBox& box )
{
	/* ほぼ単位キューブクリッピングと同じ処理 */
	float scaleX  = 1.0f;
	float scaleY  = 1.0f;
	float scaleZ  = 1.0f;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float offsetZ = 0.0f;

	D3DXVECTOR3 mini = box.mini;
	D3DXVECTOR3 maxi = box.maxi;

	scaleX = 2.0f / ( maxi.x - mini.x );
	scaleY = 2.0f / ( maxi.y - mini.y );

	offsetX = -0.5f * ( maxi.x + mini.x ) * scaleX;
	offsetY = -0.5f * ( maxi.y + mini.y ) * scaleY;

	// 1.0を下回るとシャドウマップに移る部分が小さくなってしまうので，
	// 制限をかける.
	scaleX = max( 1.0f, scaleX );
	scaleY = max( 1.0f, scaleY );

	return D3DXMATRIX(
		scaleX,  0.0f,    0.0f,    0.0f,
		0.0f,    scaleY,  0.0f,    0.0f,
		0.0f,    0.0f,    scaleZ,  0.0f,
		offsetX, offsetY, offsetZ, 1.0f );
}