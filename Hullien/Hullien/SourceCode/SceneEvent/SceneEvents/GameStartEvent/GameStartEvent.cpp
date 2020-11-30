#include "GameStartEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include ".\..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\SpawnUFO\SpawnUFO.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventPlayer\EventPlayer.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventGirl\EventGirl.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventAlien\EventAlien_A\EventAlien_A.h"
#include "..\..\..\GameObject\Actor\Barrier\Barrier.h"
#include "..\..\..\GameObject\MotherShipUFO\MotherShipUFO.h"
#include "..\..\..\GameObject\Widget\EventWidget\GameStartEventWidget\GameStartEventWidget.h"
#include "..\..\..\GameObject\SkyDome\SkyDome.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\EventManager\EventManager.h"
#include "..\..\..\GameObject\SkyDome\SkyDome.h"
#include "..\..\..\GameObject\Arm\Arm.h"

#include "..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Common\DebugText\DebugText.h"

/***********************************
*	�X�^�[�g�C�x���g�N���X.
**/
CGameStartEvent::CGameStartEvent()
	: m_pGroundStage		( nullptr )
	, m_pSpawnUFO			( nullptr )
	, m_pPlayer				( nullptr )
	, m_pGirl				( nullptr )
	, m_pAlienA				( nullptr )
	, m_pBarrier			( nullptr )
	, m_pMotherShipUFO		( nullptr )
	, m_pWidget				( nullptr )
	, m_pEventCamera		( nullptr )
	, m_pEventManager		( nullptr )
	, m_pSkyDome			( nullptr )
	, m_vUFOPosition		( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ))
	, m_EventStep			( EEventStep::EventStart )
	, m_NowStep				( 0 )
	, m_Speed				( 0.0f )
	, m_DecelerationY		( 0.0f )
	, m_DecelerationZ		( 0.0f )
	, m_Count				( 0.0f )
	, m_stPlayer			()
	, m_stGirl				()
	, m_stAlien				()
	, m_stCamera			()
{
	m_pGroundStage		= std::make_shared<CGroundStage>();
	m_pSpawnUFO			= std::make_shared<CSpawnUFO>();
	m_pPlayer			= std::make_shared<CEventPlayer>();
	m_pGirl				= std::make_shared<CEventGirl>();
	m_pAlienA			= std::make_shared<CEventAlienA>();
	m_pBarrier			= std::make_shared<CBarrier>();
	m_pMotherShipUFO	= std::make_shared<CMotherShipUFO>();
	m_pWidget			= std::make_unique<CGameStartEventWidget>();
	m_pEventCamera		= std::make_shared<CEventCamera>();
	m_pSkyDome			= std::make_shared<CSkyDome>();
}

CGameStartEvent::~CGameStartEvent()
{
}

// �Ǎ��֐�.
bool CGameStartEvent::Load()
{
	CFade::SetFadeOut();
	CSoundManager::ThreadPlayBGM("StartEventBGM");
	CSoundManager::FadeInBGM("StartEventBGM");
	if (MotherShipUFOInit() == false)		return false;	// �}�U�[�V�b�v�̏�����.
	if( m_pGroundStage->Init() == false )	return false;	// �n�ʂ̏�����.
	if( SpawnUFOInit() == false )			return false;	// UFO�̏�����.
	if( PlayerInit() == false )				return false;	// �v���C���[�̏�����.
	if( GirlInit() == false )				return false;	// ���̎q�̏�����.
	if( AlienInit() == false )				return false;	// �F���lA�̏�����.
	if( CameraInit() == false )				return false;	// �J�����̏�����.
	if( m_pWidget->Init() == false )		return false;	// UI�̐ݒ�.
	if (m_pSkyDome->Init() == false)		return false;	// �w�i�̏�����.

	m_IsEventEnd = false;
	m_IsSkip = false;
	m_Speed = static_cast<float>(D3DX_PI) * 0.05f;
	return true;
}

// �X�V�֐�.
void CGameStartEvent::Update()
{
#if _DEBUG
	DebugOperation();
#endif

	m_pSkyDome->SetPosition(m_stPlayer.vPosition);
	// �V�[���̐ݒ�.
	SceneSetting();
	// �A�N�^�̍X�V.
	ActorUpdate();
	// �J�����̍X�V.
	CameraUpdate();
	// UI�̍X�V.
	m_pWidget->Update();
}

