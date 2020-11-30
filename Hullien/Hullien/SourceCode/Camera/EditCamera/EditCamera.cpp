#include "EditCamera.h"
#include "..\..\Utility\Mouse\Mouse.h"
#include "..\..\Utility\XInput\XInput.h"

namespace
{
	const float CAMERA_MOVE_SPEED				= 0.1f;		// �J�����̈ړ����x.
	const float CAMERA_POS_CAMERA_LOOK_DISTANCE = 15.0f;	// �J�����Ǝ��_�̋���.
	const float MOUSE_MOVE_SPEED				= 0.02f;	// �}�E�X�̈ړ����x.
	const float RADIAN_THRESHOLD_X_MAX			= 360.0f;	// ���W�A��X�̍ő債�����l.
	const float RADIAN_THRESHOLD_X_MIN			= 0.0f;		// ���W�A��X�̍ŏ��������l.
	const float RADIAN_THRESHOLD_Y_MAX			= 90.0f;	// ���W�A��Y�̍ő債�����l.
	const float RADIAN_THRESHOLD_Y_MIN			= -90.0f;	// ���W�A��Y�̍ŏ��������l.
	const D3DXVECTOR3	INIT_POSITION			= { 0.0f, 10.0f, -30.0f };	// �������W.
}

CEditCamera::CEditCamera()
	: m_hWnd	( nullptr )
	, m_pMouse	( nullptr )
	, m_Radian	( 0.0f, 0.0f )
{
	m_pMouse = std::make_unique<CMouse>();
	m_vPosition = INIT_POSITION;
}

CEditCamera::~CEditCamera()
{
}

// �X�V�֐�.
void CEditCamera::Updata()
{
	D3DXMATRIX mRot;
	D3DXMatrixRotationYawPitchRoll( &mRot, m_Radian.x, 0.0f, 0.0f );
	// ���x�N�g����p��.
	D3DXVECTOR3 vecAxisX( CAMERA_MOVE_SPEED, 0.0f, 0.0f );	// X��.
	D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, CAMERA_MOVE_SPEED );	// Z��.
	// X���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisX, &vecAxisX, &mRot );
	// Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisZ, &vecAxisZ, &mRot );

	// �O�i.
	if( GetAsyncKeyState('W') & 0x8000 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX )
		m_vPosition += vecAxisZ;
	// ���.
	if( GetAsyncKeyState('S') & 0x8000 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN )
		m_vPosition -= vecAxisZ;
	// �E�Ɉړ�.
	if( GetAsyncKeyState('D') & 0x8000 || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX )
		m_vPosition += vecAxisX;
	// ���Ɉړ�.
	if( GetAsyncKeyState('A') & 0x8000 || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN )
		m_vPosition -= vecAxisX;
	// �㏸.
	if( GetAsyncKeyState('Q') & 0x8000 || CXInput::RTrigger() > IDLE_TIGGER_MAX ) 
		m_vPosition.y += CAMERA_MOVE_SPEED;
	// ���~.
	if( GetAsyncKeyState('E') & 0x8000 || CXInput::LTrigger() > IDLE_TIGGER_MAX )
		m_vPosition.y -= CAMERA_MOVE_SPEED;

	MouseUpdate();

	// �J�����ʒu���Z�o.
	m_vLookPosition.x = m_vPosition.x + ( sinf(m_Radian.x) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
	m_vLookPosition.y = m_vPosition.y + ( sinf(m_Radian.y) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
	m_vLookPosition.z = m_vPosition.z + ( cosf(m_Radian.x) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
}

// �}�E�X�̍X�V.
void CEditCamera::MouseUpdate()
{
	m_pMouse->UpdateMouse( m_hWnd );	// �}�E�X�̍X�V.
	float xSub = 0.0f;
	float ySub = 0.0f;
	float moveSpeed = MOUSE_MOVE_SPEED;
	if( CXInput::RThumbY_Axis() > IDLE_THUMB_MAX ) ySub = -static_cast<float>(CXInput::RThumbY_Axis());
	if( CXInput::RThumbY_Axis() < IDLE_THUMB_MIN ) ySub = -static_cast<float>(CXInput::RThumbY_Axis());
	if( CXInput::RThumbX_Axis() > IDLE_THUMB_MAX ) xSub = static_cast<float>(CXInput::RThumbX_Axis());
	if( CXInput::RThumbX_Axis() < IDLE_THUMB_MIN ) xSub = static_cast<float>(CXInput::RThumbX_Axis());
	if(!( GetAsyncKeyState(VK_LBUTTON) & 0x8000 )){
		moveSpeed *= 0.5f;	// �ړ����x�𔼕��ɂ���.
	} else {
		// �}�E�X����ʊO�Ȃ�I��.
		if( m_pMouse->IsScreenMiddle() == false ) return;
		// �}�E�X�̌��݂̍��W�Ɖߋ��̍��W���������l���Z�o.
		if( ySub == 0.0f )
			ySub = m_pMouse->GetPosisionY() - m_pMouse->GetOldPosisionY();
		if( xSub == 0.0f )
			xSub = m_pMouse->GetPosisionX() - m_pMouse->GetOldPosisionX();
	}


	if( xSub < 0.0f ) m_Radian.x -= moveSpeed;
	if( xSub > 0.0f ) m_Radian.x += moveSpeed;
	if( ySub > 0.0f ) m_Radian.y -= moveSpeed;
	if( ySub < 0.0f ) m_Radian.y += moveSpeed;
	// �K��l�𒴂��Ȃ��悤����.
	if( m_Radian.x > static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_X_MAX)) )
		m_Radian.x = static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_X_MIN));
	if( m_Radian.x < static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_X_MIN)) ) 
		m_Radian.x = static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_X_MAX));
	if( m_Radian.y > static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_Y_MAX)) )
		m_Radian.y = static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_Y_MAX));
	if( m_Radian.y < static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_Y_MIN)) )
		m_Radian.y = static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_Y_MIN));
}