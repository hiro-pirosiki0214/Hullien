#include "EffectManager.h"
#include "..\..\Resource\EffectResource\EffectResource.h"

CEffectManager::CEffectManager()
	: m_pEffect	( nullptr )
	, m_Handle	( -1 )
{
}

CEffectManager::~CEffectManager()
{
}

//------------------------------.
// エフェクトの設定.
//------------------------------.
bool CEffectManager::SetEffect( const char* effectName )
{
	if( m_pEffect != nullptr ) return true;
	m_pEffect = CEffectResource::GetEffect( effectName );
	if( m_pEffect == nullptr ) return false;
	return true;
}

//------------------------------.
// 描画.
//------------------------------.
void CEffectManager::Render()
{
	if( m_pEffect == nullptr ) return;
	if( m_Handle == -1 ) return;
	m_pEffect->Render( m_Handle );
}

//------------------------------.
// 再生.
//------------------------------.
void CEffectManager::Play( const D3DXVECTOR3 vPos )
{
	if( m_pEffect == nullptr ) return;
	m_Handle = m_pEffect->Play( vPos );
}

//------------------------------.
// 停止.
//------------------------------.
void CEffectManager::Stop()
{
	m_pEffect->Stop( m_Handle );
}

//------------------------------.
// 全て停止.
//------------------------------.
void CEffectManager::StopAll()
{
	m_pEffect->StopAll();
}

//------------------------------.
// 一時停止.
//------------------------------.
void CEffectManager::Pause( const bool bFlag )
{
	m_pEffect->Pause( m_Handle, bFlag ); 
}

//------------------------------.
// 位置を指定する.
//------------------------------.
void CEffectManager::SetLocation( const D3DXVECTOR3 vPos )
{
	m_pEffect->SetLocation( m_Handle, vPos );
}

//------------------------------.
// サイズを指定する.
//------------------------------.
void CEffectManager::SetScale( const D3DXVECTOR3& vScale )
{
	m_pEffect->SetScale( m_Handle, vScale );
}
void CEffectManager::SetScale( const float& scale )
{
	m_pEffect->SetScale( m_Handle, { scale, scale, scale } );
}

//------------------------------.
// 回転を指定する.
//------------------------------.
void CEffectManager::SetRotation( const D3DXVECTOR3& vRot )
{
	m_pEffect->SetRotation( m_Handle, vRot );
}

//------------------------------.
// 再生速度を設定する.
//------------------------------.
void CEffectManager::SetSpeed( const float fSpeed )
{
	m_pEffect->SetSpeed( m_Handle, fSpeed );
}

//------------------------------.
// 存在しているか.
//------------------------------.
bool CEffectManager::IsExists()
{
	return m_pEffect->IsExists( m_Handle );
}