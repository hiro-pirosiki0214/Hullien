#include "Toon.h"

CToon::CToon()
	: m_pDevice11		( nullptr )
	, m_pVertexShader	( nullptr )
	, m_pVertexLayout	( nullptr )
	, m_pPixelShader	( nullptr )
	, m_pToonTexture	( nullptr )
{
}

CToon::~CToon()
{
}

HRESULT CToon::InitShader()
{
	

	return S_OK;
}

HRESULT CToon::InitVertexShader()
{
//	ID3DBlob* pCompiledShader = nullptr;
//	ID3DBlob* pErrors = nullptr;
//	UINT uCompileFlag = 0;
//#ifdef _DEBUG
//	uCompileFlag =
//		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
//#endif	// #ifdef _DEBUG
//
//	//-------------------------------------.
//	//	バーテックスシェーダーの作成.
//	//-------------------------------------.
//	// ブロブの作成.
//	if( FAILED( CCommon::InitShader( 
//		SHADER_FIEL_PATH, 
//		SHADER_ENTRY_POINT,
//		SHADER_MODEL,
//		uCompileFlag, 
//		&pCompiledShader,
//		&pErrors )) ){
//
//		ERROR_MESSAGE( "hlsl読込失敗" );
//		return E_FAIL;
//	}
//	// シェーダーの作成.
//	if( FAILED( m_pDevice11->CreateVertexShader(
//		pCompiledShader->GetBufferPointer(),
//		pCompiledShader->GetBufferSize(),
//		nullptr,
//		&m_pVertexShader ))){
//
//		ERROR_MESSAGE( "バーテックスシェーダー作成失敗" );
//		return E_FAIL;
//	}
//	InitVertexLayout( pCompiledShader );
//
	return S_OK;
}

HRESULT CToon::InitVertexLayout( ID3DBlob* pCompiledShader )
{
	// 頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	// 頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの配列要素数を算出.
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	numElements = sizeof(tmp) / sizeof(tmp[0]);	// 要素数算出.
	memcpy_s( layout, sizeof(layout),
		tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);


	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ.
	{
		_ASSERT_EXPR(false, L"頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗");
		return E_FAIL;
	}

	return S_OK;
}