#include "STGEnemy.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\..\Common\Font\Font.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"

STG::CEnemy::CEnemy()
	: CEnemy	( STG::SEnemyParam() )
{
}

STG::CEnemy::CEnemy( const STG::SEnemyParam& param )
	: PARAMETER				( param )
	, m_pFont				( nullptr )
	, m_FontRotation		( FONT_ROTATION )
	, m_NowState			( STG::EEnemyState::Spawn )
	, m_MoveSpeed			( 0.0f )
	, m_MoveingDistance		( 0.0f )
	, m_MoveingDistanceMax	( 0.0f )
	, m_SpawnCount			( 0 )
	, m_ShotAngle			( 0.0f )
	, m_NowShotBulletCount	( 0 )
	, m_IsHitShake			( false )
	, m_ShakeCount			( SHAKE_COUNT_MAX )
	, m_EscapeCount			( ESCAPE_COUNT_MAX )
{
	m_pFont			= std::make_unique<CFont>();
	m_pCollManager	= std::make_shared<CCollisionManager>();
	m_vPosition.z	= INIT_POSITION_Z;
	m_vPosition.x	= PARAMETER.PositionX;
	m_vRotation		= { 0.0f, 0.0f, static_cast<float>(D3DXToRadian(90)) };
	m_MoveSpeed		= PARAMETER.MoveSpeed;
	m_LifePoint		= PARAMETER.LifePoint;
	m_vScale		= { PARAMETER.TextSize, PARAMETER.TextSize, PARAMETER.TextSize };
}

STG::CEnemy::~CEnemy()
{
}

// �������֐�.
bool STG::CEnemy::Init()
{
	if( CollisionInit()											== false ) return false;
	if( BulletInit( m_pBullets, PARAMETER.BulletCountMax, BULLET_MODEL_NAME ) == false )	return false;
	if( FAILED( m_pFont->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() ) ))	return false;
	// �Փˎ��A�e���������ǂ����ݒ�.
	if( PARAMETER.BulletCollDisappear == 1 ) for( auto& b : m_pBullets ) b->SetCollDisappear();
	return true;
}

// �X�V�֐�.
void STG::CEnemy::Update()
{
	BulletUpdate();	// �e�̍X�V.

	switch( m_NowState )
	{
	case STG::EEnemyState::Spawn:	Spawn();	break;	// �X�|�[��.
	case STG::EEnemyState::Move:	Move();		break;	// �ړ�.
	case STG::EEnemyState::Shot:	Shot();		break;	// �e������.
	case STG::EEnemyState::Escape:	Escape();	break;	// ������.
	case STG::EEnemyState::Dead:	Dead();		break;	// ���S.
	default:									break;
	}

	HitShake();		// �q�b�g���̗h��.
}

// �`��֐�.
void STG::CEnemy::Render()
{
	BulletRender( 
		{ 
			1.0f,
			PARAMETER.BulletCollDisappear*BULLET_COLOR, 
			PARAMETER.BulletCollDisappear*BULLET_COLOR
		} );	// �e�̕`��.

	m_pFont->SetColor( { 1.0f, 1.0f, 1.0f, 1.0f } );
	m_pFont->SetPosition( m_vPosition );
	m_pFont->SetRotation( m_FontRotation );
	m_pFont->SetScale( m_vScale );
	m_pFont->SetCoverage( true );
	m_pFont->Render( PARAMETER.Text );
	m_pFont->SetCoverage( false );

#ifdef _DEBUG
	m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
}

// �����蔻��.
void STG::CEnemy::Collision( STG::CActor* pActor )
{
	if( pActor->GetActive()	== false ) return;	// ���肪���삵�ĂȂ���ΏI��.
	// �e�̐����������蔻����s��.
	for( auto& b : m_pBullets ) b->Collision( pActor );

	if( m_IsActive			== false ) return;	// ���������삵�ĂȂ���ΏI��.
	// �J�v�Z���̓����蔻��.
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;
}

// �X�|�[��.
void STG::CEnemy::Spawn()
{
	m_SpawnCount++;
	if( m_SpawnCount < PARAMETER.SpawnTime ) return;
	m_NowState = STG::EEnemyState::Move;
	m_IsActive = true;
}


// �ړ��֐�.
void STG::CEnemy::Move()
{
	m_vPosition.z += m_MoveSpeed;
	if( m_vPosition.z >= MOVE_SUB_POSITION_Z ) m_MoveSpeed -= MOVE_SUB_VALUE;

	if( m_MoveSpeed > 0.0f ) return;

	m_NowState = STG::EEnemyState::Shot;

	// ����̊p�x���擾.
	m_ShotAngle = atan2(
		m_vPosition.x - m_TargetPositon.x,
		m_vPosition.z - m_TargetPositon.z );
	m_ShotAngle -=
		static_cast<float>(PARAMETER.AnyBulletCountMax-1) * 0.5f * PARAMETER.ShotAngle;	
}

