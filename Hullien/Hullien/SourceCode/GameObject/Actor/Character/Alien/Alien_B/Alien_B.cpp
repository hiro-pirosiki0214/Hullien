#include "Alien_B.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"

CAlienB::CAlienB()
	: m_vPlayerPos		( 0.0f, 0.0f, 0.0f )
	, m_HasAimPlayer	( false )
	, m_OldHasAimPlayer	( false )
	, m_RotAccValue		( 0.0f )
{
	m_ObjectTag = EObjectTag::Alien_B;
}

CAlienB::~CAlienB()
{
}

// �������֐�.
bool CAlienB::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#else
	if( GetModel( MODEL_TEMP_NAME ) == false ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( ColliderSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CAlienB::Update()
{
	CurrentStateUpdate();	// ���݂̏�Ԃ̍X�V.
}

// �`��֐�.
void CAlienB::Render()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.5f, 0.8f, 0.5f, m_ModelAlpha } );
	m_pSkinMesh->SetBlend( true );
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
	m_pTempStaticMesh->SetColor( { 0.0f, 0.8f, 0.0f, m_ModelAlpha } );
	m_pTempStaticMesh->SetBlend( true );
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

// �����蔻��֐�.
void CAlienB::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
	PlayerCollison( pActor );		// �v���C���[�Ƃ̓����蔻��.
	GirlCollision( pActor );		// ���̎q�Ƃ̓����蔻��.
	BarrierCollision( pActor );
}

// �X�|�[��.
bool CAlienB::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if( m_NowState != EAlienState::None ) return true;
	// �������Ɏ��s������I��.
	if( Init() == false ) return false;
	m_Parameter = param;	// �p�����[�^��ݒ�.
	m_vPosition = spawnPos;	// �X�|�[�����W�̐ݒ�.
	m_vPosition.y += INIT_POSITION_ADJ_HEIGHT;
	m_NowState = EAlienState::Spawn;	// ���݂̏�Ԃ��X�|�[���ɕύX.
	return true;
}

// ������W�̐ݒ�.
void CAlienB::SetTargetPos( CActor& actor )
{
	// ���̎q�̍��W���擾.
	CAlien::SetTargetPos( actor );

	// �v���C���[����Ȃ���ΏI��.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_vPlayerPos = actor.GetPosition();	// �v���C���[�̍��W���擾.
}

// �X�|�[��.
void CAlienB::Spawning()
{
	CAlien::Spawning();
}

// �ړ�.
void CAlienB::Move()
{
	AimPlayerDecision();	// �v���C���[��_��������.
	TargetRotation();		// ��].
	if( m_HasAimPlayer == true ){
		CAlienB::VectorMove( m_Parameter.MoveSpeed );	// �ړ�.
		Attack();				// �U��.
		CAlien::WaitMove();		// �ҋ@.
	} else {
		CAlien::VectorMove( m_Parameter.MoveSpeed );		// �ړ�.
		CAlien::WaitMove();		// �ҋ@.
	}

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState = EAlienState::Escape;
	m_NowMoveState = EMoveState::Rotation;	// �ړ���Ԃ���]����.
}

// ����.
void CAlienB::Abduct()
{
	CAlien::Abduct();
}

// ����.
void CAlienB::Fright()
{
	CAlien::Fright();
}

// ���S.
void CAlienB::Death()
{
	CAlien::Death();
}

// ������.
void CAlienB::Escape()
{
	CAlien::Escape();
}

// �ړ��֐�.
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

// �U���֐�.
void CAlienB::Attack()
{
	if( m_NowMoveState != EMoveState::Attack ) return;

	// ��].
	m_vRotation.y += (m_Parameter.AttackRotPower - fabsf(m_RotAccValue));
	m_RotAccValue -= m_Parameter.AttackRotAddValue;	// ��]�����x�����Z.

	// �����x���ړ��͈͂Ȃ�ړ�.
	if( -m_Parameter.AttackMoveRange <= m_RotAccValue && m_RotAccValue <= m_Parameter.AttackMoveRange ){
		m_vPosition.x += m_MoveVector.x * m_Parameter.AttackMoveSpeed;
		m_vPosition.z += m_MoveVector.z * m_Parameter.AttackMoveSpeed;
	}

	if( m_RotAccValue > -m_Parameter.AttackRotPower ) return;
	m_NowMoveState = EMoveState::Wait;
}

// �v���C���[�Ƃ̓����蔻��.
void CAlienB::PlayerCollison( CActor* pActor )
{
	// �I�u�W�F�N�g�̃^�O���v���C���[����Ȃ���ΏI��.
	if( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if( m_NowMoveState != EMoveState::Attack ) return;	// �U����Ԃ���Ȃ���ΏI��.
	if( m_NowState == EAlienState::Death ) return;	// ���S���Ă�����I��.
	if( m_NowState == EAlienState::Fright ) return;	// ���ݏ�ԂȂ�I��.
	
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	// �v���C���[�̗̑͂����炷.
	pActor->LifeCalculation( [&]( float& life ){ life -= m_Parameter.AttackPower; });
}

// �v���C���[��_��������.
void CAlienB::AimPlayerDecision()
{
	if( m_NowMoveState == EMoveState::Attack ) return;

	// �v���C���[�Ƃ̋������v�Z.
	float playerLenght = D3DXVec3Length( &D3DXVECTOR3(m_vPlayerPos - m_vPosition) );

	m_OldHasAimPlayer = m_HasAimPlayer;
#if 1
	// �v���C���[�̑_���͈͂��r.
	if( playerLenght <= m_Parameter.PlayerAimLenght ){
		// �v���C���[�̂ق����߂��̂Ńv���C���[��_��.
		m_HasAimPlayer = true;
		SetMoveVector( m_vPlayerPos );
	} else {
		// ���̎q�̂ق���_��.
		m_HasAimPlayer = false;
		SetMoveVector( m_TargetPosition );
	}
#else
	// ���̎q�Ƃ̋������v�Z.
	float girlLenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	// ���̎q�ƃv���C���[�̋������r.
	if( playerLenght <= girlLenght ){
		// �v���C���[�̂ق����߂��̂Ńv���C���[��_��.
		m_HasAimPlayer = true;
		SetMoveVector( m_vPlayerPos );
	} else {
		// ���̎q�̂ق���_��.
		m_HasAimPlayer = false;
		SetMoveVector( m_TargetPosition );
	}
#endif // #if 0.
	if( m_OldHasAimPlayer == m_HasAimPlayer ) return;
	m_NowMoveState = EMoveState::Rotation;
}

// �����蔻��̐ݒ�.
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
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}