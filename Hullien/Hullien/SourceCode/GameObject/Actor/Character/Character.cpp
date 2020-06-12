#include "Character.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

#include "..\..\..\Utility\FileManager\FileManager.h"

CCharacter::CCharacter()
	: m_pSkinMesh				( nullptr )
	, m_MoveVector				( 0.0f, 0.0f, 0.0f )
	, m_InvincibleCount			( 0 )
	, m_HasFinishedParamSetting	( false )
{
	
}

CCharacter::~CCharacter()
{
}

// 無敵時間かどうか.
bool CCharacter::IsInvincibleTime( const int& invincibleTime )
{
	return m_InvincibleCount > invincibleTime * FPS;
}

// モデルの取得.
bool CCharacter::GetModel( const char* modelName )
{
	// 既に読み込めていたら終了.
	if( m_pSkinMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetSkin( m_pSkinMesh, modelName );
	// モデルが読み込めてなければ false.
	if( m_pSkinMesh == nullptr ) return false;
	return true;
}