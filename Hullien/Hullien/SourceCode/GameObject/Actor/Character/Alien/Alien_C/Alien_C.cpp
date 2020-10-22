#include "Alien_C.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CAlienC::CAlienC()
{
	m_ObjectTag = EObjectTag::Alien_C;
	m_pArm = std::make_unique<CArm>();
}

CAlienC::~CAlienC()
{
}

// 初期化関数.
bool CAlienC::Init()
{
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( ColliderSetting()			== false ) return false;
	if( m_pArm->Init()				== false ) return false;
	return true;
}

// 更新関数.
void CAlienC::Update()
{
	// アニメーションフレームの更新.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	SetMoveVector( m_TargetPosition );	// 目的のベクトルを取得.
	CurrentStateUpdate();				// 現在の状態の更新.
	// アーム.
	m_pArm->SetPosition( {m_vPosition.x, m_vPosition.y+5.5f, m_vPosition.z} );		// 座標を設定.
	m_pArm->SetRotationY( m_vRotation.y );	// 回転値を設定.
	m_pArm->Update();						// 更新.

	// アルファ値が0より多ければ終了.
	if( m_IsDelete == false ) return;
	if( m_NowState == EAlienState::RisingMotherShip ) return;	// マザーシップに昇っている状態なら終了.
	m_IsExplosion = true;	// 爆発させる.
	CSoundManager::PlaySE("Bomb");
}

// 描画関数.
void CAlienC::Render()
{
	// 画面の外なら終了.
	if( IsDisplayOut() == true ) return;
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetColor( { 0.5f, 0.8f, 0.5f, 1.0f } );
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
void CAlienC::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	GirlCollision( pActor );	// 女の子との当たり判定.
	BarrierCollision( pActor );	// バリアとの当たり判定.
}

// スポーン.
bool CAlienC::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if( m_NowState != EAlienState::None ) return true;
	m_Parameter = param;	// パラメータを設定.
	// 初期化に失敗したら終了.
	if( Init() == false ) return false;
	m_vPosition		= spawnPos;				// スポーン座標の設定.
	m_LifePoint		= m_Parameter.LifeMax;	// 体力の設定.
	m_NowState		= EAlienState::Spawn;	// 現在の状態をスポーンに変更.
	m_AnimSpeed		= 0.0;					// アニメーション速度を止める.
	return true;
}

// スポーン.
void CAlienC::Spawning()
{
	CAlien::Spawning();
}

// 移動.
void CAlienC::Move()
{
	CAlien::Move();
}

// 拐う.
void CAlienC::Abduct()
{
	CAlien::Abduct();
}

// 怯み.
void CAlienC::Fright()
{
	CAlien::Fright();
}

// 死亡.
void CAlienC::Death()
{
	CAlien::Death();
}

// 逃げる.
void CAlienC::Escape()
{
	CAlien::Escape();
}

// 当たり判定の設定.
bool CAlienC::ColliderSetting()
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
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
}