#include "Alien_B.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Arm\Arm.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

#define IS_TEMP_MODEL_RENDER

CAlienB::CAlienB()
	: m_vPlayerPos		( 0.0f, 0.0f, 0.0f )
	, m_HasAimPlayer	( false )
	, m_OldHasAimPlayer	( false )
	, m_RotAccValue		( 0.0f )
	, m_IsAttackSE		( false )
{
	m_ObjectTag = EObjectTag::Alien_B;
	m_pArm = std::make_unique<CArm>();
}

CAlienB::~CAlienB()
{
}

// 初期化関数.
bool CAlienB::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#else
	// 既に読み込めていたら終了.
	if( m_pTempStaticMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetStatic( m_pTempStaticMesh, MODEL_TEMP_NAME );
	// モデルが読み込めてなければ false.
	if( m_pTempStaticMesh == nullptr ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( ColliderSetting() == false ) return false;
	if( m_pArm->Init() == false ) return false;
	return true;
}

// 更新関数.
void CAlienB::Update()
{
	CurrentStateUpdate();	// 現在の状態の更新.
	m_pArm->SetPosition( m_vPosition );
	m_pArm->SetRotationY( m_vRotation.y + static_cast<float>(D3DX_PI) );
	m_pArm->Update();
}

// 描画関数.
void CAlienB::Render()
{
	// 画面の外なら終了.
	if( IsDisplayOut() == true ) return;
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
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pTempStaticMesh->SetRotation( rot );
	m_pTempStaticMesh->SetScale( m_vSclae );
	m_pTempStaticMesh->SetColor( { 0.0f, 0.8f, 0.0f, m_ModelAlpha } );
	AlphaBlendSetting();
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pTempStaticMesh->Render();
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pTempStaticMesh->SetBlend( false );
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
	m_pArm->Render();
#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CAlienB::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
	if( m_HasAimPlayer == true ){
		PlayerCollison( pActor );		// プレイヤーとの当たり判定.
		if( m_pArm->IsCleanUp() == false ) m_pArm->SetCleanUp();
	} else {
		GirlCollision( pActor );		// 女の子との当たり判定.
	}
	BarrierCollision( pActor );
}

// スポーン.
bool CAlienB::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if( m_NowState != EAlienState::None ) return true;
	m_Parameter = param;	// パラメータを設定.
	// 初期化に失敗したら終了.
	if( Init() == false ) return false;
	m_vPosition = spawnPos;	// スポーン座標の設定.
	m_LifePoint = m_Parameter.LifeMax;	// 体力の設定.
	m_vPosition.y += INIT_POSITION_ADJ_HEIGHT;
	m_NowState = EAlienState::Spawn;	// 現在の状態をスポーンに変更.
	return true;
}

// 相手座標の設定.
void CAlienB::SetTargetPos( CActor& actor )
{
	// 女の子の座標を取得.
	CAlien::SetTargetPos( actor );
	// プレイヤーの座標を取得.
	CAlienB::SetPlayerPos( actor );
}

// プレイヤー座標の設定.
void CAlienB::SetPlayerPos( CActor& actor )
{
	// プレイヤーじゃなければ終了.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_vPlayerPos = actor.GetPosition();	// プレイヤーの座標を取得.
}

// スポーン.
void CAlienB::Spawning()
{
	CAlien::Spawning();
}

// 移動.
void CAlienB::Move()
{
	AimPlayerDecision();	// プレイヤーを狙うか判定.
	TargetRotation();		// 回転.
	if( m_HasAimPlayer == true ){
		CAlienB::VectorMove( m_MoveSpeed );	// 移動.
		Attack();				// 攻撃.
		CAlien::WaitMove();		// 待機.
	} else {
		CAlien::VectorMove( m_MoveSpeed );		// 移動.
		CAlien::WaitMove();		// 待機.
	}

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState = EAlienState::Escape;
	m_NowMoveState = EMoveState::Rotation;	// 移動状態を回転する.
}

// 拐う.
void CAlienB::Abduct()
{
	CAlien::Abduct();
}

