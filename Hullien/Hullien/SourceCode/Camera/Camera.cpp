#include "Camera.h"

CCamera::CCamera()
	: m_ViewMatrix			()
	, m_ProjMatrix			()
	, m_vPosition			()
	, m_vLookPosition		()
	, m_ViewingAngle		( INITIAL_VIEWING_ANGLE )
	, m_MaxClippingDistance	( INITIAL_MAX_CLIPPING_DISTANCE )
	, m_MinClippingDistance	( INITIAL_MIN_CLIPPING_DISTANCE )
{
}

CCamera::~CCamera()
{
}

//--------------------------------------------.
// カメラの更新処理.
//--------------------------------------------.
void CCamera::Update()
{
}

//--------------------------------------------.
// ビュー行列、プロジェクション行列の初期化.
//--------------------------------------------.
void CCamera::InitViewProj()
{
	// ビュー(カメラ)変換.
	D3DXMatrixLookAtLH( 
		&m_ViewMatrix,		// (out)viewMarix.
		&m_vPosition,		// カメラ座標.
		&m_vLookPosition,	// カメラ注視座標.
		&VIEW_UP_VECTOR );	// カメラベクトル.

	// プロジェクション(射影)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_ProjMatrix,				// (out)ProjMatrix.
		m_ViewingAngle,				// 視野角.
		SCREEN_ASPECT_RATIO,		// 画面アスペクト比.
		m_MinClippingDistance,		// 最小描画距離.
		m_MaxClippingDistance );	// 最大描画距離.
}

//--------------------------------------------.
// 継承先のカメラ情報の設定関数.
//--------------------------------------------.
void CCamera::SetCamera( std::shared_ptr<CCamera> camera )
{
	this->m_vPosition		= camera->GetPosition();
	this->m_vLookPosition	= camera->GetLookPosition();
}