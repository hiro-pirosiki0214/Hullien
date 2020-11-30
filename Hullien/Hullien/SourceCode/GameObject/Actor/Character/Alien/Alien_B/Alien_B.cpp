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
	: CAlienB	( nullptr )
{}

CAlienB::CAlienB( const SAlienParam* pParam )
	: CAlien			( pParam )
	, m_pAttackMesh		( nullptr )
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

// �������֐�.
bool CAlienB::Init()
{
	if( pPARAMETER					== nullptr ) return false;
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAttackModel()			== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( ColliderSetting()			== false ) return false;
	if( EffectSetting()				== false ) return false;
	if( m_pArm->Init()				== false ) return false;
	m_pSkinMesh->ChangeAnimSet_StartPos( alien::EAnimNo_Move, 0.0f, m_pAC );
	return true;
}

// �X�V�֐�.
void CAlienB::Update()
{
	// �A�j���[�V�����t���[���̍X�V.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	CurrentStateUpdate();	// ���݂̏�Ԃ̍X�V.
	// �A�[��.
	if( m_IsRisingMotherShip == false )
		m_pArm->SetPosition( {m_vPosition.x, m_vPosition.y+5.0f, m_vPosition.z} );		// ���W���Z�b�g.
	m_pArm->SetRotationY( m_vRotation.y );	// ��]�����Z�b�g.
	m_pArm->Update();						// �X�V.
}

// �`��֐�.
void CAlienB::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if( IsDisplayOut() == true ) return;
	if( m_pSkinMesh == nullptr ) return;
	if( m_pAttackMesh == nullptr ) return;

#if 0
	if( m_NowMoveState == EMoveState::Attack ){
		m_pAttackMesh->SetPosition( m_vPosition );
		m_pAttackMesh->SetRotation( m_vRotation );
		m_pAttackMesh->SetScale( m_vScale );
		m_pAttackMesh->SetColor( { 0.8f, 0.2f, 0.2f, 1.0f } );
		m_pAttackMesh->SetRasterizerState( ERS_STATE::Back );
		m_pAttackMesh->Render();
		m_pAttackMesh->SetRasterizerState( ERS_STATE::None );
	} else {
		m_pSkinMesh->SetPosition( m_vPosition );
		m_pSkinMesh->SetRotation( m_vRotation );
		m_pSkinMesh->SetScale( m_vScale );
		m_pSkinMesh->SetColor( { 0.8f, 0.2f, 0.2f, 1.0f } );
		m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
		m_pSkinMesh->SetRasterizerState( ERS_STATE::Back );
		m_pSkinMesh->Render( m_pAC );
		m_pSkinMesh->SetRasterizerState( ERS_STATE::None );
	}
#else 
	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetColor( { 0.8f, 0.2f, 0.2f, 1.0f } );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->SetRasterizerState( ERS_STATE::Back );
	m_pSkinMesh->Render( m_pAC );
	m_pSkinMesh->SetRasterizerState( ERS_STATE::None );
#endif

	m_pArm->Render();	// �A�[���̕`��.
#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �G�t�F�N�g�̕`��.
void CAlienB::EffectRender()
{
	// �q�b�g���̃G�t�F�N�g.
	m_pEffects[alien::EEffectNo_Hit]->SetScale( 2.0f );
	m_pEffects[alien::EEffectNo_Hit]->Render();

	// �X�|�[���G�t�F�N�g.
	m_pEffects[alien::EEffectNo_Spawn]->SetLocation( m_vPosition );
	m_pEffects[alien::EEffectNo_Spawn]->Render();

	// ���S�A�j���[�V����.
	m_pEffects[alien::EEffectNo_Dead]->Render();

	// �U�����̃G�t�F�N�g.
	m_pEffects[alien::EEffectNo_Attack]->SetLocation( m_vPosition );
	m_pEffects[alien::EEffectNo_Attack]->SetScale( 0.5f );
	m_pEffects[alien::EEffectNo_Attack]->Render();

}

