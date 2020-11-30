#include "Girl.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\..\Utility\FileManager\FileManager.h"

CGirl::CGirl()
	: m_Parameter			()
	, m_pSearchCollManager	( nullptr )
	, m_pWarning			( nullptr )
	, m_OldPosition			( 0.0f, 0.0f, 0.0f )
	, m_NowState			( ENowState::None )
	, m_NowMoveState		( EMoveState::None )
	, m_CameraRadianX		( 0.0f )
	, m_IsDanger			( false )
	, m_IsOnlyFirst			( false )
{
	m_ObjectTag		= EObjectTag::Girl;
	m_NowState		= ENowState::Protected;
	m_NowMoveState	= EMoveState::Wait;
	m_NowAnimNo		= EAnimNo_Wait;	// ���݂̃A�j���[�V������ҋ@�ɐݒ�.
	m_OldAnimNo		= EAnimNo_None;	// �ߋ��̃A�j���[�V�����͖���.
	m_pSearchCollManager = std::make_shared<CCollisionManager>();
	m_pWarning		= std::make_unique<CWarning>();
	m_AnimFrameList.resize( EAnimNo_Max );
	m_vPosition.z = 4.0f;
}

CGirl::~CGirl()
{
}

// �������֐�.
bool CGirl::Init()
{
	if( GetModel( MODEL_NAME )	== false ) return false;
	if( SetAnimFrameList()		== false ) return false;
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_Parameter ) == false ) return false;
	if( ColliderSetting()		== false ) return false;
	if( m_pWarning->Init()		== false ) return false;
	
	// �ҋ@�A�j���[�V�����ɕύX.
	m_pSkinMesh->ChangeAnimSet_StartPos( EAnimNo_Wait, 0.0 );

	return true;
}

// �X�V�֐�.
void CGirl::Update()
{
	switch( m_NowState )
	{
	case ENowState::None:
		break;
	case ENowState::Protected:
		break;
	case ENowState::Abduct:
		Abduct();
		break;
	case ENowState::Move:
		Move();
		break;
	default:
		break;
	}

	if (m_NowState == ENowState::Abduct || m_IsDanger == true)
	{
		m_pWarning->Update();
		m_pWarning->SetPosition(m_vPosition);
	}

	// �T�E���h.
	Sound();

	//�x��.
	WarningRotation();

	m_IsDanger = false;
}

// �`��֐�.
void CGirl::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if( IsDisplayOut() == true ) return;

	MeshRender();	// ���b�V���̕`��.

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
	if( m_pSearchCollManager == nullptr ) return;
	m_pSearchCollManager->DebugRender();

//	DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CGirl::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
	SearchCollision( pActor );
}

// ������W�̐ݒ�֐�.
void CGirl::SetPosition( const D3DXVECTOR3& pos )
{
	m_vPosition = pos;
	SetAnimationBlend( EAnimNo_Abduct );
	m_NowState = ENowState::Abduct;
}

// �X�v���C�g�̕`��.
void CGirl::SpriteRender()
{
	// ���̎q���A�ꋎ���Ă����Ԃ܂��͊댯�ȏ�ԂȂ�Όx����`��.
	if (m_NowState == ENowState::Abduct || m_IsDanger == true)
	{
		m_pWarning->SetPosition({ m_vPosition.x, m_vPosition.y+5.0f, m_vPosition.z});
		m_pWarning->Update();
		m_pWarning->Render();
	}
}

// �ړ��֐�.
void CGirl::Move()
{
	switch( m_NowMoveState )
	{
	case EMoveState::None:
		break;
	case EMoveState::Rotation:
		CGirl::TargetRotation();
		break;
	case EMoveState::Move:
		TargetMove();
		break;
	case EMoveState::Wait:
		break;
	default:
		break;
	}
}

// �A�ꋎ��Ă���.
void CGirl::Abduct()
{
	// �O��̍��W�����̍��W�Ȃ�(�A�ꋎ��Ă����Ԃňړ����Ă��Ȃ�).
	if( m_OldPosition == m_vPosition ){
		m_NowState		= ENowState::Move;		// �ړ���Ԃ֑J��.
		m_NowMoveState	= EMoveState::Rotation;	// �ړ��̉�]��Ԃ֑J��.
	}
	m_OldPosition = m_vPosition;	// ���W���L��.
}

// �ړI�̏ꏊ�Ɍ����ĉ�].
void CGirl::TargetRotation()
{
	if( m_NowMoveState != EMoveState::Rotation ) return;
	// �ړI�̉�]�����擾.
	const float targetRotation = atan2f(
		m_vPosition.x - m_Parameter.InitPosition.x,
		m_vPosition.z - m_Parameter.InitPosition.z );
	// ��]������x�N�g�����擾.
	m_MoveVector.x = sinf( targetRotation );
	m_MoveVector.z = cosf( targetRotation );
	// �ړI�̍��W�Ɍ����ĉ�].
	if( CCharacter::TargetRotation( m_MoveVector, m_Parameter.RotatlonalSpeed, TOLERANCE_RADIAN ) == false ) return;
	m_vRotation.y = targetRotation;		// �^�[�Q�b�g�ւ̉�]�擾.
	m_NowMoveState = EMoveState::Move;	// �ړ���Ԃ֑J��.
	SetAnimationBlend( EAnimNo_Move );
}

