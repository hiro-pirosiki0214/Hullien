#include "D3DX9.h"

LPDIRECT3DDEVICE9 CDirectX9::m_pDevice9 = nullptr;

CDirectX9::CDirectX9()
	: m_hWnd	( nullptr )
{
}

CDirectX9::~CDirectX9()
{
}

//-----------------------------------.
// DirectX9の構築.
//-----------------------------------.
HRESULT CDirectX9::Create( HWND hWnd )
{
	m_hWnd = hWnd;

	if( FAILED( CreateDevice9() )) return E_FAIL;

	return S_OK;
}

//-----------------------------------.
// DirectX9の解放.
//-----------------------------------.
void CDirectX9::Release()
{
	SAFE_RELEASE( m_pDevice9 );
	m_hWnd = nullptr;
}

//-----------------------------------.
// デバイス9の作成.
//-----------------------------------.
HRESULT CDirectX9::CreateDevice9()
{
	LPDIRECT3D9 m_pD3d9 = nullptr; // Dx9オブジェクト.

	//「Direct3D」オブジェクトの作成.
	m_pD3d9 = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pD3d9 == nullptr ){
		_ASSERT_EXPR( false, L"Dx9オブジェクト作成失敗" );
		MessageBox( nullptr, "Dx9オブジェクト作成失敗", "警告", MB_OK );
		return E_FAIL;
	}

	// Diret3D デバイスオブジェクトの作成.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	// バックバッファのフォーマット.
	d3dpp.BackBufferCount = 1;					// バックバッファの数.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// スワップエフェクト.
	d3dpp.Windowed = true;						// ウィンドウモード.
	d3dpp.EnableAutoDepthStencil = true;		// ステンシル有効.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// ステンシルのフォーマット(16bit).

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

	// デバイス作成
	// (HALﾓｰﾄﾞ:描画と頂点処理をGPUで行う).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	// (HALﾓｰﾄﾞ:描画はGPU、頂点処理をCPUで行う).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}
	MessageBox( nullptr, "HALﾓｰﾄﾞでﾃﾞﾊﾞｲｽ作成できません\nREFﾓｰﾄﾞで再試行します", "警告", MB_OK );

	// (REFﾓｰﾄﾞ:描画はCPU、頂点処理をGPUで行う).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_REF, D3DCREATE_HARDWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	// (REFﾓｰﾄﾞ:描画と頂点処理をCPUで行う).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	MessageBox( nullptr, "Direct3Dﾃﾞﾊﾞｲｽ作成失敗", "警告", MB_OK );

	SAFE_RELEASE( m_pD3d9 );
	return E_FAIL;
}