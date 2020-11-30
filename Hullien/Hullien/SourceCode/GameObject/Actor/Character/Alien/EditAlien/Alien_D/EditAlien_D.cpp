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
	m_ObjectTag = EObjectTag::Alien_B;
	m_pLaserBeam = std::make_unique<CLaserBeam>();
}

CEditAlienD::~CEditAlienD()
{}

// �������֐�.
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

// �X�V�֐�.
void CEditAlienD::Update()
{
	// �A�j���[�V�����t���[���̍X�V.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	CurrentStateUpdate();				// ���݂̏�Ԃ̍X�V.
	m_pLaserBeam->Update();
}

// �`��֐�.
void CEditAlienD::Render()
{
	m_pLaserBeam->Render();	// ���[�U�[�̕`��.
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
void CEditAlienD::Collision( CActor* pActor )
{}

// �U���̍Đ�.
void CEditAlienD::PlayAttack()
{
	if( m_IsPlaying == true ) return;
	m_IsAttackStart	= false;	// �U�����n�܂�t���O������.
	m_AttackCount	= 0.0f;		// �U���J�E���g��������,
	m_NowMoveState	= alien::EMoveState::Attack;	// �U����Ԃ֑J��.
	m_pLaserBeam->SetTargetPos( { 3.0f, 0.0f, 3.0f } );	// �v���C���[�̍��W���擾.
	SetAnimation( alien::EAnimNo_Attack, m_pAC );
	m_IsPlaying = true;
}

// �p�����[�^�[�̐ݒ�.
void CEditAlienD::SetParamter( const SAlienParam& param )
{
	m_Paramter = param;
	// ���[�U�[�̈ړ����x�̐ݒ�.
	m_pLaserBeam->SetMoveSpped( param.LaserMoveSpeed );
	// ���[�U�[�̖�჎��Ԃ̐ݒ�.
	m_pLaserBeam->SetParalysisTime( param.ParalysisTime );
}

// �X�|�[��.
void CEditAlienD::Spawning()
{
	CEditAlien::Spawning();
}

// �ړ�.
void CEditAlienD::Move()
{
	CEditAlien::Move();
	Attack();
}

// ����.
void CEditAlienD::Abduct()
{
	CEditAlien::Abduct();
}

// ����.
void CEditAlienD::Fright()
{
	CEditAlien::Fright();
}

// ���S.
void CEditAlienD::Death()
{
	CEditAlien::Death();
}

// ������.
void CEditAlienD::Escape()
{
	CEditAlien::Escape();
}

// �U���֐�.
void CEditAlienD::Attack()
{
	if( m_NowMoveState != alien::EMoveState::Attack ) return;

	const double attackSpeed = m_IsAttackStart == false ? m_AnimSpeed : -m_AnimSpeed;
	m_AttackCount += static_cast<float>(attackSpeed);	// �U���J�E���g�̒ǉ�.

	if( m_AnimFrameList[m_NowAnimNo].NowFrame >= 0.7 ){
		m_IsAttackStart = true;

		// ����ւ̌������擾.
		const float radius = atan2f(
			m_TargetPosition.x - m_vPosition.x,
			m_TargetPosition.z - m_vPosition.z );

		D3DXVECTOR3 headPos = m_vPosition;
		headPos.y += 15.0f;
		headPos.x += sinf( radius ) * 3.5f;
		headPos.z += cosf( radius ) * 3.5f;

		// ������������ɐݒ�..
		m_ControlPositions[0].x = headPos.x + sinf( radius ) * m_Paramter.ControlPointOneLenght;
		m_ControlPositions[0].y = headPos.y + m_Paramter.ControlPointOneLenghtY;
		m_ControlPositions[0].z = headPos.z + cosf( radius ) * m_Paramter.ControlPointOneLenght;

		// ��Őݒ肵���R���g���[���|�W�V������ݒ�.
		m_pLaserBeam->SetControlPointList( m_ControlPositions );

		m_pLaserBeam->Shot( headPos );	// �r�[����ł�.
		CSoundManager::NoMultipleSEPlay("AlienDAttack");
	}

	if( m_AnimFrameList[m_NowAnimNo].IsNowFrameOver() == false ) return;
	m_NowMoveState = alien::EMoveState::Wait;	// �ҋ@��Ԃ֑J��.
	SetAnimation( alien::EAnimNo_Move, m_pAC );
	m_IsPlaying = false;
}

// �����蔻��̐ݒ�.
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