// 怯み.
void CAlienB::Fright()
{
	CAlien::Fright();
}

// 死亡.
void CAlienB::Death()
{
	CAlien::Death();
}

// 逃げる.
void CAlienB::Escape()
{
	CAlien::Escape();
}

// 移動関数.
void CAlienB::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;

	float lenght = D3DXVec3Length( &D3DXVECTOR3(m_vPlayerPos - m_vPosition) );

	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

	if( lenght >= m_Parameter.AttackLenght ) return;

	m_NowMoveState = EMoveState::Attack;
	m_RotAccValue = m_Parameter.AttackRotInitPower;
}

// 攻撃関数.
void CAlienB::Attack()
{
	if( m_NowMoveState != EMoveState::Attack ) return;

	// 回転.
	m_vRotation.y += (m_Parameter.AttackRotPower - fabsf(m_RotAccValue));
	m_RotAccValue -= m_Parameter.AttackRotAddValue;	// 回転加速度を加算.

	// 加速度が移動範囲なら移動.
	if( -m_Parameter.AttackMoveRange <= m_RotAccValue && m_RotAccValue <= m_Parameter.AttackMoveRange ){
		m_vPosition.x += m_MoveVector.x * m_Parameter.AttackMoveSpeed;
		m_vPosition.z += m_MoveVector.z * m_Parameter.AttackMoveSpeed;
	}
	if (m_IsAttackSE == false)
	{
		CSoundManager::NoMultipleSEPlay("AlienAttack");
		m_IsAttackSE = true;
	}

	if( m_RotAccValue > -m_Parameter.AttackRotPower ) return;
	m_NowMoveState = EMoveState::Wait;
	m_IsAttackSE = false;
}

// プレイヤーとの当たり判定.
void CAlienB::PlayerCollison( CActor* pActor )
{
	// オブジェクトのタグがプレイヤーじゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if( m_NowMoveState != EMoveState::Attack ) return;	// 攻撃状態じゃなければ終了.
	if( m_NowState == EAlienState::Death ) return;	// 死亡していたら終了.
	if( m_NowState == EAlienState::Fright ) return;	// 怯み状態なら終了.
	
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetCollManager() ) == false ) return;
	// プレイヤーの体力を減らす.
	pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	{ 
		life -= m_Parameter.AttackPower;
		isAttack = true;
	});
	pActor->SetVector( m_MoveVector );
	CSoundManager::NoMultipleSEPlay("PlayerVoiceHit");
}

// プレイヤーを狙うか判定.
void CAlienB::AimPlayerDecision()
{
	if( m_NowMoveState == EMoveState::Attack ) return;

	// プレイヤーとの距離を計算.
	float playerLenght = D3DXVec3Length( &D3DXVECTOR3(m_vPlayerPos - m_vPosition) );

	m_OldHasAimPlayer = m_HasAimPlayer;
#if 1
	// プレイヤーの狙う範囲を比較.
	if( playerLenght <= m_Parameter.PlayerAimLenght ){
		// プレイヤーのほうが近いのでプレイヤーを狙う.
		m_HasAimPlayer = true;
		SetMoveVector( m_vPlayerPos );
	} else {
		// 女の子のほうを狙う.
		m_HasAimPlayer = false;
		SetMoveVector( m_TargetPosition );
	}
#else
	// 女の子との距離を計算.
	float girlLenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	// 女の子とプレイヤーの距離を比較.
	if( playerLenght <= girlLenght ){
		// プレイヤーのほうが近いのでプレイヤーを狙う.
		m_HasAimPlayer = true;
		SetMoveVector( m_vPlayerPos );
	} else {
		// 女の子のほうを狙う.
		m_HasAimPlayer = false;
		SetMoveVector( m_TargetPosition );
	}
#endif // #if 0.
	if( m_OldHasAimPlayer == m_HasAimPlayer ) return;
	m_NowMoveState = EMoveState::Rotation;
}

// 当たり判定の設定.
bool CAlienB::ColliderSetting()
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
	if( FAILED( m_pCollManager->InitCapsule( 
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		-1.0f,
		0.0f ) )) return false;

	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}