// �`��֐�.
void CGameStartEvent::Render()
{
	m_pSkyDome->Render();		// �w�i�̕`��.
	m_pGroundStage->Render();	// �X�e�[�W�̕`��.
	m_pPlayer->Render();		// �v���C���[�̕`��.
	m_pGirl->Render();			// ���̎q�̕`��.
	m_pAlienA->Render();		// �F��A�̕`��.
	m_pSpawnUFO->Render();		// UFO�̕`��.
	m_pMotherShipUFO->Render();
	m_pBarrier->EffectRender();

//	DebugRender();

}

// �X�v���C�g�`��֐�.
void CGameStartEvent::SpriteRender()
{
	m_pWidget->Render();
//	DebugRender();
}

// �J�����������֐�.
bool CGameStartEvent::CameraInit()
{
//	m_stCamera.vPosition = CAMERA_INITPOSITION;
//	m_stCamera.vLookPosition = m_pPlayer->GetPosition();
//	m_stCamera.vLookPosition.y = m_pPlayer->GetPosition().y + CAMERA_CORRECTION_PLAYERPOS_Y;

	m_stCamera.vPosition = { -7.0f, 2.5f, -86.0f };
	m_stCamera.vLookPosition = { 0.0f, 1.0f, -86.0f };
	return true;
}

// UFO�������֐�.
bool CGameStartEvent::SpawnUFOInit()
{
	if (m_pSpawnUFO->Init() == false) return false;	
	m_vUFOPosition = UFO_INITPOSITION;
	m_pSpawnUFO->SetDisp(false);

	return true;
}

// �v���C���[�������֐�.
bool CGameStartEvent::PlayerInit()
{
	if (m_pPlayer->Init() == false) return false;	
	m_stPlayer.vPosition.z = PLAYER_INITPOSITION_Z;
	m_stPlayer.vRotation.y = PLAYER_DEFAULT_ROTATION_Y;
	return true;
}

// ���̎q�������֐�.
bool CGameStartEvent::GirlInit()
{
	if (m_pGirl->Init() == false) return false;
	m_stGirl.vPosition.z = m_stPlayer.vPosition.z + GIRL_DISTANCE_Z;
	m_stGirl.vRotation.y = GIRL_DEFAULT_ROTATION_Y;
	return true;
}

// �F���l�������֐�.
bool CGameStartEvent::AlienInit()
{
	if (m_pAlienA->Init() == false) return false;
	m_stAlien.vRotation.y = ALIEN_INITROTATION_Y;
	m_stAlien.IsDisp = false;
	m_stAlien.ModelAlpha = 0.0f;
	m_stAlien.vScale = D3DXVECTOR3(0.0f,0.0f,0.0f);
	return true;
}

// �}�U�[�V�b�v�������֐�.
bool CGameStartEvent::MotherShipUFOInit()
{
	if (m_pMotherShipUFO->Init() == false) return false;
	m_pMotherShipUFO->SetDisp(false);
	m_pMotherShipUFO->DischargePreparation();

	return true;
}

// �A�N�^�̍X�V�֐�.
void CGameStartEvent::ActorUpdate()
{
	// �v���C���[.
	m_pPlayer->SetOptionalState( m_stPlayer );
	// ���̎q.
	m_pGirl->SetOptionalState( m_stGirl );
	// �F���l.
	m_pAlienA->SetOptionalState( m_stAlien );
	m_pAlienA->Update();
	m_pMotherShipUFO->Update();
	// UFO�̈ʒu�ݒ�.
	m_pSpawnUFO->SetPosition( m_vUFOPosition );
	m_pSpawnUFO->Update();
}

// �J�����̍X�V�֐�.
void CGameStartEvent::CameraUpdate()
{
	m_pEventCamera->SetState( m_stCamera );
	CCameraManager::SetCamera( m_pEventCamera );
}

