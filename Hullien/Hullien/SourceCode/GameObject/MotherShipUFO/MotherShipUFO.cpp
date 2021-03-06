#include "MotherShipUFO.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\Actor\Actor.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\XAudio2\SoundManager.h"
#include "..\UFOLight\UFOLight.h"

CMotherShipUFO::CMotherShipUFO()
	: m_pStaticMesh		( nullptr )
	, m_pCollManager	( nullptr )
	, m_pUFOLight		( nullptr )
	, m_Param			()
	, m_IsDisp			( true )
	, m_IsReturnAlien	( false )
{
	m_pUFOLight = std::make_unique<CUFOLight>( 1.85f, 1.5f );
	m_ObjectTag = EObjectTag::MotherShipUFO;
}

CMotherShipUFO::~CMotherShipUFO()
{
}

// 初期化関数.
bool CMotherShipUFO::Init()
{
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_Param ) == false ) return false;
	if( GetModel()			== false ) return false;
	if( CollisionSetting()	== false ) return false;
	if( m_pUFOLight->Init() == false ) return false;
	m_vPosition = m_Param.Position;

	return true;
}

// 更新関数.
void CMotherShipUFO::Update()
{
	m_pUFOLight->Update();
}

// 描画関数.
void CMotherShipUFO::Render()
{
	// 画面外なら終了.
	if( IsDisplayOut()	== true ) return;
	if( m_pStaticMesh	== nullptr ) return;
	if( m_IsDisp		== false ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRasterizerState( ERS_STATE::Back );
	m_pStaticMesh->Render();
	m_pStaticMesh->SetRasterizerState( ERS_STATE::None );

	m_pUFOLight->SetPosition( m_vPosition );
	m_pUFOLight->Render();

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定.
void CMotherShipUFO::Collision( CActor* pActor )
{
	// 対象オブジェクトじゃなければ終了.
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	m_IsReturnAlien = true;
	if (CSoundManager::GetIsPlaySE("UFOSucked", 0) == false) {
		CSoundManager::PlaySE("UFOSucked");
	}
	D3DXVECTOR3 pos = pActor->GetPosition();
	pos.y += m_Param.AddPosYPower;	// 座標を上にあげる.
	pActor->SetPosition( pos );
}

// パラメータの設定.
void CMotherShipUFO::SetParameter( const SMotherShipUFOParam& param )
{
	m_Param = param;
	m_vPosition = m_Param.Position;
	if( m_pCollManager != nullptr ){
		m_pCollManager->InitSphere(
			&m_vPosition,
			&m_vRotation,
			&m_vScale.x,
			m_Param.SphereAdjPos,
			m_Param.CollisionRadius );
	}
}

// ライトを取り出す.
void CMotherShipUFO::LightDischarge()
{
	m_pUFOLight->Discharge();
}

// ライトをしまう.
void CMotherShipUFO::LightCleanUP()
{
	m_pUFOLight->CleanUP();
}

// 光を完全に放出する.
void CMotherShipUFO::DischargePreparation()
{
	m_pUFOLight->DischargePreparation();
}

// 光を片づける.
void CMotherShipUFO::CleanUPPreparation()
{
	m_pUFOLight->CleanUPPreparation();
}


// モデルの取得.
bool CMotherShipUFO::GetModel()
{
	if( m_pStaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME );
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}

// 当たり判定の設定.
bool CMotherShipUFO::CollisionSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Param.SphereAdjPos,
		m_Param.CollisionRadius ))){
		return false;
	}
	return true;
}