// �����蔻��֐�.
void CAlienB::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// �v���C���[��_���Ă��邩.
	if( m_HasAimPlayer == true ){
		PlayerCollison( pActor );		// �v���C���[�Ƃ̓����蔻��.
		// �A�[�����Еt�����ĂȂ���ΕЕt����.
		if( m_pArm->IsCleanUp() == false ) m_pArm->SetCleanUpPreparation();
	} else {
		GirlCollision( pActor );		// ���̎q�Ƃ̓����蔻��.
	}
	BarrierCollision( pActor );			// �o���A�Ƃ̓����蔻��.
}

// �X�|�[��.
bool CAlienB::Spawn( const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if( m_NowState != alien::EAlienState::None ) return true;
	m_vPosition		= spawnPos;						// �X�|�[�����W�̐ݒ�.
	m_LifePoint		= pPARAMETER->LifeMax;			// �̗͂̐ݒ�.
	m_NowState		= alien::EAlienState::Spawn;	// ���݂̏�Ԃ��X�|�[���ɕύX.
	m_AnimSpeed		= 0.0;							// �A�j���[�V�������x���~�߂�.
	m_pEffects[alien::EEffectNo_Spawn]->Play( m_vPosition );
	return true;
}

// ������W�̐ݒ�.
void CAlienB::SetTargetPos( CActor& actor )
{
	// ���̎q�̍��W���擾.
	CAlien::SetTargetPos( actor );
	// �v���C���[�̍��W���擾.
	CAlienB::SetPlayerPos( actor );
}

// ���C�t�v�Z�֐�.
void CAlienB::LifeCalculation( const std::function<void(float&,bool&)>& proc )
{
	if( m_NowState == alien::EAlienState::Spawn ) return;
	if( m_NowState == alien::EAlienState::Death ) return;
	if( m_NowState == alien::EAlienState::Fright ) return;

	bool isAttack = false;
	proc( m_LifePoint, isAttack );
	m_NowState = alien::EAlienState::Fright;	// ���ݏ�Ԃ֑J��.
	SetAnimation( alien::EAnimNo_Damage, m_pAC );
	m_AnimSpeed = 0.01;
	m_pEffects[alien::EEffectNo_Attack]->Stop();
	m_pEffects[alien::EEffectNo_Hit]->Play( { m_vPosition.x, m_vPosition.y+4.0f, m_vPosition.z });
	if( m_pArm != nullptr ){
		// �A�[����Еt���Ă��Ȃ���ΕЕt����.
		if( m_pArm->IsCleanUp() == false ){
			m_pArm->SetCleanUp();
		}
	}

	if( m_LifePoint > 0.0f ) return;
	// �̗͂� 0.0�ȉ��Ȃ玀�S��Ԃ֑J��.
	m_NowState = alien::EAlienState::Death;
	m_NowMoveState = alien::EMoveState::Wait;
	m_pEffects[alien::EEffectNo_Spawn]->Play( m_vPosition );
	SetAnimation( alien::EAnimNo_Dead, m_pAC );
}

// �v���C���[���W�̐ݒ�.
void CAlienB::SetPlayerPos( CActor& actor )
{
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
	// �v���C���[��_���Ă��邩.
	if( m_HasAimPlayer == true ){
		CAlienB::VectorMove( m_MoveSpeed );	// �ړ�.
		Attack();				// �U��.
		CAlien::WaitMove();		// �ҋ@.
	} else {
		CAlien::VectorMove( m_MoveSpeed );		// �ړ�.
		CAlien::WaitMove();		// �ҋ@.
	}

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == alien::EAlienState::Abduct ) return;
	m_pEffects[alien::EEffectNo_Attack]->Stop();
	SetAnimation( alien::EAnimNo_Move, m_pAC );
	m_NowState		= alien::EAlienState::Escape;	// �������Ԃ֑J��.
	m_NowMoveState	= alien::EMoveState::Rotation;	// �ړ���Ԃ���]�֑J�ڂ���.
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
	if( m_NowMoveState != alien::EMoveState::Move ) return;

	// �x�N�g�����g�p���Ĉړ�.
	m_vPosition.x -= m_MoveVector.x * moveSpeed;
	m_vPosition.z -= m_MoveVector.z * moveSpeed;

	// �v���C���[�̍��W�ƉF���l�̍��W���r.
	if( D3DXVec3Length( &D3DXVECTOR3(m_vPlayerPos - m_vPosition) ) >= pPARAMETER->AttackLenght ) return;
	if( m_IsBarrierHit == true ) return;
	m_NowMoveState	= alien::EMoveState::Attack;
	m_RotAccValue	= pPARAMETER->AttackRotInitPower;
	m_pEffects[alien::EEffectNo_Attack]->Play( m_vPosition );
}