// �V�[���̐ݒ�.
void CGameStartEvent::SceneSetting()
{
	switch (m_EventStep)
	{
	case EEventStep::FootUp:	//���̃A�b�v.
		FootUp();
		break;
	case EEventStep::Escape_PlayerAndGirl:	//������v���C���[�Ə��̎q.
		EscapePlayerAndGirl();
		break;
	case EEventStep::Viewpoint_UFO:	//2�l��ǂ�UFO�̎��_.
		ViewpointUFO();
		break;
	case EEventStep::Move_UFO:	//UFO��ʒu�ɂ�.
		MoveUFO();
		break;
	case EEventStep::Stop_UFO: // UFO��~.
		StopUFO();
		break;
	case EEventStep::Appearance_Alien: //�F���l�o��.
		AppearanceAlien();
		break;
	case EEventStep::Move_Alien: // �F���l�_�b�V��.
		MoveAlien();
		break;
	case EEventStep::GetCaught_Girl: // ���̎q�߂܂�.
		GetCaughtGirl();
		break;
	case EEventStep::InvocatingOrder_Barrier://�o���A�����w��.
		InvocatingOrderBarrier();
		break;
	case EEventStep::Player_Up:				//�v���C���[�A�b�v.
		PlayerUp();
		break;
	case EEventStep::Invocating_Barrier:	//�o���A����.
		InvocatingBarrier();
		break;
	case EEventStep::Return_Girl:			// ���̎q�A��.
		ReturnGirl();
		break;
	case EEventStep::Disp_Preserve_Girl:	// ���̎q�����w���̕\��.
		DispPreserveGirl();
		break;
	case EEventStep::GameStart:	//�Q�[���V�[����.
		GameStart();
		break;
	default:
		break;
	}

	// �X�L�b�v.
	if (GetAsyncKeyState(VK_RETURN) & 0x8001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD) {
		m_SkipWaitCount++;

	}
	else {
		if (m_SkipWaitCount < SKIP_WAIT_COUNT) m_SkipWaitCount = 0;
	}

	if (m_SkipWaitCount < SKIP_WAIT_COUNT) return;
	Skip();
}

// ���̃V�[���ɐi�߂�.
void CGameStartEvent::NextStep()
{
	m_NowStep++;
	m_EventStep = static_cast<EEventStep>(m_NowStep);
}

// �X�L�b�v.
void CGameStartEvent::Skip()
{
	if (m_EventStep == EEventStep::Disp_Preserve_Girl) return;
	if (m_EventStep == EEventStep::GetCaught_Girl) return;
	if (m_EventStep == EEventStep::GameStart) return;
	if (m_IsSkip == true) return;

	CFade::SetFadeIn();
	if (CFade::GetIsFade() == true) return;

	// �v���C���[.
	m_stPlayer.vPosition.z = 0.0f;
	m_stPlayer.vRotation.y = PLAYER_ROTATION_Y;
	m_pPlayer->SetAnimation(player::EAnimNo::EAnimNo_Wait);
	// �F���l.
	m_stAlien.IsDisp = true;
	m_stAlien.vScale = SCALE_MAX;
	const D3DXVECTOR3 ALIEN_DESTINATION = { m_stAlien.vPosition.x, ALIEN_MOVEING_LIMIT_Y, ALIEN_MOVEING_LIMIT_Z };
	m_stAlien.vPosition = ALIEN_DESTINATION;
	m_pAlienA->SetArmAppearance();
	// ���̎q.
	m_stGirl.vPosition.x = -0.015f;
	m_stGirl.vPosition.y = m_stAlien.vPosition.y;
	m_stGirl.vPosition.z = m_stAlien.vPosition.z-CArm::GRAB_DISTANCE;
	m_pGirl->SetAnimation(girl::EAnimNo::EAnimNo_Abduct);
	// UFO.
	m_pSpawnUFO->SetDisp( false );
	CSoundManager::StopAllSE("UFOMove");

	// �X�L�b�v��̃V�[���ɑJ��.
	m_NowStep = static_cast<int>(EEventStep::Skip);
	NextStep();
	// �t�F�[�h�A�E�g.
	CFade::SetFadeOut();

	m_IsSkip = true;
	// UI.
	m_pWidget->SetSkip(m_IsSkip);

}

// ���̃A�b�v.
void CGameStartEvent::FootUp()
{
	m_stPlayer.vPosition.z += RUN_SPEED;
	m_stGirl.vPosition.z += RUN_SPEED;

	if (m_stPlayer.vPosition.z >= -87.0f)
	{
		m_Count += AMPLITUDE_SPEED;
		m_stCamera.vLookPosition.y += static_cast<float>(sin(D3DX_PI * TWO / 13.0f * m_Count) * 0.1f);
	}

	if (m_stPlayer.vPosition.z > -70.0f)
	{
		m_stCamera.vPosition = CAMERA_INITPOSITION;
		m_Count = 0;
		NextStep();
	}
}

