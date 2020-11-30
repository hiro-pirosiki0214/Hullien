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
	m_ObjectTag = EObjectTag::Alien_B;
}

CEditAlienB::~CEditAlienB()
{}

// �������֐�.
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

// �X�V�֐�.
void CEditAlienB::Update()
{
	// �A�j���[�V�����t���[���̍X�V.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	CurrentStateUpdate();				// ���݂̏�Ԃ̍X�V.
}

// �`��֐�.
void CEditAlienB::Render()
{
	// ��ʂ̊O�Ȃ�I��.
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

// �����蔻��֐�.
void CEditAlienB::Collision( CActor* pActor )
{}

// �U���̍Đ�.
void CEditAlienB::PlayAttack()
{
	if( m_IsPlaying == true ) return;
	m_RotAccValue	= m_Paramter.AttackRotInitPower;
	m_NowState = alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Attack;
	m_IsPlaying = true;
}

// �X�|�[��.
void CEditAlienB::Spawning()
{
	CEditAlien::Spawning();
}

// �ړ�.
void CEditAlienB::Move()
{
	CEditAlien::Move();
	Attack();
}

// ����.
void CEditAlienB::Abduct()
{
	CEditAlien::Abduct();
}

// ����.
void CEditAlienB::Fright()
{
	CEditAlien::Fright();
}

// ���S.
void CEditAlienB::Death()
{
	CEditAlien::Death();
}

// ������.
void CEditAlienB::Escape()
{
	CEditAlien::Escape();
}

// �U���֐�.
void CEditAlienB::Attack()
{
	if( m_NowMoveState != alien::EMoveState::Attack ) return;

	// ��].
	m_vRotation.y += (m_Paramter.AttackRotPower - fabsf(m_RotAccValue));
	m_RotAccValue -= m_Paramter.AttackRotAddValue;	// ��]�����x�����Z.

	// �����x���ړ��͈͂Ȃ�ړ�.
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

// �����蔻��̐ݒ�.
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