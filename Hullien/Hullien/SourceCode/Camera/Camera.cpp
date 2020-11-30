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
// �J�����̍X�V����.
//--------------------------------------------.
void CCamera::Update()
{
}

//--------------------------------------------.
// �r���[�s��A�v���W�F�N�V�����s��̏�����.
//--------------------------------------------.
void CCamera::InitViewProj()
{
	// �r���[(�J����)�ϊ�.
	D3DXMatrixLookAtLH( 
		&m_ViewMatrix,		// (out)viewMarix.
		&m_vPosition,		// �J�������W.
		&m_vLookPosition,	// �J�����������W.
		&VIEW_UP_VECTOR );	// �J�����x�N�g��.

#if 0
	// ����p�̕ύX.
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

	// �v���W�F�N�V����(�ˉe)�ϊ�.
	D3DXMatrixPerspectiveFovLH(
		&m_ProjMatrix,				// (out)ProjMatrix.
		m_ViewingAngle,				// ����p.
		SCREEN_ASPECT_RATIO,		// ��ʃA�X�y�N�g��.
		m_MinClippingDistance,		// �ŏ��`�拗��.
		m_MaxClippingDistance );	// �ő�`�拗��.
}

//--------------------------------------------.
// �p����̃J�������̐ݒ�֐�.
//--------------------------------------------.
void CCamera::SetCamera( std::shared_ptr<CCamera> camera )
{
	this->m_vPosition		= camera->GetPosition();
	this->m_vLookPosition	= camera->GetLookPosition();
}