#include "STGPlayer.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"

STG::CPlayer::CPlayer()
	: m_Direction		( 0.0f, 0.0f, 0.0f )
	, m_SpawnMoveSpeed	( MOVE_SPEED )
	, m_IsDead			( false )
{
	m_pCollManager	= std::make_shared<CCollisionManager>();
	m_vPosition		= INIT_POSITION;
	m_LifePoint		= LIFE_POINT_MAX;
}

STG::CPlayer::~CPlayer()
{
}

// �������֐�.
bool STG::CPlayer::Init()
{
	if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME )	== false ) return false;
	if( CollisionInit()											== false ) return false;
	if( BulletInit( m_pBullets,BULLET_COUNT_MAX, BULLET_MODEL_NAME )	== false ) return false;
	for( auto& b : m_pBullets ) b->SetCollDisappear();	// ���������e�������邩�ǂ����̐ݒ�.
	return true;
}

// �X�V�֐�.
void STG::CPlayer::Update()
{
	SpawnMove();	// �X�|�[���ړ�.
	Move();			// �ړ�.
	DeadUpdate();	// ���S����.
	BulletUpdate();	// �e�̍X�V.
}

// �`��֐�.
void STG::CPlayer::Render()
{
	if( m_pStaticMesh == nullptr ) return;
	MeshRender();					// ���b�V���̕`��.
	BulletRender( BULLET_COLOR );	// �e�̕`��.

#ifdef _DEBUG
	m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
}

// �����蔻��.
void STG::CPlayer::Collision( STG::CActor* pActor )
{
	if( m_IsActive			== false ) return;	// ���������삵�ĂȂ���ΏI��.
	if( pActor->GetActive()	== false ) return;	// ���肪���삵�ĂȂ���ΏI��.

	// �e�̐����������蔻����s��.
	for( auto& b : m_pBullets ) b->Collision( pActor );
	// �J�v�Z���̓����蔻��.
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;
}

// �X�|�[���ړ�.
void STG::CPlayer::SpawnMove()
{
	if( m_IsActive == true ) return;
	m_vPosition.z -= m_SpawnMoveSpeed;
	if( m_vPosition.z <= SPAWN_END_POS_Z ) m_SpawnMoveSpeed -= SPAWN_SPEED_SUB;
	if( m_SpawnMoveSpeed > 0.0f ) return;
	m_IsActive = true;
}

// �ړ��֐�.
void STG::CPlayer::Move()
{
	if( m_IsActive == false ) return;
	if( m_IsDead == true ) return;
	m_vPosition.x -= m_MoveVector.x * MOVE_SPEED;
	m_vPosition.z -= m_MoveVector.z * MOVE_SPEED;
	OutDispMove();	// ��ʊO�ɍs�����ۂ̏���.
}

// ���S�㏈��.
void STG::CPlayer::DeadUpdate()
{
	if( m_IsDead == false ) return;

	m_vScale.x -= DEAD_SPEED;
	m_vScale.y -= DEAD_SPEED;
	m_vScale.z -= DEAD_SPEED;
	m_vRotation.y += DEAD_SPEED;

	if( m_vScale.x > 0.0f ) return;
	m_vRotation.y		= 0.0f;
	m_vScale			= { 1.0f, 1.0f, 1.0f };
	m_IsDead			= false;
	m_IsActive			= false;
	m_SpawnMoveSpeed	= MOVE_SPEED;
	m_LifePoint			= LIFE_POINT_MAX;
	m_vPosition			= INIT_POSITION;
}

// ����֐�.
void STG::CPlayer::Controller()
{
	if( m_IsActive == false ) return;

	// �R���g���[���[��L�X�e�B�b�N�̌X�����擾.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());
	// �R���g���[���[��R�X�e�B�b�N�̌X�����擾.
	m_Direction.x = static_cast<float>(CXInput::RThumbX_Axis());
	m_Direction.z = static_cast<float>(CXInput::RThumbY_Axis());

	if( GetAsyncKeyState(VK_UP) & 0x8000 )		m_MoveVector.z = IDLE_THUMB_MAX;
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MIN;
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MAX;
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MIN;

	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ){
		m_MoveVector = { 0.0f, 0.0f, 0.0f };
	} else {
		// �m�[�}���C�Y.
		D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );
	}
	// �e�l���L���͈͓��Ȃ�.
	if( m_Direction.x >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_Direction.x ||
		m_Direction.z >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_Direction.z ){
		m_vRotation.y = atan2( m_Direction.x, m_Direction.z );	// ��]�l���擾.
	}

	// �e��������.
	ShotController();
}

// �e��������֐�.
void STG::CPlayer::ShotController()
{
	// �������u�Ԃ�ShotCount�������E�e������.
	if( CXInput::R_Button() == CXInput::enPRESSED_MOMENT ){
		m_ShotCount = 0;
		if( BulletShot( m_vRotation.y, BULLET_MOVE_SPEED ) == true ){
			CSoundManager::PlaySE(SHOT_SE_NAME);
		}
	}
	// �������̏ꍇ�e�����EShotCount�̉��Z.
	if( CXInput::R_Button() == CXInput::enPRESS_AND_HOLD || ( GetAsyncKeyState('Z') & 0x8000 )){
		m_ShotCount++;
		if( m_ShotCount == SHOT_INTERVAL_FRAME ){
			if( BulletShot( m_vRotation.y, BULLET_MOVE_SPEED ) == true ){
				CSoundManager::PlaySE(SHOT_SE_NAME);
			}
			m_ShotCount = 0;
		}
	}
}

// ���C�t�v�Z�֐�.
void STG::CPlayer::LifeCalculation( const std::function<void(float&)>& proc )
{
	proc( m_LifePoint );
	CSoundManager::PlaySE(HIT_SE_NAME);

	if( m_LifePoint > 0.0f ) return;
	m_IsDead = true;
}

// ��ʊO�ɍs�������̏���.
void STG::CPlayer::OutDispMove()
{
	if( OUT_POSITION_X < m_vPosition.x || m_vPosition.x < -OUT_POSITION_X ){
		m_vPosition.x += m_MoveVector.x * MOVE_SPEED;
	}
	if( OUT_POSITION_Z < m_vPosition.z || m_vPosition.z < -OUT_POSITION_Z ){
		m_vPosition.z += m_MoveVector.z * MOVE_SPEED;
	}
}

// �����蔻��̍쐬.
bool STG::CPlayer::CollisionInit()
{
	if( FAILED( m_pCollManager->InitCapsule(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{0.0f, 0.0f, 0.0f},
		2.0f,
		2.0f ))) return false;
	return true;
}