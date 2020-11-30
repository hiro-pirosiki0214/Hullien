#include "Camera.h"

CCamera::CCamera()
	: m_ViewMatrix			()
	, m_ProjMatrix			()
	, m_vPosition			( 0.0f, 10.0f, -5.0f )
	, m_vLookPosition		( 0.0f, 0.0f, 0.0f )
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

#if 0
	// 視野角の変更.
	static float p = 4.0f;
	if( GetAsyncKeyState('V') & 0x8000 ) p -= 0.01f;
	if( GetAsyncKeyState('B') & 0x8000 ) p += 0.01f;
//	m_ViewingAngle = static_cast<float>(D3DX_PI/p);
	//{
	//	static float w = FLOAT_WND_W;
	//	static float h = FLOAT_WND_H;
	//	if( GetAsyncKeyState('V') & 0x8000 ) w -= 1.0f;
	//	if( GetAsyncKeyState('B') & 0x8000 ) w += 1.0f;
	//	if( GetAsyncKeyState('N') & 0x8000 ) h -= 1.0f;
	//	if( GetAsyncKeyState('M') & 0x8000 ) h += 1.0f;
	//	const float aspect = w/h;
	//}
#endif

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