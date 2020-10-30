#include "Alien_B.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\Common\Effect\EffectManager.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CAlienB::CAlienB()
	: m_pAttackMesh		( nullptr )
	, m_vPlayerPos		( 0.0f, 0.0f, 0.0f )
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
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAttackModel()			== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( ColliderSetting()			== false ) return false;
	if( EffectSetting()				== false ) return false;
	if( m_pArm->Init()				== false ) return false;
	m_pSkinMesh->ChangeAnimSet_StartPos( EAnimNo_Move, 0.0f, m_pAC );
	return true;
}

// 更新関数.
void CAlienB::Update()
{
	// アニメーションフレームの更新.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	CurrentStateUpdate();	// 現在の状態の更新.
	// アーム.
	m_pArm->SetPosition( {m_vPosition.x, m_vPosition.y+5.0f, m_vPosition.z} );		// 座標をセット.
	m_pArm->SetRotationY( m_vRotation.y );	// 回転情報をセット.
	m_pArm->Update();						// 更新.
}

// 描画関数.
void CAlienB::Render()
{
	// 画面の外なら終了.
	if( IsDisplayOut() == true ) return;
	if( m_pSkinMesh == nullptr ) return;
	if( m_pAttackMesh == nullptr ) return;

	if( m_NowMoveState == EMoveState::Attack ){
		m_pAttackMesh->SetPosition( m_vPosition );
		m_pAttackMesh->SetRotation( m_vRotation );
		m_pAttackMesh->SetScale( m_vScale );
		m_pAttackMesh->SetColor( { 0.8f, 0.2f, 0.2f, 1.0f } );
		m_pAttackMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
		m_pAttackMesh->Render( m_pAC );
		m_pAttackMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	} else {
		m_pSkinMesh->SetPosition( m_vPosition );
		m_pSkinMesh->SetRotation( m_vRotation );
		m_pSkinMesh->SetScale( m_vScale );
		m_pSkinMesh->SetColor( { 0.8f, 0.2f, 0.2f, 1.0f } );
		m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
		m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
		m_pSkinMesh->Render( m_pAC );
		m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	}

	m_pArm->Render();	// アームの描画.
#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// エフェクトの描画.
void CAlienB::EffectRender()
{
	// ヒット時のエフェクト.
	m_pEffects[EEffectNo_Hit]->SetScale( 2.0f );
	m_pEffects[EEffectNo_Hit]->Render();

	// 攻撃時のエフェクト.
	m_pEffects[EEffectNo_Attack]->SetLocation( m_vPosition );
	m_pEffects[EEffectNo_Attack]->SetScale( 0.5f );
	m_pEffects[EEffectNo_Attack]->Render();
}

// 当たり判定関数.
void CAlienB::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// プレイヤーを狙っているか.
	if( m_HasAimPlayer == true ){
		PlayerCollison( pActor );		// プレイヤーとの当たり判定.
		// アームが片付けられてなければ片付ける.
		if( m_pArm->IsCleanUp() == false ) m_pArm->SetCleanUpPreparation();
	} else {
		GirlCollision( pActor );		// 女の子との当たり判定.
	}
	BarrierCollision( pActor );			// バリアとの当たり判定.
}

// スポーン.
bool CAlienB::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if( m_NowState != EAlienState::None ) return true;
	m_Parameter = param;	// パラメータを設定.
	// 初期化に失敗したら終了.
	if( Init() == false ) return false;
	m_vPosition		= spawnPos;					// スポーン座標の設定.
	m_LifePoint		= m_Parameter.LifeMax;		// 体力の設定.
	m_NowState		= EAlienState::Spawn;		// 現在の状態をスポーンに変更.
	m_AnimSpeed		= 0.0;						// アニメーション速度を止める.
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

// ライフ計算関数.
void CAlienB::LifeCalculation( const std::function<void(float&,bool&)>& proc )
{
	if( m_NowState == EAlienState::Spawn ) return;
	if( m_NowState == EAlienState::Death ) return;
	if( m_NowState == EAlienState::Fright ) return;

	bool isAttack = false;
	proc( m_LifePoint, isAttack );
	m_NowState = EAlienState::Fright;	// 怯み状態へ遷移.
	SetAnimation( EAnimNo_Damage, m_pAC );
	m_AnimSpeed = 0.01;
	m_pEffects[1]->Stop();
	m_pEffects[0]->Play( { m_vPosition.x, m_vPosition.y+4.0f, m_vPosition.z });
	if( m_pArm != nullptr ){
		// アームを片付けていなければ片付ける.
		if( m_pArm->IsCleanUp() == false ){
			m_pArm->SetCleanUp();
		}
	}

	if( m_LifePoint > 0.0f ) return;
	// 体力が 0.0以下なら死亡状態へ遷移.
	m_NowState = EAlienState::Death;
	m_NowMoveState = EMoveState::Wait;
	SetAnimation( EAnimNo_Dead, m_pAC );
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
	// プレイヤーを狙っているか.
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
	m_pEffects[1]->Stop();
	SetAnimation( EAnimNo_Move, m_pAC );
	m_NowState		= EAlienState::Escape;	// 逃げる状態へ遷移.
	m_NowMoveState	= EMoveState::Rotation;	// 移動状態を回転へ遷移する.
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

	// ベクトルを使用して移動.
	m_vPosition.x -= m_MoveVector.x * moveSpeed;
	m_vPosition.z -= m_MoveVector.z * moveSpeed;

	// プレイヤーの座標と宇宙人の座標を比較.
	if( D3DXVec3Length( &D3DXVECTOR3(m_vPlayerPos - m_vPosition) ) >= m_Parameter.AttackLenght ) return;
	if( m_IsBarrierHit == true ) return;
	m_NowMoveState	= EMoveState::Attack;
	m_RotAccValue	= m_Parameter.AttackRotInitPower;
	m_pEffects[1]->Play( m_vPosition );
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
		m_vPosition.x -= m_MoveVector.x * m_Parameter.AttackMoveSpeed;
		m_vPosition.z -= m_MoveVector.z * m_Parameter.AttackMoveSpeed;
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
	// 現在の移動ベクトルを設定.
	pActor->SetVector( m_MoveVector );
	// プレイヤーの体力を減らす.
	pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	{
		life -= m_Parameter.AttackPower;
		isAttack = true;
	});
}

// プレイヤーを狙うか判定.
void CAlienB::AimPlayerDecision()
{
	if( m_NowMoveState == EMoveState::Attack ) return;

	// プレイヤーとの距離を計算.
	float playerLenght = D3DXVec3Length( &D3DXVECTOR3(m_vPlayerPos - m_vPosition) );

	m_OldHasAimPlayer = m_HasAimPlayer;
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
	if( m_OldHasAimPlayer == m_HasAimPlayer ) return;
	m_NowMoveState = EMoveState::Rotation;
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
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	if( FAILED( m_pCollManager->InitCapsule( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		-1.0f,
		0.0f ) )) return false;
	return true;
}

// 攻撃モデルの取得.
bool CAlienB::GetAttackModel()
{
	// 既に読み込めていたら終了.
	if( m_pAttackMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetStatic( m_pAttackMesh, ATTACK_MODEL_NAME );
	// モデルが読み込めてなければ false.
	if( m_pAttackMesh == nullptr ) return false;
	return true;
}