// �ړI�̏ꏊ�Ɍ����Ĉړ�.
void CGirl::TargetMove()
{
	if( m_NowMoveState != EMoveState::Move ) return;

	// �ړ��x�N�g�����g�p���Ĉړ�.
	m_vPosition.x -= m_MoveVector.x * m_Parameter.MoveSpeed;
	m_vPosition.z -= m_MoveVector.z * m_Parameter.MoveSpeed;
	const float lenght = D3DXVec3Length( &D3DXVECTOR3(m_Parameter.InitPosition-m_vPosition) );
	// �ړI�̍��W�Ƃ̋��������l��菭�Ȃ�����r.
	if( lenght >= m_Parameter.InitPosLenght ) return;
	SetAnimationBlend( EAnimNo_Wait );
	m_NowMoveState = EMoveState::Wait;	// �ҋ@��Ԃ֑J��.
}

// ���G�̓����蔻��.
void CGirl::SearchCollision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pSearchCollManager == nullptr ) return;
	if( m_pSearchCollManager->GetSphere() == nullptr ) return;

	// ���ɘA�ꋎ���Ă�����I��.
	if( m_NowState == ENowState::Abduct ){
		m_IsDanger = true;
		return;
	}

	// �ΏۃI�u�W�F�N�g����Ȃ���ΏI��.
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	// ���̂̓����蔻��.
	if (m_pSearchCollManager->IsShereToShere(pActor->GetCollManager()) == false) return;
	m_IsDanger = true;

	// �G�����������ꍇ�A�Փ˂��Ă��Ă��A
	// �Փ˂��Ă��Ȃ��G�������ꍇ�A
	// false ���㏑������Ă��܂��̂ŁA
	// �����蔻����s���O�� Update�ŁA
	// ���������Ă�����.
//	{
//		m_IsDanger = false;
//	}
//	else {
//		m_IsDanger = true;
//	}
}

// �����蔻��̍쐬.
bool  CGirl::ColliderSetting()
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
	// ���G�̓����蔻��.
	if( FAILED( m_pSearchCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SearchCollRadius ) )) return false;
	return true;
}

// �A�j���[�V�����t���[���̐ݒ�.
bool CGirl::SetAnimFrameList()
{
	// �����p�A�j���[�V�����t���[���̃��X�g��p��.
	const double animAdjFrames[] =
	{
		0.0f,
		0.0f,
		0.0f,
	};
	if( m_pSkinMesh == nullptr ) return false;
	for( int i = EAnimNo_Begin; i < EAnimNo_End; i++ ){
		m_AnimFrameList.at(i) = { 0.0, m_pSkinMesh->GetAnimPeriod(i)-animAdjFrames[i] };
	}
	return true; 
}

// �T�E���h.
void CGirl::Sound()
{
	// ���̎q���댯�ȏ�ԂȂ��SE��炷.
	if (m_IsDanger == true && m_IsOnlyFirst == false)
	{
		CSoundManager::NoMultipleSEPlay("Warning");
		m_IsOnlyFirst = true;
	}
	// �댯�ȏ�Ԃ�E������SE���ēx�点���Ԃɂ���.
	if (m_IsDanger == false)
	{
		m_IsOnlyFirst = false;
	}
}

// �댯���̉�].
void CGirl::WarningRotation()
{
	// ���̎q���댯�ȏ�ԂłȂ���Ώ������Ȃ�.
	if (m_IsDanger == false) return;

	// �J�������珗�̎q�ւ̉�]�����擾.
	const float targetRotation = atan2f(
		m_vPosition.x - CCameraManager::GetPosition().x,
		m_vPosition.z - CCameraManager::GetPosition().z);
	D3DXVECTOR3 MoveVector = { 0.0f, 0.0f, 0.0f };
	MoveVector.x = sinf( targetRotation );
	MoveVector.z = cosf( targetRotation );

	// �J�����̑O�x�N�g����p��.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf(m_CameraRadianX);
	myVector.z = cosf(m_CameraRadianX);

	// �x�N�g���̒��������߂�.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(MoveVector.x*MoveVector.x + MoveVector.z*MoveVector.z);

	// �J�������珗�̎q�ւ̃x�N�g���ƁA�J�����̑O�x�N�g���̊O�ς����߂�.
	float cross = myVector.x * MoveVector.z - myVector.z * MoveVector.x;
	float dot = myVector.x * MoveVector.x + myVector.z * MoveVector.z;
	dot = acosf(dot / (myLenght * targetLenght));

	// ���ς��������l�𒴂����珗�̎q����ʊO�ɏo��.
	bool IsOffScreen = dot < THRESHOLD_VALUE_RADIAN ? true : false;
	m_pWarning->SetIsGirlOffScreen( IsOffScreen );

	// �O�ς�0.0f���傫����� �E�F�� �ɕ\��.
	bool IsRight = cross > 0.0f ? true : false;
	m_pWarning->SetIsDispRight( IsRight );
}

// �f�o�b�O�`��֐�.
void CGirl::DebugRender()
{
	const float pos_y = 40.0f;
	const float pos_x = 880.0f;
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale()*10, 0.0f });
	CDebugText::Render("- warning -");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 11, 0.0f });
	CDebugText::Render("-- CameraRadianX --");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 12, 0.0f });
	CDebugText::Render("CameraRadianX : ", m_CameraRadianX);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 14, 0.0f });
	CDebugText::Render("-- CameraPosition --");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 15, 0.0f });
	CDebugText::Render("Pos_X : ", CCameraManager::GetPosition().x);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 16, 0.0f });
	CDebugText::Render("Pos_Y : ", CCameraManager::GetPosition().y);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 17, 0.0f });
	CDebugText::Render("Pos_Z : ", CCameraManager::GetPosition().z);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 19, 0.0f });
	CDebugText::Render("-- WarningPosition --");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 20, 0.0f });

}