// ������v���C���[�Ə��̎q.
void CGameStartEvent::EscapePlayerAndGirl()
{
	m_stCamera.vPosition.z = m_stPlayer.vPosition.z;
	m_stCamera.vLookPosition = m_stPlayer.vPosition;
	m_stCamera.vLookPosition.y = m_stPlayer.vPosition.y + 1.0f;
	
	m_Count += AMPLITUDE_SPEED;
	if (static_cast<int>(m_Count) % 7)
	{
		m_stCamera.vLookPosition.y += static_cast<float>(sin(D3DX_PI * TWO / 13.0f * m_Count) * 0.1f);
	}
	else
	{
		m_stCamera.vLookPosition.x += static_cast<float>(sin(D3DX_PI * TWO / 13.0f * m_Count) * 0.1f);
		m_stCamera.vLookPosition.y += static_cast<float>(sin(D3DX_PI * TWO / 13.0f * m_Count) * 0.2f);
	}
	// �v���C���[�̖ړI�n.
	const D3DXVECTOR3 PLAYER_DESTINATION = { 0.0f, m_stPlayer.vPosition.y, 30.0f };
	// ���̎q�̖ړI�n.
	const D3DXVECTOR3 GIRL_DESTINATION = { 0.0f, m_stGirl.vPosition.y, GIRL_DISTANCE_Z };

	m_stPlayer.MoveSpeed = m_stGirl.MoveSpeed = RUN_SPEED;
	MoveDestination(m_stPlayer.vPosition, PLAYER_DESTINATION, m_stPlayer.MoveSpeed);
	if(MoveDestination(m_stGirl.vPosition, GIRL_DESTINATION, m_stGirl.MoveSpeed) == false) return;
	CSoundManager::PlaySE("UFOMove");

	// ���̃X�e�b�v��.
	m_pSpawnUFO->SetDisp(true);
	m_Count = 0;
	NextStep();
}

// UFO���_.
void CGameStartEvent::ViewpointUFO()
{
	// �J�����̐ݒ�.
	m_stCamera.vPosition = m_vUFOPosition;
	m_stCamera.vPosition.y = m_vUFOPosition.y + CAMERA_CORRECTION_UFOPOS_Y;
	m_stCamera.vPosition.z = m_vUFOPosition.z; CAMERA_CORRECTION_UFOPOS_Z;
	m_stCamera.vLookPosition.z = m_pPlayer->GetPosition().z + CAMERA_CORRECTION_PLAYERLOOK_Z;
	// �v���C���[�Ə��̎q�̈ړ�.
	const D3DXVECTOR3 PLAYER_DESTINATION = { 0.0f,m_stPlayer.vPosition.y, 0.0f };
	const D3DXVECTOR3 GIRL_DESTINATION = { 0.0f,m_stGirl.vPosition.y, GIRL_DISTANCE_Z };

	MoveDestination(m_stGirl.vPosition, GIRL_DESTINATION, m_stGirl.MoveSpeed);
	MoveDestination(m_stPlayer.vPosition, PLAYER_DESTINATION, m_stPlayer.MoveSpeed);

	// UFO�ړ�.
	const D3DXVECTOR3 UFO_DESTINATION = { 0.0f, m_vUFOPosition.y, CAMERASWITCHING_POS_Z };
	if(MoveDestination(m_vUFOPosition, UFO_DESTINATION, UFO_MOVE_SPEED) == false) return;
	// ���̃X�e�b�v��.
	NextStep();
}

// UFO�ړ�.
void CGameStartEvent::MoveUFO()
{
	// �J�����ʒu.
	m_stCamera.vPosition = D3DXVECTOR3(0.0f, 7.0f, 20.0f);
	// �J�����̎��_�ړ�.
	m_stCamera.vLookPosition = m_pSpawnUFO->GetPosition();
	// �v���C���[�̈ʒu�ݒ�.
	m_stPlayer.vPosition.z = 0.0f;
	// ���̎q�̈ʒu�ݒ�.
	m_stGirl.vPosition.z = GIRL_DISTANCE_Z;

	// UFO�ړ�.
	const D3DXVECTOR3 UFO_DESTINATION = { 0.0f, m_vUFOPosition.y, UFO_MOVEING_LIMIT_Z };
	if (MoveDestination(m_vUFOPosition, UFO_DESTINATION, UFO_MOVE_SPEED) == false) return;
	// ���̃X�e�b�v.
	NextStep();
}

// UFO��~.
void CGameStartEvent::StopUFO()
{
	m_pPlayer->SetAnimation(player::EAnimNo::EAnimNo_Wait);
	m_pGirl->SetAnimation(girl::EAnimNo::EAnimNo_Wait);
	MoveDestination(m_stCamera.vPosition, CAMERA_POSITION_MOVEUFO, 0.5f);

	m_Count++;
	if (m_Count < UFO_STOP_COUNT)
	{
		m_vUFOPosition.y = m_vUFOPosition.y + static_cast<float>(sin(D3DX_PI * TWO / FREQUENCY_UFO_STOP * m_Count) * AMPLITUDE_UFO_STOP);
	}
	else
	{
		if (m_vUFOPosition.y >= UFO_POSITION.y) m_vUFOPosition.y -= UFO_STOP_SPEED - m_DecelerationY;
		m_DecelerationY += UFO_STOP_DECELERATION_Y * UFO_STOP_DECELERATION_Y;
	}

	if (m_vUFOPosition.y < UFO_POSITION.y && m_Count >= WAIT_COUNT)
	{
		m_pSpawnUFO->LightDischarge();	// ���C�g���o��.
		m_stAlien.vPosition = m_pSpawnUFO->GetPosition();
		m_Count = 0.0f;
		NextStep();
	}
}

