#ifndef FOG_H
#define FOG_H

#include "..\..\Global.h"

//------------------------------.
// フォグの構造体.
//------------------------------.
// フォグの情報 : 16byteにするために調整変数追加.
struct stFogState
{
	float MinHeight;	// フォグの最小高さ.
	float MaxHeight;	// フォグの最大高さ.
	float TexScale;		// フォグのテクセルスケール値.
	stFogState()
		: MinHeight	( 0.0f )
		, MaxHeight ( 10.0f )
		, TexScale	( 0.03f )
	{}
private:
	float MemoryAdj;	// メモリ調整用 16byteに設定するため.
} typedef SFogState;

// フォグの情報 : コンスタントバッファに渡すよう.
struct stFog
{
	D3DXVECTOR4 Tex;	// フォグのテクスチャ座標.
	D3DXVECTOR4 Color;	// フォグの色.
	SFogState	State;	// フォグのステータス.

	stFog()
		: Tex	{ 0.0f, 0.0f, 0.5f, 0.5f }
		, Color	{ 1.0f, 1.0f, 1.0f, 1.0f }
		, State	()
	{}
} typedef SFog;

class CFog
{
public:
	CFog();
	~CFog();

	// インスタンスの取得.
	static CFog* GetInstance();

	// 更新.
	static void Update();
	// フォグのテクスチャ座標取得.
	static SFog GetFog() { return GetInstance()->m_Fog; }

private:
	SFog m_Fog;	// フォグのテクスチャ座標.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CFog( const CFog & )				= delete;
	CFog( CFog && )						= delete;
	CFog& operator = ( const CFog & )	= delete;
	CFog& operator = ( CFog && )		= delete;
};

#endif	// #ifndef FOG_H.