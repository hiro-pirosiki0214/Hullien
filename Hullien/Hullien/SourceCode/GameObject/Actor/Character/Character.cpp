#include "Character.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\XAudio2\SoundManager.h"

CCharacter::CCharacter()
	: m_pSkinMesh				( nullptr )
	, m_MoveVector				( 0.0f, 0.0f, 0.0f )
	, m_InvincibleCount			( 0 )
	, m_HasFinishedParamSetting	( false )
	, m_pFootCollision			()
	, m_pGroundCollision		( nullptr )
	, m_vGroundPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vRightPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vLeftPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
}

CCharacter::~CCharacter()
{
}

// メッシュの表示.
void CCharacter::MeshRender()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetAnimSpeed( 0.01 );
	m_pSkinMesh->Render();
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


void CCharacter::FootStep(const char * rightfoot, const char * leftfoot)
{
	m_vGroundPosition = m_vPosition;
	m_pSkinMesh->GetPosFromBone(leftfoot, &m_vLeftPosition);
	m_pSkinMesh->GetPosFromBone(rightfoot, &m_vRightPosition);

	if (m_pFootCollision[0]->IsShereToShere(m_pGroundCollision.get()) == true
		|| m_pFootCollision[1]->IsShereToShere(m_pGroundCollision.get()) == true)
	{
		CSoundManager::NoMultipleSEPlay("Walk");
	}

	m_pGroundCollision->DebugRender();
	m_pFootCollision[0]->DebugRender();
	m_pFootCollision[1]->DebugRender();
}

bool CCharacter::FootStepCollisionSetting()
{
	// 地面の当たり判定.
	if (m_pGroundCollision != nullptr) return true;
	if (m_pGroundCollision == nullptr)
	{
		m_pGroundCollision = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pGroundCollision->InitSphere(
		&m_vGroundPosition,
		&m_vRotation,
		&m_vSclae.x,
		{0.0f,0.0f,0.0f},
		1.0f))) return false;

	// 足の当たり判定.
	if (m_pFootCollision.size() != 0) return true;
	m_pFootCollision.emplace_back(std::make_shared<CCollisionManager>());
	m_pFootCollision.emplace_back(std::make_shared<CCollisionManager>());
	if (FAILED(m_pFootCollision[0]->InitSphere(
		&m_vRightPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f,0.0f,0.0f },
		0.5f))) return false;
	if (FAILED(m_pFootCollision[1]->InitSphere(
		&m_vLeftPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f,0.0f,0.0f },
		0.5f))) return false;
	return true;
}
