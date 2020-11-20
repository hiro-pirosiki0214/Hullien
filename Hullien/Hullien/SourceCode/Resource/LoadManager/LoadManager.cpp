#include "LoadManager.h"
#include "..\..\Common\Sprite\CSprite.h"
#include "..\EffectResource\EffectResource.h"
#include "..\MeshResource\MeshResource.h"
#include "..\SpriteResource\SpriteResource.h"
#include "..\FontResource\FontResource.h"
#include "..\..\XAudio2\SoundManager.h"

CLoadManager::CLoadManager()
	: m_Thread			()
	, m_isLoadEnd		( false )
	, m_isThreadJoined	( false )
{
}

CLoadManager::~CLoadManager()
{
}

//------------------------.
// ì«Ç›çûÇ›.
//------------------------.
void CLoadManager::LoadResource(  
	HWND hWnd, 
	ID3D11Device* pDevice11, 
	ID3D11DeviceContext* pContext11, 
	LPDIRECT3DDEVICE9 pDevice9 )
{
	CSoundManager::CreateSoundData();
	auto load = [&]( HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9 )
	{
		CFontResource::Load( pDevice11, pContext11 );
		CSpriteResource::Load( pDevice11, pContext11 );
		CMeshResorce::Load( hWnd, pDevice11, pContext11, pDevice9 );
		CEffectResource::Load( pDevice11, pContext11 );
		m_isLoadEnd = true;
	};
	m_Thread = std::thread( load, hWnd, pDevice11, pContext11, pDevice9 );
}

//------------------------.
// ÉXÉåÉbÉhÇÃâï˙.
//------------------------.
bool CLoadManager::ThreadRelease()
{
	if( m_isThreadJoined == true ) return true;
	if( m_isLoadEnd == false ) return false;
	m_Thread.join();
	m_isThreadJoined = true;
	return true;
}