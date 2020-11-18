#include "EditAlien_D.h"
#include "..\..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\..\LaserBeam\LaserBeam.h"

CEditAlienD::CEditAlienD()
	: m_pAttackRangeSprite	( nullptr )
	, m_pLaserBeam			( nullptr )
	, m_ControlPositions	( 1 )
	, m_AttackCount			( 0.0f )
	, m_IsAttackStart		( false )
{
	m_vScale = { 1.0f, 1.0f, 1.0f };
	m_pLaserBeam = std::make_unique<CLaserBeam>();
}

CEditAlienD::~CEditAlienD()
{}

// 初期化関数.
bool CEditAlienD::Init()
{
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( ColliderSetting()			== false ) return false;
	if( EffectSetting()				== false ) return false;
	if( m_pLaserBeam->Init()		== false ) return false;
	m_pSkinMesh->ChangeAnimSet_StartPos( alien::EAnimNo_Move, 0.0f, m_pAC );
	return true;
}

// 更新関数.
void CEditAlienD::Update()
{
	// アニメーションフレームの更新.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	CurrentStateUpdate();				// 現在の状態の更新.
	m_pLaserBeam->Update();
}

// 描画関数.
void CEditAlienD::Render()
{
	m_pLaserBeam->Render();	// レーザーの描画.
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
void CEditAlienD::Collision( CActor* pActor )
{}

// 攻撃の再生.
void CEditAlienD::PlayAttack()
{
	m_IsAttackStart	= false;	// 攻撃が始まるフラグを下す.
	m_AttackCount	= 0.0f;		// 攻撃カウントを初期化,
	m_NowMoveState	= alien::EMoveState::Attack;	// 攻撃状態へ遷移.
	m_pLaserBeam->SetTargetPos( { 3.0f, 0.0f, 3.0f } );	// プレイヤーの座標を取得.
	SetAnimation( alien::EAnimNo_Attack, m_pAC );
}

// パラメーターの設定.
void CEditAlienD::SetParamter( const SAlienParam& param )
{
	m_Paramter = param;
	// レーザーの移動速度の設定.
	m_pLaserBeam->SetMoveSpped( param.LaserMoveSpeed );
	// レーザーの麻痺時間の設定.
	m_pLaserBeam->SetParalysisTime( param.ParalysisTime );
}

// スポーン.
void CEditAlienD::Spawning()
{
	CEditAlien::Spawning();
}

// 移動.
void CEditAlienD::Move()
{
	CEditAlien::Move();
	Attack();
}

// 拐う.
void CEditAlienD::Abduct()
{
	CEditAlien::Abduct();
}

// 怯み.
void CEditAlienD::Fright()
{
	CEditAlien::Fright();
}

// 死亡.
void CEditAlienD::Death()
{
	CEditAlien::Death();
}

// 逃げる.
void CEditAlienD::Escape()
{
	CEditAlien::Escape();
}

// 攻撃関数.
void CEditAlienD::Attack()
{
	if( m_NowMoveState != alien::EMoveState::Attack ) return;

	const double attackSpeed = m_IsAttackStart == false ? m_AnimSpeed : -m_AnimSpeed;
	m_AttackCount += static_cast<float>(attackSpeed);	// 攻撃カウントの追加.

	if( m_AnimFrameList[m_NowAnimNo].NowFrame >= 0.7 ){
		m_IsAttackStart = true;

		// 相手への向きを取得.
		const float radius = atan2f(
			m_TargetPosition.x - m_vPosition.x,
			m_TargetPosition.z - m_vPosition.z );

		D3DXVECTOR3 headPos = m_vPosition;
		headPos.y += 15.0f;
		headPos.x += sinf( radius ) * 3.5f;
		headPos.z += cosf( radius ) * 3.5f;

		// 上向き少し後ろに設定..
		m_ControlPositions[0].x = headPos.x + sinf( radius ) * m_Paramter.ControlPointOneLenght;
		m_ControlPositions[0].y = headPos.y + m_Paramter.ControlPointOneLenghtY;
		m_ControlPositions[0].z = headPos.z + cosf( radius ) * m_Paramter.ControlPointOneLenght;

		// 上で設定したコントロールポジションを設定.
		m_pLaserBeam->SetControlPointList( m_ControlPositions );

		m_pLaserBeam->Shot( headPos );	// ビームを打つ.
		CSoundManager::NoMultipleSEPlay("AlienDAttack");
	}

	if( m_AnimFrameList[m_NowAnimNo].IsNowFrameOver() == false ) return;
	m_NowMoveState = alien::EMoveState::Wait;	// 待機状態へ遷移.
	SetAnimation( alien::EAnimNo_Move, m_pAC );
}

// 当たり判定の設定.
bool CEditAlienD::ColliderSetting()
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