// �U���֐�.
void CAlienB::Attack()
{
	if( m_NowMoveState != alien::EMoveState::Attack ) return;

	// ��].
	m_vRotation.y += (pPARAMETER->AttackRotPower - fabsf(m_RotAccValue));
	m_RotAccValue -= pPARAMETER->AttackRotAddValue;	// ��]�����x�����Z.

	// �����x���ړ��͈͂Ȃ�ړ�.
	if( -pPARAMETER->AttackMoveRange <= m_RotAccValue && m_RotAccValue <= pPARAMETER->AttackMoveRange ){
		m_vPosition.x -= m_MoveVector.x * pPARAMETER->AttackMoveSpeed;
		m_vPosition.z -= m_MoveVector.z * pPARAMETER->AttackMoveSpeed;
	}
	if (m_IsAttackSE == false)
	{
		CSoundManager::NoMultipleSEPlay("AlienAttack");
		m_IsAttackSE = true;
	}

	if( m_RotAccValue > -pPARAMETER->AttackRotPower ) return;
	m_NowMoveState = alien::EMoveState::Wait;
	m_IsAttackSE = false;
}

// �v���C���[�Ƃ̓����蔻��.
void CAlienB::PlayerCollison( CActor* pActor )
{
	// �I�u�W�F�N�g�̃^�O���v���C���[����Ȃ���ΏI��.
	if( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if( m_NowMoveState != alien::EMoveState::Attack ) return;	// �U����Ԃ���Ȃ���ΏI��.
	if( m_NowState == alien::EAlienState::Death ) return;	// ���S���Ă�����I��.
	if( m_NowState == alien::EAlienState::Fright ) return;	// ���ݏ�ԂȂ�I��.
	
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetCollManager() ) == false ) return;
	// ���݂̈ړ��x�N�g����ݒ�.
	pActor->SetVector( m_MoveVector );
	// �v���C���[�̗̑͂����炷.
	pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	{
		life -= pPARAMETER->AttackPower;
		isAttack = true;
	});
}

// �v���C���[��_��������.
void CAlienB::AimPlayerDecision()
{
	if( m_NowMoveState == alien::EMoveState::Attack ) return;

	// �v���C���[�Ƃ̋������v�Z.
	float playerLenght = D3DXVec3Length( &D3DXVECTOR3(m_vPlayerPos - m_vPosition) );

	m_OldHasAimPlayer = m_HasAimPlayer;
	// �v���C���[�̑_���͈͂��r.
	if( playerLenght <= pPARAMETER->PlayerAimLenght ){
		// �v���C���[�̂ق����߂��̂Ńv���C���[��_��.
		m_HasAimPlayer = true;
		SetMoveVector( m_vPlayerPos );
	} else {
		// ���̎q�̂ق���_��.
		m_HasAimPlayer = false;
		SetMoveVector( m_TargetPosition );
	}
	if( m_OldHasAimPlayer == m_HasAimPlayer ) return;
	m_NowMoveState = alien::EMoveState::Rotation;
}

// �����蔻��̐ݒ�.
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
		pPARAMETER->SphereAdjPos,
		pPARAMETER->SphereAdjRadius ) )) return false;
	if( FAILED( m_pCollManager->InitCapsule( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		pPARAMETER->SphereAdjPos,
		-1.0f,
		0.0f ) )) return false;
	return true;
}

// �U�����f���̎擾.
bool CAlienB::GetAttackModel()
{
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_pAttackMesh != nullptr ) return true;
	// ���f���̎擾.
	CMeshResorce::GetStatic( m_pAttackMesh, ATTACK_MODEL_NAME );
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if( m_pAttackMesh == nullptr ) return false;
	return true;
}