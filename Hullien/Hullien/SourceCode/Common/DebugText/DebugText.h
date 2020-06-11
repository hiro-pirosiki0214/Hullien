#ifndef TEXT_SPRITE_H
#define TEXT_SPRITE_H

#include <string>
#include <sstream>

#include "..\Common.h"

class CDebugTextShader;

// デバッグテキストクラス.
class CDebugText : public CCommon
{
private:
	const float DIMENSION		= 12.0f;
	const float WDIMENSION		= 10.0f;
	const float TEX_DIMENSION	= 128.0f;

public:
	CDebugText();	// コンストラクタ
	~CDebugText();	// デストラクタ.

	// インスタンスの取得.
	static CDebugText* GetInstance();

	// 初期化関数.
	static HRESULT Init( 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		float Scale, 
		D3DXVECTOR4 vColor );
	// 座標の設定.
	static void SetPosition( const D3DXVECTOR3& vPos ){ GetInstance()->m_vPos = vPos; }

	// 描画関数(可変引数).
	template<class... T1>
	static void Render( const T1&... t1 )
	{
		std::string Text = GetInstance()->initstring(t1...);
		GetInstance()->Render( Text );
	}
	// 描画関数.
	void Render( const std::string& stext );
	
	// サイズの取得.
	static float GetScale(){ return GetInstance()->m_vScale.x*10.0f; };
private:
	// フォントレンダリング関数.
	void RenderFont( int FontIndex, float x, float y );
	// モデルの作成.
	HRESULT InitModel();
	// サンプラの作成.
	HRESULT InitSampleLinear();
	// テクスチャ作成.
	HRESULT InitTexture();

	template <class T>
	std::string to_std_string(const T& x)
	{
		std::stringstream ss;
		ss << x;
		return ss.str();
	}
	template<class T1, class... T2>
	std::string initstring( const T1& t1, const T2&... t2 )
	{
		std::string s = to_std_string(t1);
		return s + initstring(t2...);
	}
	template<class T1>
	std::string initstring( const T1& t1 )
	{
		std::string s = to_std_string(t1);
		return s;
	}

private:
	ID3D11Buffer*				m_pVertexBuffer[100];	// 頂点バッファ.
	ID3D11SamplerState*			m_pSampleLinear;		// サンプラ:テクスチャに各種フィルタをかける.
	ID3D11ShaderResourceView*	m_pAsciiTexture;		// テクスチャ.

	std::unique_ptr<CDebugTextShader> m_pShader;		// シェーダー.

	float		m_fKerning[100];	// カーニング.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDebugText( const CDebugText & )				= delete;
	CDebugText& operator = ( const CDebugText & )	= delete;
	CDebugText( CDebugText && )						= delete;
	CDebugText& operator = ( CDebugText && )		= delete;
};

#endif // #ifndef CDEBUG_TEXT_H.