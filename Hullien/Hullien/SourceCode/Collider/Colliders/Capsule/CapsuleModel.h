#ifndef CAPSULE_MODEL_H
#define CAPSULE_MODEL_H

#include "..\..\..\Common\Common.h"

class CSimpleShader;

class CCapsuleModel : public CCommon
{
private: //------- 定数 -------.
	const D3DXVECTOR4 MODEL_COLOR			= { 0.0f, 0.0f, 1.0f, 1.0f };	// モデルの色.
	const D3DXVECTOR4 MODEL_CHANGE_COLOR	= { 1.0f, 0.0f, 0.0f, 1.0f };	// モデルの色.

public: //------- 関数 -------.
	CCapsuleModel();
	virtual ~CCapsuleModel();

	// 初期化.
	HRESULT Init( const float& fRadius, const float& fHeight );
	// 描画.
	void Render();
	// 色を変える.
	void ChangeColor( const bool& changed ){ m_IsColorChanged = changed; }

private: //------- 関数 -------.
		 // WVP Matrixの作成.
	D3DXMATRIX CreateWVPMatrix( D3DXMATRIX& mWVP );
	// モデルの作成.
	HRESULT InitModel( const float& fRadius, const float& fHeight );

private: //------- 変数 -------.
	std::unique_ptr<CSimpleShader>	m_pSimpleShader;	// シンプルシェーダ.
	ID3D11Buffer*					m_pVertexBuffer;	// 頂点バッファ.
	ID3D11Buffer*					m_pIndexBuffer;		// インデックスバッファ.

	bool m_IsColorChanged;	// 色が変わったかどうか.

	int index_num;
};

#endif	// #ifndef CAPSULE_MODEL_H.