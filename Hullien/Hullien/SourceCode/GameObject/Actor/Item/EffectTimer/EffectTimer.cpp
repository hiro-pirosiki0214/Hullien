#include "EffectTimer.h"
#include "..\..\..\..\Global.h"

CEffectTimer::CEffectTimer()
	: m_IsUpdate	( false )
	, m_Time		( 0.0f )
{}

CEffectTimer::~CEffectTimer()
{
}

// タイマーの設定.
void CEffectTimer::Set()
{
	m_Time *= FPS;
	m_IsUpdate = true;
}

// 更新.
bool CEffectTimer::Update()
{
	if( m_IsUpdate == false ) return false;
	m_Time--;

	if( m_Time > 0.0f ) return false;
	m_IsUpdate = false;
	return true;
}