// �e������.
void STG::CEnemy::Shot()
{
	m_ShotCount++;
	if( m_ShotCount != PARAMETER.ShotIntervalFrame ) return;

	float angle = m_ShotAngle;
	for( int i = 0; i < PARAMETER.AnyBulletCountMax; i++ ){
		if( m_NowShotBulletCount < PARAMETER.BulletCountMax ){
			STG::CCharacter::BulletShot( angle, PARAMETER.BulletSpeed );
			angle += PARAMETER.ShotAngle;
			m_NowShotBulletCount++;
		}
	}
	if( m_NowShotBulletCount == PARAMETER.BulletCountMax ){
		m_NowState = STG::EEnemyState::Escape;
	}
	m_ShotAngle += PARAMETER.BulletAngle;	// �p�x�̉��Z.
	m_ShotCount = 0;
}

// ������.
void STG::CEnemy::Escape()
{
	// �ړ����x����葬�x�ɂȂ�܂ŉ��Z.
	if( m_MoveSpeed < PARAMETER.MoveSpeed ) m_MoveSpeed += MOVE_SUB_VALUE;

	// �x�N�g�����g�p���Ĉړ�.
	m_vPosition.x += m_MoveVector.x * m_MoveSpeed;
	m_vPosition.z += m_MoveVector.z * m_MoveSpeed;

	m_EscapeCount--;	// ������J�E���g�̌��Z.
	// ������J�E���g�� 0.0 ���傫�����.
	if( m_EscapeCount > 0.0f ){
		m_MoveingDistance += m_MoveSpeed;	// ���������Z.
	}
	// �ړ���������苗���𒴓�����.
	if( m_MoveingDistance >= m_MoveingDistanceMax ){
		SearchRandomMoveVector();	// �ړ��x�N�g������������.
	}
	// ��ʊO�ɏo����.
	if( IsDisplayOut( E_WND_OUT_ADJ_SIZE ) == true ){
		m_NowState = STG::EEnemyState::Dead;	// ���S.
	}
}

// ���S.
void STG::CEnemy::Dead()
{
	m_IsActive	= false;		// ����I��.
	m_vScale.x -= DEAD_SCALE_SUB_VALUE;	// �X�P�[�������Z.
	m_vScale.y -= DEAD_SCALE_SUB_VALUE;	// �X�P�[�������Z.
	m_vScale.z -= DEAD_SCALE_SUB_VALUE;	// �X�P�[�������Z.
	m_FontRotation.z += DEAD_ROTATION_SPEED;	// ��]������.

	if( m_vScale.x > 0.0f ) return;
	// �X�P�[���� 0.0 �ȉ��ɂȂ��.
	m_NowState	= STG::EEnemyState::None;	// �����Ȃ���Ԃ֑J��.
	// ���W����ʊO��.
	m_vPosition	= { INIT_POSITION_Z, 0.0f, INIT_POSITION_Z };
}

// �����������̗h��.
void STG::CEnemy::HitShake()
{
	if( m_IsHitShake == false ) return;

	m_ShakeCount -= SHAKE_SUB_VALUE;	// �h��J�E���g�����Z.
	m_vPosition.x += sinf( static_cast<float>(D3DX_PI)		* m_ShakeCount ) * SHAKE_SPEED;
	m_vPosition.z += sinf( static_cast<float>(D3DX_PI)*0.5f * m_ShakeCount ) * SHAKE_SPEED;

	if( m_ShakeCount > 0.0f ) return;
	// �h��J�E���g�� 0.0 �ȉ��ɂȂ��.
	m_ShakeCount = SHAKE_COUNT_MAX;	// �h��J�E���g��������.
	m_IsHitShake = false;			// �q�b�g�t���O������.
}

// ���C�t�v�Z�֐�.
void STG::CEnemy::LifeCalculation( const std::function<void(float&)>& proc )
{
	// �������Ԃ̂ݍU�����󂯂�.
	if( m_NowState != STG::EEnemyState::Escape ){
		m_IsHitShake = true;	// �q�b�g�t���O�𗧂Ă�.
		return;
	}

	proc( m_LifePoint );
	m_IsHitShake = true;	// �q�b�g�t���O�𗧂Ă�.

	if( m_LifePoint > 0.0f ) return;
	// ���C�t��0�ȉ��ɂȂ��
	m_NowState = STG::EEnemyState::Dead;	// ���S��Ԃ֑J��.
	CSoundManager::PlaySE(DEAD_SE_NAME);
}

// �����_���ňړ��x�N�g��������.
void STG::CEnemy::SearchRandomMoveVector()
{
	// �ړ��x�N�g���������_������擾.
	m_MoveVector.x = sinf( static_cast<float>(rand()) );
	m_MoveVector.z = cosf( static_cast<float>(rand()) );

	// �ő�ړ������������_������擾.
	m_MoveingDistanceMax = fabsf(cosf( static_cast<float>(rand()) )) * ESCAPE_MOVE_DISTANCE;
	m_MoveingDistance = 0.0f;
	BulletShot( m_ShotAngle, PARAMETER.BulletSpeed );
	m_ShotAngle += PARAMETER.ShotAngle;	// �p�x�̉��Z.
}

// �����蔻��̍쐬.
bool STG::CEnemy::CollisionInit()
{
	float isLeadByte = 2.0f;
	if( IsDBCSLeadByte( PARAMETER.Text[0] ) == TRUE ){
		isLeadByte = 1.0f;
	}
	if( FAILED( m_pCollManager->InitCapsule(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{0.0f, 0.0f, 0.0f},
		PARAMETER.TextSize,
		PARAMETER.TextSize*isLeadByte*static_cast<float>(PARAMETER.Text.length()) ))) return false;
	return true;
}