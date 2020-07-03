#include "SphereCollision.h"
#include "SphereModel.h"

#ifdef _DEBUG
bool CSphereCollision::m_IsRender = false;
#endif	// #ifdef _DEBUG.

CSphereCollision::CSphereCollision()
	: m_Radius			( 0.0f )
	, m_AdjRadius		( 0.0f )
#ifdef _DEBUG
	, m_pDebugSphere	( std::make_unique<CSphereModel>() )
#endif	// #ifdef _DEBUG.
{
}

CSphereCollision::~CSphereCollision()
{
}

//----------------------------------------.
// ƒ‚ƒfƒ‹‚Ì”¼Œa‚Ìì¬.
//----------------------------------------.
HRESULT CSphereCollision::InitModelRadius( LPD3DXMESH pMesh )
{
	GetBoundingSphere( pMesh, m_Radius );

	m_Radius = (m_Radius * (*m_pScale)) + m_AdjRadius;
	
#ifdef _DEBUG
	if( FAILED( m_pDebugSphere->Init( m_Radius ))){
		ERROR_MESSAGE("Model creation failed");
		return E_FAIL;
	}
#endif	// #ifdef _DEBUG.

	return S_OK;
}

//----------------------------------------.
// ƒ‚ƒfƒ‹‚Ì”¼Œa‚Ìì¬.
//----------------------------------------.
HRESULT CSphereCollision::InitModelRadius( const float& radius )
{
	m_Radius = radius;
#ifdef _DEBUG
	if( FAILED( m_pDebugSphere->Init( m_Radius ))){
		ERROR_MESSAGE("Model creation failed");
		return E_FAIL;
	}
#endif	// #ifdef _DEBUG.
	return S_OK;
}

//----------------------------------------.
// “–‚½‚è”»’è‚Ì•\Ž¦.
//----------------------------------------.
void CSphereCollision::DebugRender()
{
#ifdef _DEBUG
	if( ( GetAsyncKeyState('D') & 0x8000 ) &&
		( GetAsyncKeyState('E') & 0x8000 )){
		if( GetAsyncKeyState('S') & 0x0001 ) m_IsRender = !m_IsRender;
	}
	if( m_IsRender == false ) return;
	m_pDebugSphere->SetPosition( GetPosition() );
	m_pDebugSphere->Render();
#endif	// #ifdef _DEBUG.
}

//----------------------------------------.
// F‚ð•Ï‚¦‚é‚©‚Ç‚¤‚©.
//----------------------------------------.
void CSphereCollision::SetChangeColor( const bool& changed )
{
#ifdef _DEBUG
	m_pDebugSphere->ChangeColor( changed );
#endif	// #ifdef _DEBUG.
}