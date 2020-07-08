#include "SpriteShader.h"

CSpriteShader::CSpriteShader()
	: m_pVertexShaderUI(nullptr)
{
}

CSpriteShader::~CSpriteShader()
{
}

//-------------------.
// 初期化.
//-------------------.
HRESULT CSpriteShader::Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if (FAILED(InitShader())) return E_FAIL;

	return S_OK;
}

//-------------------.
// 解放.
//-------------------.
HRESULT CSpriteShader::Release()
{
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pVertexShaderUI);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pConstantBuffer);

	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;

	return S_OK;
}

//--------------------------------.
// コンスタントバッファにデータを渡す.
//--------------------------------.
void CSpriteShader::SetConstantBufferData(const D3DXMATRIX& mWVP, const float& fAlpha, const D3DXVECTOR2& texPos)
{
	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// コンスタントバッファ.

					// バッファ内のデータの書き換え開始時にMap.
	if (SUCCEEDED(m_pContext11->Map(
		m_pConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData))) {

		//ﾜｰﾙﾄﾞ行列を渡す.
		cb.mWVP = mWVP;
		cb.mW = mWVP;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);//行列を転置する.
		D3DXMatrixTranspose(&cb.mW, &cb.mW);//行列を転置する.
											  // ビューポートの幅,高さを渡す.
		cb.vUV_ViewPort.z = static_cast<float>(WND_W);
		cb.vUV_ViewPort.w = static_cast<float>(WND_H);

		// アルファ値を渡す.
		cb.vColor.w = fAlpha;

		// テクスチャ座標.
		cb.vUV_ViewPort.x = texPos.x;
		cb.vUV_ViewPort.y = texPos.y;

		memcpy_s(
			pData.pData,
			pData.RowPitch,
			(void*)(&cb),
			sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}
}

//--------------------------------.
// 各種シェーダの設定.
//--------------------------------.
void CSpriteShader::ShaderSet(ID3D11Buffer* pVertexBuffer)
{
	// 使用するシェーダのセット.
	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);	// 頂点シェーダ.
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);	// ピクセルシェーダ.

																// このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);	// ピクセルシェーダー.

																	// 頂点バッファをセット.
	UINT stride = sizeof(VERTEX); // データの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// 頂点インプットレイアウトをセット.
	m_pContext11->IASetInputLayout(m_pVertexLayout);
}

//--------------------------------.
// 各種UI用のシェーダの設定.
//--------------------------------.
void CSpriteShader::ShaderUISet(ID3D11Buffer* pVertexBuffer)
{
	// 使用するシェーダのセット.
	m_pContext11->VSSetShader(m_pVertexShaderUI, nullptr, 0);	// 頂点シェーダ.
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);	// ピクセルシェーダ.

																// このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);	// ピクセルシェーダー.

																	// 頂点バッファをセット.
	UINT stride = sizeof(VERTEX); // データの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// 頂点インプットレイアウトをセット.
	m_pContext11->IASetInputLayout(m_pVertexLayout);
}

//--------------------------------.
// シェーダー作成.
//--------------------------------.
HRESULT CSpriteShader::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSLからバーテックスシェーダーのブロブを作成.
	if (FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// シェーダーファイル名.
			"VS_Main",			// シェーダーエントリーポイント.
			"vs_5_0",			// シェーダーモデル.
			uCompileFlag,		// シェーダーコンパイルフラグ.
			&pCompiledShader,	// ブロブを格納するメモリへのポインタ.
			&pErrors))) {		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE("hlsl read failure");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// 上記で作成したﾌﾞﾛﾌﾞから「ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ」を作成.
	if (FAILED(CShaderBase::CreateVertexShader(pCompiledShader, &m_pVertexShader))) {
		ERROR_MESSAGE("VertexShader creation failed");
		return E_FAIL;
	}



	// HLSLからバーテックスシェーダーのブロブを作成.
	if (FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// シェーダーファイル名.
			"VS_MainUI",		// シェーダーエントリーポイント.
			"vs_5_0",			// シェーダーモデル.
			uCompileFlag,		// シェーダーコンパイルフラグ.
			&pCompiledShader,	// ブロブを格納するメモリへのポインタ.
			&pErrors))) {		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE("hlsl read failure");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// 上記で作成したﾌﾞﾛﾌﾞから「ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ」を作成.
	if (FAILED(CShaderBase::CreateVertexShader(pCompiledShader, &m_pVertexShaderUI))) {
		ERROR_MESSAGE("VertexShader creation failed");
		return E_FAIL;
	}



	// 頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		CShaderBase::GetPositionInputElement(),
		CShaderBase::GetTexcoordInputElement(),
	};

	// 頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// 頂点インプットレイアウトを作成.
	if (FAILED(
		CShaderBase::CreateInputLayout(
			layout,
			numElements,
			pCompiledShader,
			&m_pVertexLayout))) {
		ERROR_MESSAGE("Vertex input layout creation failed");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);



	//HLSLからﾋﾟｸｾﾙｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// シェーダーファイル名.
			"PS_Main",			// シェーダーエントリーポイント.
			"ps_5_0",			// シェーダーモデル.
			uCompileFlag,		// シェーダーコンパイルフラグ.
			&pCompiledShader,	// ブロブを格納するメモリへのポインタ.
			&pErrors))) {		// エラーと警告一覧を格納するメモリへのポインタ.
		ERROR_MESSAGE("hlsl read failure");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾋﾟｸｾﾙｼｪｰﾀﾞ」を作成.
	if (FAILED(CShaderBase::CreatePixelShader(pCompiledShader, &m_pPixelShader))) {
		ERROR_MESSAGE("VertexShader creation failed");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	// シェーダーに特定の数値を送るバッファ.
	// コンスタントバッファの作成.
	if (FAILED(
		CShaderBase::CreateConstantBuffer(&m_pConstantBuffer, sizeof(C_BUFFER)))) {
		ERROR_MESSAGE("Instant buffer creation failed");
		return E_FAIL;
	}

	return S_OK;
}