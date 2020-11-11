#include "Fog.h"

namespace
{
	static const float FOG_TEX_ADD_VALUE_X	= 0.0001f;	// フォグのテクスチャ加算値.
	static const float FOG_TEX_ADD_VALUE_W	= 0.0003f;	// フォグのテクスチャ加算値.
	static const float FOG_TEX_MAX			= 1.0f;		// フォグのテクスチャ座標最大値.
	static const float FOG_TEX_SCALE		= 0.003f;	// フォグのテクセルスケール値.
	static const D3DXVECTOR4 FOG_COLOR		= {0.6f,0.6f,0.6f,1.0f};	// フォグの色.
	static const float FOG_MIN_HEIGHT		= 0.0f;		// フォグの最小高さ.
	static const float FOG_MAX_HEIGHT		= 6.0f;	// フォグの最大高さ.
}

CFog::CFog()
	: m_Fog	()
{
	m_Fog.Color	= FOG_COLOR;
	m_Fog.State.MinHeight	= FOG_MIN_HEIGHT;
	m_Fog.State.MaxHeight	= FOG_MAX_HEIGHT;
	m_Fog.State.TexScale	= FOG_TEX_SCALE;
}

CFog::~CFog()
{
}

// インスタンスの取得.
CFog* CFog::GetInstance()
{
	static std::unique_ptr<CFog> pInstance = std::make_unique<CFog>();
	return pInstance.get();
}

// 更新.
void CFog::Update()
{
	GetInstance()->m_Fog.Tex.x += FOG_TEX_ADD_VALUE_X;
	if( GetInstance()->m_Fog.Tex.x >= FOG_TEX_MAX ){
		GetInstance()->m_Fog.Tex.x = 0.0f;
	}
	GetInstance()->m_Fog.Tex.w += FOG_TEX_ADD_VALUE_W;
	if( GetInstance()->m_Fog.Tex.w >= FOG_TEX_MAX ){
		GetInstance()->m_Fog.Tex.w = 0.0f;
	}
}