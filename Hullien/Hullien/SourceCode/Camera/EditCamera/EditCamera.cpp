#include "EditCamera.h"
#include "..\..\Utility\Mouse\Mouse.h"

CEditCamera::CEditCamera()
	: m_hWnd	( nullptr )
	, m_pMouse	( nullptr )
	, m_vDegree	{ 0.0f, 0.0f }
{
	m_pMouse = std::make_unique<CMouse>();
}

CEditCamera::~CEditCamera()
{
}

// 更新関数.
void CEditCamera::Updata()
{
	D3DXMATRIX mRot;
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vDegree.x, 0.0f, 0.0f );
	// 軸ベクトルを用意.
	D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, 1.0f );
	// Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord( &vecAxisZ, &vecAxisZ, &mRot );

	
	if( GetAsyncKeyState('W') & 0x8000 ) m_vPosition += vecAxisZ;
	if( GetAsyncKeyState('S') & 0x8000 ) m_vPosition -= vecAxisZ;
	vecAxisZ = { 1.0f, 0.0f, 0.0f };
	// X軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord( &vecAxisZ, &vecAxisZ, &mRot );
	if( GetAsyncKeyState('D') & 0x8000 ) m_vPosition += vecAxisZ;
	if( GetAsyncKeyState('A') & 0x8000 ) m_vPosition -= vecAxisZ;

	if( GetAsyncKeyState('Q') & 0x8000 ) m_vPosition.y += 1.0f;
	if( GetAsyncKeyState('E') & 0x8000 ) m_vPosition.y -= 1.0f;

	MouseUpdate();

	// カメラ位置を算出.
	m_vLookPosition.x = m_vPosition.x + ( sinf(m_vDegree.x) * 15.0f );
	m_vLookPosition.y = m_vPosition.y + ( sinf(m_vDegree.y) * 15.0f );
	m_vLookPosition.z = m_vPosition.z + ( cosf(m_vDegree.x) * 15.0f );
}

void CEditCamera::MouseUpdate()
{
	m_pMouse->UpdateMouse( m_hWnd );
	// マウスが画面外なら終了.
	if( m_pMouse->IsScreenMiddle() == false ) return;
	if(!( GetAsyncKeyState(VK_LBUTTON) & 0x8000 )) return;

	float xsub = m_pMouse->GetPosisionX()-m_pMouse->GetOldPosisionX();
	float ysub = m_pMouse->GetPosisionY()-m_pMouse->GetOldPosisionY();

	const float movePow = 0.02f;

	if( xsub < 0.0f ) m_vDegree.x -= movePow;
	if( xsub > 0.0f ) m_vDegree.x += movePow;
	if( ysub > 0.0f ) m_vDegree.y -= movePow;
	if( ysub < 0.0f ) m_vDegree.y += movePow;
	if( m_vDegree.x > static_cast<float>(D3DXToRadian(360.0f)) )
		m_vDegree.x = static_cast<float>(D3DXToRadian(0.0f));
	if( m_vDegree.x < static_cast<float>(D3DXToRadian(0.0f)) ) 
		m_vDegree.x = static_cast<float>(D3DXToRadian(360.0f));
	if( m_vDegree.y > static_cast<float>(D3DXToRadian(170.0f)) )
		m_vDegree.y = static_cast<float>(D3DXToRadian(170.0f));
	if( m_vDegree.y < static_cast<float>(D3DXToRadian(-170.0f)) )
		m_vDegree.y = static_cast<float>(D3DXToRadian(-170.0f));
}