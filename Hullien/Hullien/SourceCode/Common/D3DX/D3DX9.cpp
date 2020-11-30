#include "D3DX9.h"

CDirectX9::CDirectX9()
	: m_hWnd		( nullptr )
	, m_pDevice9	( nullptr )
{
}

CDirectX9::~CDirectX9()
{
}

//-----------------------------------.
// �C���X�^���X�̎擾.
//-----------------------------------.
CDirectX9* CDirectX9::GetInstance()
{
	static std::unique_ptr<CDirectX9> pInstance = 
		std::make_unique<CDirectX9>();
	return pInstance.get();
}

//-----------------------------------.
// DirectX9�̍\�z.
//-----------------------------------.
HRESULT CDirectX9::Create( HWND hWnd )
{
	GetInstance()->m_hWnd = hWnd;

	if( FAILED( GetInstance()->CreateDevice9() )) return E_FAIL;

	return S_OK;
}

//-----------------------------------.
// DirectX9�̉��.
//-----------------------------------.
HRESULT CDirectX9::Release()
{
	SAFE_RELEASE( GetInstance()->m_pDevice9 );
	GetInstance()->m_hWnd = nullptr;

	return S_OK;
}

//-----------------------------------.
// �f�o�C�X9�̍쐬.
//-----------------------------------.
HRESULT CDirectX9::CreateDevice9()
{
	LPDIRECT3D9 m_pD3d9 = nullptr; // Dx9�I�u�W�F�N�g.

	//�uDirect3D�v�I�u�W�F�N�g�̍쐬.
	m_pD3d9 = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pD3d9 == nullptr ){
		ERROR_MESSAGE("Dx9�I�u�W�F�N�g�쐬���s");
		return E_FAIL;
	}

	// Diret3D �f�o�C�X�I�u�W�F�N�g�̍쐬.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g.
	d3dpp.BackBufferCount = 1;					// �o�b�N�o�b�t�@�̐�.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �X���b�v�G�t�F�N�g.
	d3dpp.Windowed = true;						// �E�B���h�E���[�h.
	d3dpp.EnableAutoDepthStencil = true;		// �X�e���V���L��.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// �X�e���V���̃t�H�[�}�b�g(16bit).

	auto createDevice = [&]( const D3DDEVTYPE& deviceType, const DWORD& behaviorFlags )
	{
		return m_pD3d9->CreateDevice(
			D3DADAPTER_DEFAULT, 
			deviceType, 
			m_hWnd,
			behaviorFlags,
			&d3dpp, 
			&m_pDevice9 );
	};

	// �f�o�C�X�쐬
	// (HALӰ��:�`��ƒ��_������GPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	// (HALӰ��:�`���GPU�A���_������CPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}
	MessageBox( nullptr, "HALӰ�ނ����޲��쐬�ł��܂���\nREFӰ�ނōĎ��s���܂�", "�x��", MB_OK );

	// (REFӰ��:�`���CPU�A���_������GPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_REF, D3DCREATE_HARDWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	// (REFӰ��:�`��ƒ��_������CPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	ERROR_MESSAGE("Direct3�f�o�C�X�쐬���s");

	SAFE_RELEASE( m_pD3d9 );
	return E_FAIL;
}