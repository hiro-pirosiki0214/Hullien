#include "DebugTextShader.h"

CDebugTextShader::CDebugTextShader()
{
}

CDebugTextShader::~CDebugTextShader()
{
}

//--------------------------------------.
// 初期化.
//--------------------------------------.
HRESULT CDebugTextShader::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if( FAILED( InitShader())) return E_FAIL;

	return S_OK;
}

//--------------------------------------.
// 解放.
//--------------------------------------.
HRESULT CDebugTextShader::Release()
{
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBuffer );

	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;

	return S_OK;
}

//--------------------------------------.
// コンスタントバッファにデータを渡す.
//--------------------------------------.
void CDebugTextShader::SetConstantBufferData( const D3DXMATRIX& mWVP, const D3DXVECTOR4& color )
{
	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// コンスタントバッファ.

	if( SUCCEEDED(
		m_pContext11->Map( m_pConstantBuffer,
			0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) ){

		cb.mWVP = mWVP;
		cb.vColor = color;
		cb.fAlpha.x = color.w;

		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );

		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}
}

//--------------------------------------.
// 各種シェーダの設定.
//--------------------------------------.
void CDebugTextShader::ShaderSet( ID3D11Buffer* pVertexBuffer, ID3D11SamplerState* pSampleLinear, ID3D11ShaderResourceView* pAsciiTexture )
{
	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.

	// このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ピクセルシェーダー.

	// 頂点バッファをセット.
	UINT stride = sizeof(VERTEX); // データの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &pVertexBuffer, &stride, &offset );
	m_pContext11->PSSetSamplers( 0, 1, &pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &pAsciiTexture );

	// 頂点インプットレイアウトをセット.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
}

//--------------------------------------.
// シェーダー作成.
//--------------------------------------.
HRESULT CDebugTextShader::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSLからバーテックスシェーダーのブロブを作成.
	if( FAILED( CShaderBase::InitShader(
			SHADER_NAME,
			"VS_Main",
			"vs_5_0",
			uCompileFlag,
			&pCompiledShader,
			&pErrors ))){
		_ASSERT_EXPR( false, L"hlsl読み込み失敗" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//　上記で作成したブロブからバーテックスシェーダーを作成.
	if( FAILED(
		CShaderBase::CreateVertexShader( pCompiledShader, &m_pVertexShader ))){
		_ASSERT_EXPR( false, L"ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ作成失敗" );
		return E_FAIL;
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		CShaderBase::GetPositionInputElement(),
		{
			"TEXCOORD",						//ﾃｸｽﾁｬ位置.
			0,
			DXGI_FORMAT_R32G32_FLOAT,		//DXGIのﾌｫｰﾏｯﾄ(32bit float型*2).
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,	//ﾃﾞｰﾀの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	// 頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// 頂点インプットレイアウトを作成.
	if( FAILED(
		CShaderBase::CreateInputLayout( 
			layout,
			numElements,
			pCompiledShader,
			&m_pVertexLayout ))){
		_ASSERT_EXPR( false, L"頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// HLSLからピクセルシェーダーのブロブを作成.
	if( FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,
			"PS_Main",
			"ps_5_0",
			uCompileFlag,
			&pCompiledShader,
			&pErrors ))){
		_ASSERT_EXPR( false, L"hlsl読み込み失敗" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	// 上記で作成したブロブからピクセルシェーダーを作成.
	if( FAILED(
		CShaderBase::CreatePixelShader( pCompiledShader, &m_pPixelShader ))){
		_ASSERT_EXPR( false, L"ﾋﾟｸｾﾙｼｪｰﾀﾞ作成失敗" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// コンスタント(定数)バッファ作成.
	if( FAILED( CShaderBase::CreateConstantBuffer( &m_pConstantBuffer, sizeof(C_BUFFER) ))){
		_ASSERT_EXPR( false, L"ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}

	return S_OK;
}