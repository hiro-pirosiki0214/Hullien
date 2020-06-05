#include "LightBase.h"

CLightBase::CLightBase()
	: m_vPosition	( 0.0f, 0.0f, 0.0f )
	, m_vDirection	( 0.0f, 0.0f, 0.0f )
	, m_mRotation	()
	, m_fIntensity	( 0.0f )
{
}

CLightBase::~CLightBase()
{
}	