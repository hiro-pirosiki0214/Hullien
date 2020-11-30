#include "DebugText.h"
#include "..\Font\Font.h"

CDebugText::CDebugText()
	: m_pFont			( nullptr )
{
	m_pFont = std::make_unique<CFont>();
}

CDebugText::~CDebugText()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CDebugText* CDebugText::GetInstance()
{
	static std::unique_ptr<CDebugText> pInstance =
		std::make_unique<CDebugText>();	// インスタンスの作成.
	return pInstance.get();
}

//----------------------------.
// 初期化関数.
//----------------------------.
HRESULT CDebugText::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	float Scale, 
	D3DXVECTOR4 vColor )
{
	GetInstance()->m_vScale		= { Scale, Scale, Scale };
	GetInstance()->m_vColor		= vColor;
	GetInstance()->InitPram( pDevice11, pContext11 );
	if( FAILED( GetInstance()->m_pFont->Init( pDevice11, pContext11 ) )) return E_FAIL;

	return S_OK;
}

//----------------------------.
// 描画関数.
//----------------------------.
void CDebugText::Render( const std::string& stext )
{
	GetInstance()->m_pFont->SetPosition( m_vPos );
	GetInstance()->m_pFont->SetColor( m_vColor );
	GetInstance()->m_pFont->SetScale( m_vScale.x );
	GetInstance()->SetDeprh( false );
	GetInstance()->m_pFont->RenderUI( stext );
	GetInstance()->SetDeprh( true );
}