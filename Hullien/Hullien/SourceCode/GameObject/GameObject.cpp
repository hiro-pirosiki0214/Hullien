#include "GameObject.h"
#include "..\Camera\CameraManager\CameraManager.h"
#include "..\Common\SceneTexRenderer\SceneTexRenderer.h"

// ウィンドウ外の調整値.
static const float WND_OUT_ADJ_SIZE = 300.0f;

/****************************************
*	ゲームオブジェクト元クラス.
**/
CGameObject::CGameObject()
	: m_ObjectTag	( EObjectTag::None )
	, m_vPosition	( 0.0f, 0.0f, 0.0f )
	, m_vRotation	( 0.0f, 0.0f, 0.0f )
	, m_vSclae		( 1.0f, 1.0f, 1.0f )
{
}

CGameObject::~CGameObject()
{
}

//-----------------------------.
// オブジェクトタグ取得関数.
//-----------------------------.
EObjectTag CGameObject::GetObjectTag() const
{
	return m_ObjectTag;
}

//-----------------------------.
// 座標取得関数.
//-----------------------------.
D3DXVECTOR3 CGameObject::GetPosition() const
{
	return m_vPosition;
}

//-----------------------------.
// 座標設定関数.
//-----------------------------.
void CGameObject::SetPosition( const D3DXVECTOR3& vPos )
{
	m_vPosition = vPos;
}

//-----------------------------.
// 画面の外に出ているか.
//-----------------------------.
bool CGameObject::IsDisplayOut()
{
	// ビューポート行列.
	D3DXMATRIX mViewport;
	D3DXMatrixIdentity( &mViewport );	// 単位行列作成.
	mViewport._11 = static_cast<float>( WND_W ) *  0.5f;
	mViewport._22 = static_cast<float>( WND_H ) * -0.5f;
	mViewport._41 = static_cast<float>( WND_W ) *  0.5f;
	mViewport._42 = static_cast<float>( WND_H ) *  0.5f;

	// ワールド, ビュー, プロジェ, ビューポートの計算.
	D3DXMATRIX m = CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix() * mViewport;
	D3DXVECTOR3 screenPos = { 0.0f, 0.0f, 0.0f };	// スクリーン座標.
	// D3DXVECTOR3と掛け合わせ.
	D3DXVec3TransformCoord( &screenPos, &m_vPosition, &m );

	if( -WND_OUT_ADJ_SIZE < screenPos.x && screenPos.x < WND_W + WND_OUT_ADJ_SIZE &&
		-WND_OUT_ADJ_SIZE < screenPos.y && screenPos.y < WND_H + WND_OUT_ADJ_SIZE ) return false;	// 画面内.
	// 画面外でもシャドウは描画する.
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ) return false;
	
	return true;	// 画面外.
}