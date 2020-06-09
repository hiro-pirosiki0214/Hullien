#include "Character.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

CCharacter::CCharacter()
	: m_pSkinMesh	( nullptr )
	, m_Life		( 1.0f )
	, m_MoveVector	( 0.0f, 0.0f, 0.0f )
{
	
}

CCharacter::~CCharacter()
{
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