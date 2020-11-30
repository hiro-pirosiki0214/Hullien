#include "Common.h"

CCommon::CCommon()
	: m_pDevice11				( nullptr )
	, m_pContext11				( nullptr )
	, m_vPos					( 0.0f, 0.0f, 0.0f )
	, m_vRot					( 0.0f, 0.0f, 0.0f )
	, m_vScale					( 1.0f, 1.0f, 1.0f )
	, m_vColor					( 1.0f, 1.0f, 1.0f, 1.0f )
{
}

CCommon::~CCommon()
{
	m_pContext11	= nullptr;
	m_pDevice11		= nullptr;
}

//--------------------------------------------.
// �f�o�C�X�̎擾�A�e���̏�����.
//--------------------------------------------.
HRESULT CCommon::InitPram( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pDevice11		= pDevice11;
	m_pContext11	= pContext11;
	return S_OK;
}

//--------------------------------------------.
// �u�����h��L��:�����ɐݒ肷��.
//--------------------------------------------.
void CCommon::SetBlend( bool EnableAlpha )
{
	CDirectX11::SetBlend( EnableAlpha );
}

//--------------------------------------------.
// �A���t�@�J�o���[�W��L��:�����ɐݒ肷��.
//--------------------------------------------.
void CCommon::SetCoverage( bool EnableCoverage )
{
	CDirectX11::SetCoverage( EnableCoverage );
}

//--------------------------------------------.
// �[�x�e�X�g��L��:�����ɐݒ肷��.
//--------------------------------------------.
void CCommon::SetDeprh( bool flag )
{
	CDirectX11::SetDeprh( flag );
}

//--------------------------------------------.
// ���X�^���C�U�X�e�[�g�ݒ�.
//--------------------------------------------.
void CCommon::SetRasterizerState( const ERS_STATE& rsState )
{
	CDirectX11::SetRasterizerState( rsState );
}
