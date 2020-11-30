#include "Fog.h"

namespace
{
	static const float FOG_TEX_ADD_VALUE_X	= 0.0001f;	// �t�H�O�̃e�N�X�`�����Z�l.
	static const float FOG_TEX_ADD_VALUE_W	= 0.0003f;	// �t�H�O�̃e�N�X�`�����Z�l.
	static const float FOG_TEX_MAX			= 1.0f;		// �t�H�O�̃e�N�X�`�����W�ő�l.
	static const float FOG_TEX_SCALE		= 0.003f;	// �t�H�O�̃e�N�Z���X�P�[���l.
	static const D3DXVECTOR4 FOG_COLOR		= {0.0f,0.0f,0.0f,1.0f};	// �t�H�O�̐F.
	static const float FOG_MIN_HEIGHT		= 0.0f;		// �t�H�O�̍ŏ�����.
	static const float FOG_MAX_HEIGHT		= 6.0f;		// �t�H�O�̍ő卂��.
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

// �C���X�^���X�̎擾.
CFog* CFog::GetInstance()
{
	static std::unique_ptr<CFog> pInstance = std::make_unique<CFog>();
	return pInstance.get();
}

// �X�V.
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