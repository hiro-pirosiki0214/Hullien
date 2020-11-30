#include "Alien_D.h"
#include "..\..\..\LaserBeam\LaserBeam.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Common\Effect\EffectManager.h"

CAlienD::CAlienD()
	: CAlienD	( nullptr )
{}

CAlienD::CAlienD( const SAlienParam* pParam )
	: CAlien				( pParam )
	, m_pAttackRangeSprite	( nullptr )
	, m_pLaserBeam			( nullptr )
	, m_ControlPositions	( 1 )
	, m_AttackCount			( 0.0f )
	, m_IsAttackStart		( false )
{
	m_ObjectTag = EObjectTag::Alien_D;
	m_pLaserBeam = std::make_unique<CLaserBeam>();
}

CAlienD::~CAlienD()
{
}

// �������֐�.
bool CAlienD::Init()
{
	if( pPARAMETER					== nullptr ) return false;
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( GetSprite( SPRITE_NAME )	== false ) return false;
	if( EffectSetting()				== false ) return false;
	if( ColliderSetting()			== false ) return false;
	if( EffectSetting()				== false ) return false;
	if( m_pLaserBeam->Init()		== false ) return false;

	m_pSkinMesh->ChangeAnimSet_StartPos( alien::EAnimNo_Move, 0.0, m_pAC );
	return true;
}

// �X�V�֐�.
void CAlienD::Update()
{
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	SetMoveVector( m_TargetPosition );	// �ړI�̃x�N�g�����擾.
	m_pLaserBeam->Update();	// ���[�U�[�̍X�V.
	CurrentStateUpdate();	// ���݂̏�Ԃ̍X�V.
}

// �`��֐�.
void CAlienD::Render()
{
	m_pLaserBeam->Render();	// ���[�U�[�̕`��.
	// ��ʂ̊O�Ȃ�I��.
	if( IsDisplayOut() == true ) return;
	ModelRender();			// ���f���̕`��.

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �G�t�F�N�g�̕`��.
void CAlienD::EffectRender()
{
	// �q�b�g���̃G�t�F�N�g.
	m_pEffects[alien::EEffectNo_Hit]->SetScale( 2.0f );
	m_pEffects[alien::EEffectNo_Hit]->Render();

	// �X�|�[���G�t�F�N�g.
	m_pEffects[alien::EEffectNo_Spawn]->SetLocation( m_vPosition );
	m_pEffects[alien::EEffectNo_Spawn]->SetScale( 5.0f );
	m_pEffects[alien::EEffectNo_Spawn]->Render();

	// ���S�G�t�F�N�g.
	m_pEffects[alien::EEffectNo_Dead]->Render();

	// ���[�U�[�G�t�F�N�g.
	m_pLaserBeam->EffectRender();
}

// �����蔻��֐�.
void CAlienD::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	AttackCollision( pActor );	// �U��(���[�U�[)�̓����蔻��.
	BarrierCollision( pActor );	// �o���A�̓����蔻��.
}

// �X�|�[��.
bool CAlienD::Spawn( const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if( m_NowState != alien::EAlienState::None ) return true;
	m_vPosition		= spawnPos;					// �X�|�[�����W�̐ݒ�.
	m_LifePoint		= pPARAMETER->LifeMax;		// �̗͂̐ݒ�.
	m_NowState = alien::EAlienState::Spawn;		// ���݂̏�Ԃ��X�|�[���ɕύX.
	m_pEffects[alien::EEffectNo_Spawn]->Play( m_vPosition );
	// ���[�U�[�̈ړ����x�̐ݒ�.
	m_pLaserBeam->SetMoveSpped( pPARAMETER->LaserMoveSpeed );
	// ���[�U�[�̖�჎��Ԃ̐ݒ�.
	m_pLaserBeam->SetParalysisTime( pPARAMETER->ParalysisTime );

	if (CSoundManager::GetIsPlaySE("AlienDApp", 0) == false)
	{
		CSoundManager::NoMultipleSEPlay("AlienDApp");
	}
	return true;
}

// �X�v���C�g�̕`��.
void CAlienD::SpriteRender()
{
	AttackRangeSpriteRender();
}

// ���f���̕`��.
void CAlienD::ModelRender()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetColor( { 0.8f, 0.8f, 0.2f, 1.0f } );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->SetRasterizerState( ERS_STATE::Back );
	m_pSkinMesh->Render( m_pAC );
	m_pSkinMesh->SetRasterizerState( ERS_STATE::None );
}

// �U���͈͂̃X�v���C�g�`��.
void CAlienD::AttackRangeSpriteRender()
{
	if( m_pAttackRangeSprite == nullptr ) return;
	if( m_NowMoveState != alien::EMoveState::Attack ) return;

	D3DXVECTOR4 color;
	if( m_IsAttackStart == true ){
		// �U���̐F (��).
		color	= ATTACK_RANGE_DANGER_COLOR;
		color.w = m_AttackCount;
	} else {
		// �U���̗\���̐F (��).
		color	= ATTACK_RANGE_COLOR;
		color.w = m_AttackCount;
	}

	// �U���͈̓X�v���C�g�̕`��.
	m_pAttackRangeSprite->SetPosition( { m_TargetPosition.x, pPARAMETER->AttackRangeSpritePosY, m_TargetPosition.z } );
	m_pAttackRangeSprite->SetRotation( { static_cast<float>(D3DXToRadian(90)), 0.0f, 0.0f } );
	m_pAttackRangeSprite->SetScale( pPARAMETER->AttackRangeSpriteScale );	
	m_pAttackRangeSprite->SetColor( color );
	m_pAttackRangeSprite->SetBlend( true );
	m_pAttackRangeSprite->SetRasterizerState( ERS_STATE::Back );
	m_pAttackRangeSprite->Render();
	m_pAttackRangeSprite->SetRasterizerState( ERS_STATE::None );
	m_pAttackRangeSprite->SetBlend( false );
}