// �F���l�o��.
void CGameStartEvent::AppearanceAlien()
{
	m_stCamera.vPosition = CAMERA_POSITION_APP_ALIEN;
	m_stCamera.vLookPosition.y = m_pAlienA->GetPosition().y + CAMERA_CORRECTIONALIENPOS_Y;

	m_pAlienA->Spawn(m_stAlien.vPosition);
	m_stAlien.vScale = m_pAlienA->GetScale();
	m_stAlien.ModelAlpha = m_pAlienA->GetParameter().ModelAlpha;
	m_stAlien.IsDisp = m_pAlienA->GetParameter().IsDisp;

	// �F���l�̍~��.
	const D3DXVECTOR3 ALIEN_DESTINATION = { m_stAlien.vPosition.x, ALIEN_MOVEING_LIMIT_Y, m_stAlien.vPosition.z };
	if(MoveDestination(m_stAlien.vPosition, ALIEN_DESTINATION, ALIEN_FALL_SPEED) == false) return;
	NextStep();
}

// �F���l�ړ�.
void CGameStartEvent::MoveAlien()
{
	m_Count++;
	m_stAlien.vPosition.y = m_pAlienA->GetPosition().y + static_cast<float>(sin(D3DX_PI * TWO / FREQUENCY_ALIEN_UPDOWN * m_Count) * AMPLITUDE_ALIEN_UPDOWN);

	if (m_Count < WAIT_COUNT) return;
	m_pAlienA->SetTargetPos( *m_pGirl.get() );
	m_pAlienA->SetAnimSpeed( 0.01 );
	m_pAlienA->SetAnimation( CEventAlien::EAnimNo_Move );
	const D3DXVECTOR3 ALIEN_DESTINATION = m_pAlienA->GetTargetPosition();
	if (MoveDestination(m_stAlien.vPosition, ALIEN_DESTINATION, ALIEN_RUN_SPEED) == false) return;
	m_Count = 0;
	NextStep();
	m_pAlienA->SetAnimation( CEventAlien::EAnimNo_Arm );
}

// ���̎q�߂܂�.
void CGameStartEvent::GetCaughtGirl()
{
	// �J�����̐ݒ�.
	m_stCamera.vPosition = CAMERA_POSITION_CAUGHT_GIRL;
	m_stCamera.vLookPosition = m_pPlayer->GetPosition();
	m_stCamera.vLookPosition.y = m_pPlayer->GetPosition().y + CAMERA_CORRECTION_PLAYERPOS_Y;
	// UFO�̐ݒ�.
	m_pSpawnUFO->SetDisp(false);
	// �_���[�W�A�j���[�V�����̍X�V.
	m_pPlayer->DamageAnimUpdate();
	// ���̎q���߂܂�����v���C���[����]������.
	if (m_pGirl->GetIsDanger() == true && m_pPlayer->IsEndDamageAnim() == true )
	{
		// ���̎q�̕��֌����悤�ɂ���.
		const float rot = atan2f(
			m_stPlayer.vPosition.x - m_stGirl.vPosition.x,
			m_stPlayer.vPosition.z - m_stGirl.vPosition.z );
		m_stPlayer.vRotation.y = m_pPlayer->RotationMoveRight(rot, m_stPlayer.RotationalSpeed);
	}
	// �v���C���[���F���l�ƏՓ˂��ĂȂ����.
	if( m_pPlayer->IsAlienHit() == false ){
		m_pPlayer->SetAlienHit();
		m_pPlayer->SetAnimation( player::EAnimNo_Damage );
		m_stPlayer.vRotation.y = static_cast<float>(D3DXToRadian(270.0));
	}
	// �v���C���[���_���[�W�A�j���[�V�������͈ړ�.
	if( m_pPlayer->IsEndDamageAnim() == false ){
		m_stPlayer.vPosition.x += sinf(m_stPlayer.vRotation.y)*0.3f;
		m_stPlayer.vPosition.z += cosf(m_stPlayer.vRotation.y)*0.3f;
	}

	// �����蔻��.
	m_pGirl->Collision(m_pAlienA.get());
	m_pAlienA->Collision( m_pGirl.get() );
	m_stGirl.vPosition = m_pGirl->GetPosition();

	// �F���l�̐ݒ�.
	D3DXVECTOR3 ALIEN_DESTINATION;
	if (m_pAlienA->IsGrab() == false)
	{
		ALIEN_DESTINATION = m_pAlienA->GetTargetPosition();
	}
	else
	{
		ALIEN_DESTINATION = { m_stAlien.vPosition.x, m_stAlien.vPosition.y, ALIEN_MOVEING_LIMIT_Z };
		m_pGirl->SetAnimation(girl::EAnimNo::EAnimNo_Abduct);
	}
	if (MoveDestination(m_stAlien.vPosition, ALIEN_DESTINATION, ALIEN_MOVE_SPEED) == false) return;
	if (m_pAlienA->IsGrab() == false) return;
	m_IsSkip = true;
	m_pWidget->SetSkip(m_IsSkip);
	NextStep();
}

