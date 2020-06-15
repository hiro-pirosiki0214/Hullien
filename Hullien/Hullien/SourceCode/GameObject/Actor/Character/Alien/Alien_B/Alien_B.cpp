#include "Alien_B.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"

CAlienB::CAlienB()
	: m_vPlayerPos		( 0.0f, 0.0f, 0.0f )
	, m_HasAimPlayer	( false )
	, m_RotAcc				( 0.0f )
{
	m_ObjectTag = EObjectTag::Alien_B;
	m_vSclae = { 0.05f, 0.05f, 0.05f };
}

CAlienB::~CAlienB()
{
}

// 初期化関数.
bool CAlienB::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	return true;
}

// 更新関数.
void CAlienB::Update()
{
	SetMoveVector( m_TargetPosition );
	CurrentStateUpdate();	// 現在の状態の更新.
}

// 描画関数.
void CAlienB::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.8f, 0.5f, 0.5f, m_ModelAlpha } );
	m_pSkinMesh->SetBlend( true );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CAlienB::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
	PlayerCollison( pActor );		// プレイヤーとの当たり判定.
	GirlCollision( pActor );			// 女の子との当たり判定.
}

// スポーン.
bool CAlienB::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if( m_NowState != EAlienState::None ) return true;
	// 初期化に失敗したら終了.
	if( Init() == false ) return false;
	m_Parameter = param;	// パラメータを設定.
	m_vPosition = spawnPos;	// スポーン座標の設定.
	m_NowState = EAlienState::Spawn;	// 現在の状態をスポーンに変更.
	return true;
}

// 相手座標の設定.
void CAlienB::SetTargetPos( CActor& actor )
{
	// 女の子の座標を取得.
	CAlien::SetTargetPos( actor );

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
	TargetRotation();			// 回転.
	if( m_HasAimPlayer == true ){
		CAlienB::VectorMove( m_Parameter.MoveSpeed );		// 移動.
		Attack();						// 攻撃.
		CAlien::WaitMove();		// 待機.
	} else {
		CAlien::VectorMove( m_Parameter.MoveSpeed );		// 移動.
		CAlien::WaitMove();		// 待機.
	}

	if (*m_pIsAlienOtherAbduct == false) return;
	if (m_NowState == EAlienState::Abduct) return;
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

	float lenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

	if( lenght >= 1.0f ) return;

	m_NowMoveState = EMoveState::Attack;
	m_RotAcc = 0.0f;
}

// 攻撃関数.
void CAlienB::Attack()
{
	if( m_NowMoveState != EMoveState::Attack ) return;

	const float power = 1.0f;
	static float pow = 0.99f;
	m_vRotation.y += (power - pow);
	pow -= 0.01f;
//	m_RotAcc += power - pow;

	if( pow > -power ) return;
	m_NowMoveState = EMoveState::Wait;
}

// プレイヤーとの当たり判定.
void CAlienB::PlayerCollison( CActor* pActor )
{
	// オブジェクトのタグがプレイヤーじゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if( m_NowMoveState != EMoveState::Attack ) return;	// 攻撃状態じゃなければ終了.
	if( m_NowState == EAlienState::Death ) return;	// 死亡していたら終了.
	if( m_NowState == EAlienState::Fright ) return;	// 怯み状態なら終了.

	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
//	pActor->LifeCalculation();
}

// プレイヤーを狙うか判定.
void CAlienB::AimPlayerDecision()
{
	if( m_NowMoveState == EMoveState::Attack ) return;

	// プレイヤーとの距離を計算.
	float playerLenght = D3DXVec3Length( &D3DXVECTOR3(m_vPlayerPos - m_vPosition) );
	// 女の子との距離を計算.
	float girlLenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	if( playerLenght <= girlLenght ){
		// プレイヤーのほうが近いのでプレイヤーを狙う.
		m_HasAimPlayer = true;
		SetMoveVector( m_vPlayerPos );
	} else {
		// 女の子のほうを狙う.
		m_HasAimPlayer = false;
		SetMoveVector( m_TargetPosition );
	}
}

// 当たり判定の設定.
bool CAlienB::ColliderSetting()
{
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
}