// �X�|�[��.
void CAlienD::Spawning()
{
	CAlien::Spawning();
}

// �ړ�.
void CAlienD::Move()
{
	TargetRotation();		// ��].
	CAlienD::VectorMove( m_MoveSpeed );	// �ړ�.
	Attack();				// �U��.
	CAlien::WaitMove();		// �ҋ@.

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == alien::EAlienState::Abduct ) return;
	SetAnimation( alien::EAnimNo_Move, m_pAC );
	m_NowState		= alien::EAlienState::Escape;	// �������Ԃ֑J��.
	m_NowMoveState	= alien::EMoveState::Rotation;	// �ړ���Ԃ���]����.
}

// ����.
void CAlienD::Abduct()
{
	// ����Ȃ��̂ŏ���������Ȃ�.
}

// ����.
void CAlienD::Fright()
{
	CAlien::Fright();
}

// ���S.
void CAlienD::Death()
{
	CAlien::Death();
}

// ������.
void CAlienD::Escape()
{
	CAlien::Escape();
}

// �U���֐�.
void CAlienD::Attack()
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
		m_ControlPositions[0].x = headPos.x + sinf( radius ) * pPARAMETER->ControlPointOneLenght;
		m_ControlPositions[0].y = headPos.y + pPARAMETER->ControlPointOneLenghtY;
		m_ControlPositions[0].z = headPos.z + cosf( radius ) * pPARAMETER->ControlPointOneLenght;

		// ��Őݒ肵���R���g���[���|�W�V������ݒ�.
		m_pLaserBeam->SetControlPointList( m_ControlPositions );

		m_pLaserBeam->Shot( headPos );	// �r�[����ł�.
		CSoundManager::NoMultipleSEPlay("AlienDAttack");
	}

	if( m_AnimFrameList[m_NowAnimNo].IsNowFrameOver() == false ) return;
	m_NowMoveState = alien::EMoveState::Wait;	// �ҋ@��Ԃ֑J��.
	SetAnimation( alien::EAnimNo_Move, m_pAC );
}

// �ړ��֐�.
void CAlienD::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != alien::EMoveState::Move ) return;

	// �x�N�g�����g�p���Ĉړ�.
	m_vPosition.x -= m_MoveVector.x * moveSpeed;
	m_vPosition.z -= m_MoveVector.z * moveSpeed;

	// �ēx���W���������A��]���邩��r.
	if( D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition-m_vPosition)) >= pPARAMETER->ResearchLenght ){
		m_NowMoveState	= alien::EMoveState::Rotation;	// ��]��Ԃ֑J��.
		m_IsAttackStart = false;	// �U�����n�܂�t���O������.
		return;
	}

	// �v���C���[�Ƃ̋��������l���Ⴂ����r.
	if( D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition-m_vPosition)) >= pPARAMETER->AttackLenght ) return;
	if( m_pLaserBeam->IsEndAttack() == false ) return;
	m_IsAttackStart	= false;	// �U�����n�܂�t���O������.
	m_AttackCount	= 0.0f;		// �U���J�E���g��������,
	m_NowMoveState	= alien::EMoveState::Attack;	// �U����Ԃ֑J��.
	SetAnimation( alien::EAnimNo_Attack, m_pAC );
}

// ������W�̐ݒ�.
void CAlienD::SetTargetPos( CActor& actor )
{
	SetPlayerPos( actor );	// �v���C���[���W�̐ݒ�.
}

// �v���C���[���W�̐ݒ�.
void CAlienD::SetPlayerPos( CActor& actor )
{
	// �v���C���[����Ȃ���ΏI��.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_pLaserBeam->SetTargetPos( actor );	// �v���C���[�̍��W���擾.

	// �U�����n�܂��Ă���ꍇ�I��.
	if( m_IsAttackStart == true ) return;
	m_TargetPosition = actor.GetPosition();	// �v���C���[�̍��W���擾.
}

// �U���̓����蔻��.
void CAlienD::AttackCollision( CActor* pActor  )
{
	if( m_pLaserBeam == nullptr ) return;
	m_pLaserBeam->Collision( pActor );	// ���[�U�[�̓����蔻��.
}

// �X�v���C�g�̎擾.
bool CAlienD::GetSprite( const char* spriteName )
{
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_pAttackRangeSprite != nullptr ) return true;
	// �X�v���C�g�̎擾.
	m_pAttackRangeSprite = CSpriteResource::GetSprite( spriteName );
	// �X�v���C�g���ǂݍ��߂ĂȂ���� false.
	if( m_pAttackRangeSprite == nullptr ) return false;
	return true;
}

// �����蔻��̐ݒ�.
bool CAlienD::ColliderSetting()
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
	return true;
}