// �o���A��������.
void CGameStartEvent::InvocatingOrderBarrier()
{
	// �����蔻��.
	m_pGirl->Collision(m_pAlienA.get());
	m_pAlienA->Collision(m_pGirl.get());

	// �J�����̐ݒ�.
	m_stCamera.vPosition = CAMERA_POSITION_ORDER_BARRIER;
	m_stCamera.vLookPosition = m_pGirl->GetPosition();
	m_stPlayer.vPosition = { 0.0f, 0.0f, 0.0f };	// �v���C���[�̍��W�������l�ɐݒ�.
	m_stPlayer.vRotation.y = 0.0f;

	// �}�U�[�V�b�v�̕`��.
	m_pMotherShipUFO->SetDisp(true);
	// UI�̐ݒ�.
	m_pWidget->SetWidgetState(CGameStartEventWidget::EWidgetState::Push_YButton);
	
	if (CFade::GetIsFade() == true) return;	//�t�F�[�h���Ȃ�I��.
	if (m_pWidget->IsDispEnd() == false) return;
	// �v���C���[�̍X�V.
	m_pPlayer->Update();
	// Y�{�^���������ꂽ�ꍇ.
	if (m_pPlayer->IsYButtonPressed() == true)
	{
		// �J�����̐ݒ�.
#if 0
		m_stCamera.vLookPosition = m_pPlayer->GetPosition();
		m_stCamera.vLookPosition.y = m_pPlayer->GetPosition().y + CAMERA_CORRECTION_PLAYERPOS_Y;
		m_stCamera.vPosition = CAMERA_POSITION_PLAYER_UP;
		m_stCamera.ViewingAngle = VIEWING_ANGLE_PLAYER_UP;
#else
		m_stCamera.vPosition = {0.27f, 6.8f, -8.6f};

#endif
		// UI�̐ݒ�.
		m_pWidget->SetWidgetState(CGameStartEventWidget::EWidgetState::None);
		CSoundManager::NoMultipleSEPlay("PlayerUp");
		m_pBarrier->Init();	// �o���A�̏�����.
		m_pPlayer->IsSpecialAbility();

		NextStep();			// ���̃X�e�b�v��.
	}
}

