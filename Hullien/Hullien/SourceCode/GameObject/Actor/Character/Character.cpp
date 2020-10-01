#include "Character.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Utility\FileManager\FileManager.h"

CCharacter::CCharacter()
	: m_pSkinMesh				( nullptr )
#ifdef IS_TEMP_MODEL_RENDER
	, m_pTempStaticMesh			( nullptr )
#endif
	, m_MoveVector				( 0.0f, 0.0f, 0.0f )
	, m_InvincibleCount			( 0 )
	, m_HasFinishedParamSetting	( false )
{
	m_vPosition.y = INIT_POSITION_ADJ_HEIGHT;
//	m_vSclae = { 0.1f, 0.1f, 0.1f };
}

CCharacter::~CCharacter()
{
}

// メッシュの表示.
void CCharacter::MeshRender()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetAnimSpeed( 0.001 );
	m_pSkinMesh->Render();
#else
	if( m_pTempStaticMesh == nullptr ) return;

	m_pTempStaticMesh->SetPosition( m_vPosition );
	m_pTempStaticMesh->SetRotation( m_vRotation );
	m_pTempStaticMesh->SetScale( m_vSclae );
	m_pTempStaticMesh->Render();
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
}

// 無敵時間かどうか.
bool CCharacter::IsInvincibleTime( const int& invincibleTime )
{
	return m_InvincibleCount > invincibleTime * FPS;
}

// モデルの取得.
bool CCharacter::GetModel( const char* modelName )
{
#ifndef IS_TEMP_MODEL_RENDER
	// 既に読み込めていたら終了.
	if( m_pSkinMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetSkin( m_pSkinMesh, modelName );
	// モデルが読み込めてなければ false.
	if( m_pSkinMesh == nullptr ) return false;
	return true;
#else
	// 既に読み込めていたら終了.
	if( m_pTempStaticMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetStatic( m_pTempStaticMesh, modelName );
	// モデルが読み込めてなければ false.
	if( m_pTempStaticMesh == nullptr ) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}