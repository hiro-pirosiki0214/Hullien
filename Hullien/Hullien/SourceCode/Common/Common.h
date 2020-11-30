#ifndef COMMON_H
#define COMMON_H

#include "..\Global.h"
#include "D3DX/D3DX11.h"

class CCommon
{
public:
	CCommon();
	virtual ~CCommon();

	// 座標の設定.
	void SetPosition( const D3DXVECTOR3& vPos ){ m_vPos = vPos; }
	// 回転情報の設定.
	void SetRotation( const D3DXVECTOR3& vRot ){ m_vRot = vRot; }
	// サイズの設定.
	void SetScale( const D3DXVECTOR3& vScale ){ m_vScale = vScale; }
	void SetScale( const float& fScale ){ m_vScale = { fScale, fScale, fScale }; }
	// 色の設定.
	void SetColor( const D3DXVECTOR4& vColor ){ m_vColor = vColor; }
	// アルファ値の設定.
	void SetAlpha( const float& fAlpha ){ m_vColor.w = fAlpha; }

	// ブレンドを有効:無効に設定する.
	void SetBlend( bool EnableAlpha );
	// アルファカバレージを有効:無効に設定する.
	void SetCoverage( bool EnableCoverage );
	// 深度テストを有効:無効に設定する.
	void SetDeprh( bool flag );
	// ラスタライザステート設定.
	void SetRasterizerState( const ERS_STATE& rsState );

protected:
	// デバイスの取得、各情報の初期化.
	HRESULT InitPram( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

protected:
	ID3D11Device*			m_pDevice11;	// デバイスオブジェクト.
	ID3D11DeviceContext*	m_pContext11;	// デバイスコンテキスト.

	D3DXVECTOR3 m_vPos;		// 座標情報.
	D3DXVECTOR3 m_vRot;		// 回転情報.
	D3DXVECTOR3	m_vScale;	// スケール情報.
	D3DXVECTOR4 m_vColor;	// 色.
};

#endif	// #ifndef COMMON_H.