// �v���C���[�̃A�b�v.
void CGameStartEvent::PlayerUp()
{
#if 0
	// ����p���ړ����E�l�����傫���ꍇ.
	if (m_stCamera.ViewingAngle > VIEWING_ANGLE_MOVING_LIMIT)
	{
		if (m_stCamera.ViewingAngle > VIEWING_ANGLE_DEC_START_POS)
		{
			m_stCamera.ViewingAngle -= VIEWING_ANGLE_MOVE_SPEED;
		}
		else
		{
			m_stCamera.ViewingAngle -= VIEWING_ANGLE_MOVE_SPEED + m_DecelerationZ;
			m_DecelerationZ -= VIEWING_ANGLE_DECELERATION_SPEED;
		}
	}
	else
	{
		m_DecelerationZ = 0.0f;
		m_stCamera.ViewingAngle = VIEWING_ANGLE_MOVING_LIMIT;

		// �J�������v���C���[�̌�둤�Ɉړ�.
		if (m_stCamera.vRotation.x > CAMERA_ROTAION_MOVING_LIMIT_X)
		{
			m_stCamera.vRotation.x -= static_cast<float>(D3DX_PI) * m_stCamera.MoveSpeed;
			// �J�����̉����E�����̐ݒ�.
			if (m_stCamera.vRotation.x > CAMERA_ROTAION_DEC_START)
			{
				if (m_stCamera.vLenght.z < CAMERA_LENGHT_Z) MoveDestination(m_stCamera.vLenght.z, CAMERA_LENGHT_Z, 0.1f);
				if (m_stCamera.MoveSpeed <= CAMERA_ROTATION_SPEED) m_stCamera.MoveSpeed += CAMERA_DECELERATION_SPEED;
			}
			else
			{
				//  �v���C���[�̓���\�̓A�j���[�V�����Đ�.
				m_pPlayer->IsSpecialAbility();
				if (m_stCamera.MoveSpeed >= CAMERA_ROTATION_SPEED) m_stCamera.MoveSpeed -= CAMERA_DECELERATION_SPEED;
				if (m_stCamera.vLenght.z < 33.0f) MoveDestination(m_stCamera.vLenght.z, CAMERA_LENGHT_Z, 0.01f);

			}

			// �����ʒu�����̎q�Ɉړ�.
			MoveDestination(m_stCamera.vLookPosition.x, m_stGirl.vPosition.y, 0.5f);
			// �J�����ƃv���C���[�̋���.
//			if (m_stCamera.vLenght.z < CAMERA_LENGHT_Z) MoveDestination(m_stCamera.vLenght.z, CAMERA_LENGHT_Z, 0.1f);
			
			// �J�����ʒu���Z�o.
			D3DXVECTOR3 pos;
			pos.x = m_stCamera.vLookPosition.x - (sinf(m_stCamera.vRotation.x) * m_stCamera.vLenght.x);
			pos.y = m_stCamera.vPosition.y;
			pos.z = m_stCamera.vLookPosition.z - (cosf(m_stCamera.vRotation.x) * m_stCamera.vLenght.z);
			D3DXVec3Lerp(&m_stCamera.vPosition, &m_stCamera.vPosition, &pos, 0.1f);

		}
		else
		{
			// �����ʒu�����̎q�Ɉړ�.
			D3DXVECTOR3 pos = m_stCamera.vLookPosition;
			pos.x = m_stGirl.vPosition.x + 5.0f;
			pos.y = m_stGirl.vPosition.y + 7.0f;
			D3DXVec3Lerp(&m_stCamera.vLookPosition, &m_stCamera.vLookPosition, &pos, 0.25f);
			if(m_pPlayer->IsSpecialAbility() == false) return;
			// �J�����̗h��p�J�E���g.
			m_Count = AMPLITUDE_COUNT;
			NextStep();
		}
	}
#else
	D3DXVECTOR3 pos;	// �{���̍��W.
	static float Degree = -3.3f;
	static float Lenght = 5.0f;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		Degree += D3DXToRadian(1.0);
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		Degree -= D3DXToRadian(1.0);
	}
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		Lenght += 0.1f;
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		Lenght -= 0.1f;
	}

	m_pPlayer->IsSpecialAbility();
	m_stCamera.vLookPosition = m_pPlayer->GetBonePosition("kaito_rifa_2_R_yubi_3_3");

	// �J�����ʒu���Z�o.
	pos.x = m_stCamera.vLookPosition.x + (sinf(Degree) * Lenght);
	pos.y = m_stCamera.vPosition.y;
	pos.z = m_stCamera.vLookPosition.z + (cosf(Degree) * Lenght);
	// lerp����. ( out, ���݂̍��W, �{�����B���Ă�����W, �␳�l ).
	D3DXVec3Lerp(&m_stCamera.vPosition, &m_stCamera.vPosition, &pos, 1.0f);


#endif
}

// �o���A����.
void CGameStartEvent::InvocatingBarrier()
{
	// �J�����̗h��.
	m_stCamera.vLookPosition.x = m_stCamera.vLookPosition.x + static_cast<float>(sin(D3DX_PI * TWO / FREQUENCY_LOOKPOS * m_Count) * (m_Count * AMPLITUDE_LOOKPOS_X));
	m_stCamera.vLookPosition.y = m_stCamera.vLookPosition.y + static_cast<float>(sin(D3DX_PI * TWO / FREQUENCY_LOOKPOS * m_Count) * (m_Count * AMPLITUDE_LOOKPOS_Y));
	if (m_Count != 0){
		m_Count -= AMPLITUDE_SPEED;
		// ��ʂ̗h��ɍ��킹�ăR���g���[���[�̃o�C�u������.
		CXInput::SetVibration( INPUT_VIBRATION_MAX, 0 );
	} else {
		// �o�C�u���Ȃ���.
		CXInput::SetVibration( 0, 0 );
	}

	// �o���A.
	m_pBarrier->SetTargetPos(*m_pGirl.get());
	m_pBarrier->Update();
	m_pGirl->SetAnimationBlend( girl::EAnimNo_Wait );

	// �F���l�ƃo���A�̓����蔻��.
	m_pAlienA->Collision(m_pBarrier.get());
	m_stAlien.vPosition = m_pAlienA->GetPosition();
	if (m_pAlienA->IsBarrierHit() == true) { m_stAlien.vPosition.x += GET_EXPELLED_SPEED; }
	m_pAlienA->SetPosition(m_stAlien.vPosition);

	if (m_pBarrier->IsActive() == false)
	{
		m_stAlien.IsDisp = false;
		// �v���C���[�̃A�j���[�V����.
		m_pPlayer->SetAnimationBlend(player::EAnimNo::EAnimNo_Wait);
		m_pPlayer->SetAnimSpeed();
		m_pGirl->SetAnimationBlend( girl::EAnimNo_Move );
		NextStep();
	}
}

