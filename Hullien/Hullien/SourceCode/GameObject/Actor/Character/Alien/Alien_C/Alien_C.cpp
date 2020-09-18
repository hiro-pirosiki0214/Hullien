#include "Alien_C.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"

CAlienC::CAlienC()
{
	m_ObjectTag = EObjectTag::Alien_C;
	m_HasAnyItem = EItemList::MoveSpeedUp;
}

CAlienC::~CAlienC()
{
}

// 初期化関数.
bool CAlienC::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#else
	if( GetModel( MODEL_TEMP_NAME ) == false ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( ColliderSetting() == false ) return false;
	return true;
}

// 更新関数.
void CAlienC::Update()
{
	SetMoveVector( m_TargetPosition );
	CurrentStateUpdate();	// 現在の状態の更新.
	if( m_ModelAlpha > 0.0f ) return;
	// 体力が0より多ければ終了.
//	if( m_LifePoint > 0.0f ) return;
	// 体力がなければ爆発させる.
	m_IsExplosion = true;
	CSoundManager::PlaySE("Bomb");
}

// 描画関数.
void CAlienC::Render()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.5f, 0.8f, 0.5f, m_ModelAlpha } );
	AlphaBlendSetting();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );
#else
	if( m_pTempStaticMesh == nullptr ) return;

	if( m_pTempStaticMesh == nullptr ) return;
	m_pTempStaticMesh->SetPosition( m_vPosition );
	m_pTempStaticMesh->SetRotation( m_vRotation );
	m_pTempStaticMesh->SetScale( m_vSclae );
	m_pTempStaticMesh->SetColor( { 0.0f, 0.0f, 0.8f, m_ModelAlpha } );
	AlphaBlendSetting();
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pTempStaticMesh->Render();
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pTempStaticMesh->SetBlend( false );
#endif	// #ifdef IS_TEMP_MODEL_RENDER.

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

	GirlCollision( pActor );
	BarrierCollision( pActor );
}

// スポーン.
bool CAlienC::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if( m_NowState != EAlienState::None ) return true;
	// 初期化に失敗したら終了.
	if( Init() == false ) return false;
	m_Parameter = param;	// パラメータを設定.
	m_vPosition = spawnPos;	// スポーン座標の設定.
	m_LifePoint = m_Parameter.LifeMax;	// 体力の設定.
	m_vPosition.y += INIT_POSITION_ADJ_HEIGHT;
	m_NowState = EAlienState::Spawn;	// 現在の状態をスポーンに変更.

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
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
#else
	if( m_pTempStaticMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}