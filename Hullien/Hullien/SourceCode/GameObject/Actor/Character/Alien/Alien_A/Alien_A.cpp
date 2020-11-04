#include "Alien_A.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\Common\Effect\EffectManager.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CAlienA::CAlienA()
	: CAlienA	( nullptr )
{}

CAlienA::CAlienA( const SAlienParam* pParam )
	: CAlien	( pParam )
{
	m_ObjectTag = EObjectTag::Alien_A;
	m_pArm = std::make_unique<CArm>();
}

CAlienA::~CAlienA()
{
}

// 初期化関数.
bool CAlienA::Init()
{
	if( pPARAMETER					== nullptr ) return false;
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( ColliderSetting()			== false ) return false;
	if( EffectSetting()				== false ) return false;
	if( m_pArm->Init()				== false ) return false;
	m_pSkinMesh->ChangeAnimSet_StartPos( alien::EAnimNo_Move, 0.0f, m_pAC );
	return true;
}

// 更新関数.
void CAlienA::Update()
{
	// アニメーションフレームの更新.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );

	SetMoveVector( m_TargetPosition );	// 目的の座標のベクトルを取得.
	CurrentStateUpdate();				// 現在の状態の更新.
	// アーム.
	m_pArm->SetPosition( {m_vPosition.x, m_vPosition.y+5.0f, m_vPosition.z} );		// 座標を設定.
	m_pArm->SetRotationY( m_vRotation.y );	// 回転情報を設定.
	m_pArm->Update();						// 更新.
}

// 描画関数.
void CAlienA::Render()
{
	// 画面の外なら終了.
	if( IsDisplayOut() == true ) return;
	if( m_pSkinMesh == nullptr ) return;
	
	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetColor( { 0.2f, 0.8f, 0.2f, 1.0f } );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render( m_pAC );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );

	m_pArm->Render();	// アームの描画.

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CAlienA::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	GirlCollision( pActor );	// 女の子との当たり判定.
	BarrierCollision( pActor );	// バリアとの当たり判定.
}

// スポーン.
bool CAlienA::Spawn( const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if( m_NowState != alien::EAlienState::None ) return true;
	m_vPosition			= spawnPos;						// スポーン座標の設定.
	m_LifePoint			= pPARAMETER->LifeMax;			// 体力の設定.
	m_NowState			= alien::EAlienState::Spawn;	// 現在の状態をスポーンに変更.
	m_AnimSpeed			= 0.0;							// アニメーション速度を止める.
	m_pEffects[alien::EEffectNo_Spawn]->Play( m_vPosition );
	return true;
}

// スポーン.
void CAlienA::Spawning()
{
	CAlien::Spawning();
}

// 移動.
void CAlienA::Move()
{
	CAlien::Move();
}

// 拐う.
void CAlienA::Abduct()
{
	CAlien::Abduct();
}

// 怯み.
void CAlienA::Fright()
{
	CAlien::Fright();
}

// 死亡.
void CAlienA::Death()
{
	CAlien::Death();
}

// 逃げる.
void CAlienA::Escape()
{
	CAlien::Escape();
}

// 当たり判定の設定.
bool CAlienA::ColliderSetting()
{
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		pPARAMETER->SphereAdjPos,
		pPARAMETER->SphereAdjRadius ) )) return false;
	return true;
}