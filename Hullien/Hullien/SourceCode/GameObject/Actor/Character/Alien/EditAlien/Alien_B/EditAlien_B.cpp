#include "EditAlien_B.h"
#include "..\..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\..\..\XAudio2\SoundManager.h"

CEditAlienB::CEditAlienB()
	: m_RotAccValue	( 0.0f )
	, m_IsAttackSE	( false )
{
	m_vScale = { 1.0f, 1.0f, 1.0f };
}

CEditAlienB::~CEditAlienB()
{}

// 初期化関数.
bool CEditAlienB::Init()
{
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( ColliderSetting()			== false ) return false;
	if( EffectSetting()				== false ) return false;
	m_pSkinMesh->ChangeAnimSet_StartPos( alien::EAnimNo_Move, 0.0f, m_pAC );
	return true;
}

// 更新関数.
void CEditAlienB::Update()
{
	// アニメーションフレームの更新.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	CurrentStateUpdate();				// 現在の状態の更新.
}

// 描画関数.
void CEditAlienB::Render()
{
	// 画面の外なら終了.
	if( IsDisplayOut() == true ) return;
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetColor( { 0.2f, 0.8f, 0.2f, 1.0f } );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->SetRasterizerState( ERS_STATE::Back );
	m_pSkinMesh->Render( m_pAC );
	m_pSkinMesh->SetRasterizerState( ERS_STATE::None );

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CEditAlienB::Collision( CActor* pActor )
{}

// 攻撃の再生.
void CEditAlienB::PlayAttack()
{
	if( m_IsPlaying == true ) return;
	m_RotAccValue	= m_Paramter.AttackRotInitPower;
	m_NowState = alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Attack;
	m_IsPlaying = true;
}

// スポーン.
void CEditAlienB::Spawning()
{
	CEditAlien::Spawning();
}

// 移動.
void CEditAlienB::Move()
{
	CEditAlien::Move();
	Attack();
}

// 拐う.
void CEditAlienB::Abduct()
{
	CEditAlien::Abduct();
}

// 怯み.
void CEditAlienB::Fright()
{
	CEditAlien::Fright();
}

// 死亡.
void CEditAlienB::Death()
{
	CEditAlien::Death();
}

// 逃げる.
void CEditAlienB::Escape()
{
	CEditAlien::Escape();
}

// 攻撃関数.
void CEditAlienB::Attack()
{
	if( m_NowMoveState != alien::EMoveState::Attack ) return;

	// 回転.
	m_vRotation.y += (m_Paramter.AttackRotPower - fabsf(m_RotAccValue));
	m_RotAccValue -= m_Paramter.AttackRotAddValue;	// 回転加速度を加算.

	// 加速度が移動範囲なら移動.
	if( -m_Paramter.AttackMoveRange <= m_RotAccValue && m_RotAccValue <= m_Paramter.AttackMoveRange ){
		m_vPosition.x -= m_MoveVector.x * m_Paramter.AttackMoveSpeed;
		m_vPosition.z -= m_MoveVector.z * m_Paramter.AttackMoveSpeed;
	}
	if (m_IsAttackSE == false)
	{
		CSoundManager::NoMultipleSEPlay("AlienAttack");
		m_IsAttackSE = true;
	}

	if( m_RotAccValue > -m_Paramter.AttackRotPower ) return;
	m_NowMoveState = alien::EMoveState::Wait;
	m_IsAttackSE = false;
	m_IsPlaying = false;
}

// 当たり判定の設定.
bool CEditAlienB::ColliderSetting()
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
		m_Paramter.SphereAdjPos,
		m_Paramter.SphereAdjRadius ) )) return false;
	return true;
}