// ���̎q�A��.
void CGameStartEvent::ReturnGirl()
{
	// UFO�ʒu.
	m_vUFOPosition.y = UFO_POSITION.y;
	m_vUFOPosition.z = UFO_POSITION.z;

	if (m_stGirl.vPosition.z >= CAMERASWITCHING_GIRLPOS_Z)
	{
		// �J�����̐ݒ�.
		m_stCamera.vLookPosition = m_stPlayer.vPosition;
		m_stCamera.vLookPosition.y = 9.0f;
		m_stCamera.ViewingAngle = m_pEventCamera->ResetViewingAngle();

		D3DXVec3Lerp(&m_stCamera.vPosition, &m_stCamera.vPosition, &CAMERA_GAMEPOSITION, CAMERA_MOVE_SPEED);
	}

	const D3DXVECTOR3 GIRL_DESTINATION = { m_stGirl.vPosition.x, 0.0f, PRESERVE_GIRL_DISP_POSITION };
	if(MoveDestination(m_stGirl.vPosition, GIRL_DESTINATION, m_stGirl.MoveSpeed) == false) return;
	NextStep();
}

// ���̎q�����w���̕\��.
void CGameStartEvent::DispPreserveGirl()
{
	// UI�̐ݒ�.
	m_pWidget->SetWidgetState(CGameStartEventWidget::EWidgetState::Preserve_Girl);

	const D3DXVECTOR3 GIRL_DESTINATION = { m_stGirl.vPosition.x, m_stGirl.vPosition.y, 4.0f };
	if (MoveDestination(m_stGirl.vPosition, GIRL_DESTINATION, m_stGirl.MoveSpeed) == false) return;
	NextStep();
}

// �Q�[���J�n.
void CGameStartEvent::GameStart()
{
	// ���̎q�̃A�j���[�V������ҋ@�ɂ�����.
	m_pGirl->SetAnimationBlend( girl::EAnimNo_Wait );
	CSoundManager::FadeOutBGM("StartEventBGM");
	m_IsEventEnd = true;
}

void CGameStartEvent::DebugRender()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameStartEvent -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 2.0f ,0.0f });
	CDebugText::Render("- CameraPosition -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 3.0f ,0.0f });
	CDebugText::Render("Pos_x", m_stCamera.vPosition.x);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 4.0f ,0.0f });
	CDebugText::Render("Pos_y", m_stCamera.vPosition.y);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 5.0f ,0.0f });
	CDebugText::Render("Pos_z", m_stCamera.vPosition.z);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 7.0f ,0.0f });
	CDebugText::Render("- LookPosition -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 8.0f ,0.0f });
	CDebugText::Render("Pos_x", m_stCamera.vLookPosition.x);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 9.0f ,0.0f });
	CDebugText::Render("Pos_y", m_stCamera.vLookPosition.y);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 10.0f ,0.0f });
	CDebugText::Render("Pos_z", m_stCamera.vLookPosition.z);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 12.0f ,0.0f });
	CDebugText::Render("- ViewingAngle -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 13.0f ,0.0f });
	CDebugText::Render("ViewinfAngle :", m_pEventCamera->GetViewingAngle());

}

void CGameStartEvent::DebugOperation()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_stCamera.vPosition.z += m_Speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_stCamera.vPosition.z -= m_Speed;
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_stCamera.vPosition.y += m_Speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_stCamera.vPosition.y -= m_Speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_stCamera.vPosition.x += m_Speed;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_stCamera.vPosition.x -= m_Speed;
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_stCamera.vLookPosition.y += m_Speed;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_stCamera.vLookPosition.y -= m_Speed;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_stCamera.vLookPosition.z -= m_Speed;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_stCamera.vLookPosition.z += m_Speed;
	}
}

