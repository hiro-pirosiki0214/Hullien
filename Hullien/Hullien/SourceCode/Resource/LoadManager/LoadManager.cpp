#include "LoadManager.h"
#include "..\..\Common\Sprite\CSprite.h"
#include "..\EffectResource\EffectResource.h"
#include "..\MeshResource\MeshResource.h"
#include "..\SpriteResource\SpriteResource.h"
#include "..\FontResource\FontResource.h"
#include "..\..\XAudio2\SoundManager.h"

CLoadManager::CLoadManager()
	: m_Thread				()
	, m_Sprites				()
	, m_Mutex				()
	, m_isLoadEnd			( false )
	, m_isThreadJoined		( false )
	, m_isLoadFailed		( false )
	, m_isLoadSpriteFailed	( false )
{
}

CLoadManager::~CLoadManager()
{
	if( m_isLoadSpriteFailed == true ) return;
	if( m_isLoadFailed == true ) m_Thread.join();
}

//------------------------.
// 読み込み.
//------------------------.
void CLoadManager::LoadResource(  
	HWND hWnd, 
	ID3D11Device* pDevice11, 
	ID3D11DeviceContext* pContext11, 
	LPDIRECT3DDEVICE9 pDevice9 )
{
	if( FAILED( GetSprite( pDevice11, pContext11 ) )){
		m_isLoadSpriteFailed = true;
		m_isLoadFailed = true;
		return;
	}

	CFontResource::Load( pDevice11, pContext11 );
	CSoundManager::CreateSoundData();
	auto load = [&]( 
		HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9 )
	{
		m_Mutex.lock();
		if( FAILED( CSpriteResource::Load( pDevice11, pContext11 ) )){
			m_Mutex.unlock();
			m_isLoadFailed = true;
			return;
		}
		if( FAILED( CMeshResorce::Load( hWnd, pDevice11, pContext11, pDevice9 ) )){
			m_Mutex.unlock();
			m_isLoadFailed = true;
			return;
		}
//		CEffectResource::Load( pDevice11, pContext11 );
		m_isLoadEnd = true;
		m_Mutex.unlock();
	};
	m_Thread = std::thread( load, hWnd, pDevice11, pContext11, pDevice9 );
}

//------------------------.
// 描画.
//------------------------.
void CLoadManager::Render()
{
	for( auto& s : m_Sprites ){
		s->SetFrameTime( 6 );
		s->SetBlend( true );
		s->SetDeprh( false );
		s->RenderUI();
		s->SetBlend( false );
		s->SetDeprh( true );
	}
}

//------------------------.
// スレッドの解放.
//------------------------.
bool CLoadManager::ThreadRelease()
{
	if( m_isThreadJoined == true ) return true;
	if( m_isLoadEnd == false ) return false;
	m_Thread.join();
	m_isThreadJoined = true;
	return true;
}

//------------------------.
// スプライトの取得.
//------------------------.
HRESULT CLoadManager::GetSprite(
	ID3D11Device* pDevice11, 
	ID3D11DeviceContext* pContext11 )
{
	const char* names[] =
	{
		LOAD_BACK_IMAGE,
		LOAD_ANIM_IMAGE,
		LOAD_TEXT_IMAGE
	};
	const int size = sizeof(names)/sizeof(names[0]);

	for( int i = 0; i < size; i++ ){
		SSpriteState ss = CSpriteResource::SpriteStateRead( names[i] );
		m_Sprites.emplace_back( std::make_shared<CSprite>() );
		if( FAILED( m_Sprites.back()->Init( pDevice11, pContext11, names[i], ss ) )) return E_FAIL;
	}

	return S_OK;
}