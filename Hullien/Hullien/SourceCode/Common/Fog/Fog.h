#ifndef FOG_H
#define FOG_H

#include "..\..\Global.h"

class CFog
{
public:
	CFog();
	~CFog();

	static CFog* GetInstance()
	{
		static std::unique_ptr<CFog> pInstance = std::make_unique<CFog>();
		return pInstance.get();
	}

	// 更新.
	static void Update();
	// フォグのテクスチャ座標取得.
	static D3DXVECTOR4 GetFogTex() { return GetInstance()->m_FogTex; }

private:
	D3DXVECTOR4 m_FogTex;	// フォグのテクスチャ座標.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CFog( const CFog & )				= delete;
	CFog( CFog && )						= delete;
	CFog& operator = ( const CFog & )	= delete;
	CFog& operator = ( CFog && )		= delete;
};

#endif	// #ifndef FOG_H.