#include "GameClearEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\SpawnUFO\SpawnUFO.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventPlayer\EventPlayer.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventGirl\EventGirl.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventAlien\EventAlien_A\EventAlien_A.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\GameObject\Widget\EventWidget\EventWidget.h"

#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Common\DebugText\DebugText.h"

/************************************
*	�N���A�C�x���g�N���X.
**/
CGameClearEvent::CGameClearEvent()
	: m_pGroundStage		( nullptr )
	, m_pSpawnUFO			( nullptr )
	, m_pPlayer				( nullptr )
	, m_pGirl				( nullptr )
	, m_pEventCamera		( nullptr )
	, m_pEventManager		( nullptr )
	, m_pEventWidget		( nullptr )
	, m_stPlayer			()
	, m_stGirl				()
	, m_stAlien				()
	, m_stCamera			()
	, m_vUFOPosition		( D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vUFOScale			( D3DXVECTOR3(1.0f,1.0f,1.0f))
	, m_vAlienOldPosition	( D3DXVECTOR3(1.0f,1.0f,1.0f))
	, m_EventStep			( EEventStep::Start )
	, m_WaitCount			( 0 )
	, m_SwingCameraCount	( 50 )
	, m_Gravity				( 0.0f )
	, m_Speed				( 0.0f ) 
	, m_UFOStep				( 0 )
{
	m_pGroundStage	= std::make_shared<CGroundStage>();
	m_pSpawnUFO		= std::make_shared<CSpawnUFO>();
	m_pPlayer		= std::make_shared<CEventPlayer>();
	m_pGirl			= std::make_shared<CEventGirl>();
	m_pAlienA		= std::make_shared<CEventAlienA>();
	m_pEventCamera	= std::make_shared<CEventCamera>();
	m_pEventWidget  = std::make_shared<CEventWidget>();
}

CGameClearEvent::~CGameClearEvent()
{
}

// �ǂݍ��݊֐�.
bool CGameClearEvent::Load()
{
	if( m_pGroundStage->Init() == false )	return false;	// �X�e�[�W�̏�����.
	if( SpawnUFOInit() == false )			return false;	// UFO�̏�����.
	if( PlayerInit() == false )				return false;	// �v���C���[�̏�����.
	if( GirlInit() == false )				return false;	// ���̎q�̏�����.
	if( AlienInit() == false )				return false;	// �F���l�̏�����.
	if( CameraInit() == false )				return false;	// �J�����̏�����.
	if( m_pEventWidget->Init() == false )	return false;	// UI�̏�����.

	m_IsEventEnd = false;
	m_IsSkip = false;
	m_Speed = static_cast<float>(D3DX_PI) * 0.05f;
	CFade::SetFadeOut();

	return true;
}

// �X�V�֐�.
void CGameClearEvent::Update()
{
#if 0
	DebugOperation();
#endif

	// �A�N�^�̍X�V.
	ActorUpdate();
	// �J�����̍X�V.
	CameraUpdate();
	// �V�[���̐ݒ�.
	SceneSetting();

	// �X�L�b�v.
	if (GetAsyncKeyState(VK_RETURN) & 0x8000
		|| CXInput::B_Button() == CXInput::enPRESSED_MOMENT)
	{
		Skip();
	}
}

// �`��֐�.
void CGameClearEvent::Render()
{
	m_pGroundStage->Render();	// �X�e�[�W�̕`��.
	m_pSpawnUFO->Render();		// UFO�̕`��.
	m_pPlayer->Render();		// �v���C���[�̕`��.
	m_pGirl->Render();			// ���̎q�̕`��.
	m_pAlienA->Render();		// �F���lA�̕`��.

#if 1
	DebugRender();
#endif

}

// �X�v���C�g�`��֐�.
void CGameClearEvent::SpriteRender()
{
	m_pEventWidget->Update();
	m_pEventWidget->Render();
}

// �J�����������֐�.
bool CGameClearEvent::CameraInit()
{
	m_stCamera.vPosition = INIT_CAMERAPOSITION;
	m_stCamera.vLookPosition = m_stPlayer.vPosition;
	m_stCamera.vLookPosition.y = m_stPlayer.vPosition.y + CORRECTION_PLAYER_LOOKPOS_Y;
	m_stCamera.ViewingAngle = INIT_VIEWING_ANGLE;
	return true;
}

// UFO�������֐�.
bool CGameClearEvent::SpawnUFOInit()
{
	if( m_pSpawnUFO->Init() == false ) return false;
	m_vUFOPosition = INIT_UFOPOSITION;
	return true;
}

// �v���C���[�������֐�.
bool CGameClearEvent::PlayerInit()
{
	if( m_pPlayer->Init() == false ) return false;
	m_stPlayer.vPosition = m_pPlayer->GetPosition();
	m_stPlayer.vPosition.z = INIT_PLAYERPOSITION_Z;
	m_stPlayer.MoveSpeed = MOVE_SPEED;
	return true;
}

// ���̎q�������֐�
bool CGameClearEvent::GirlInit()
{
	if( m_pGirl->Init() == false )return false;
	m_stGirl.vPosition = m_pGirl->GetPosition();
	m_stGirl.vPosition.z = m_stPlayer.vPosition.z + INIT_GIRLPOSITION_Z;
	m_stGirl.MoveSpeed = MOVE_SPEED;
	return true;
}

// �F���l�������֐�.
bool CGameClearEvent::AlienInit()
{
	if( m_pAlienA->Init() == false ) return false;
	m_stAlien.vPosition = m_vAlienOldPosition = m_vUFOPosition;
	m_stAlien.vPosition.y = m_vUFOPosition.y + INIT_ALIENPOSITION_Y;
	m_stAlien.vRotation.y = ALIEN_ROTATION_Y;
	m_stAlien.IsDisp = false;
	return true;
}

// �A�N�^�̍X�V�֐�.
void CGameClearEvent::ActorUpdate()
{
	// �v���C���[.
	m_pPlayer->SetOptionalState( m_stPlayer );
	// ���̎q.
	m_pGirl->SetOptionalState( m_stGirl );
	// �F���l.
	m_pAlienA->SetOptionalState( m_stAlien );
	// UFO.
	m_pSpawnUFO->SetPosition( m_vUFOPosition );
	m_pSpawnUFO->SetScale(m_vUFOScale);
}

// �J�����̍X�V�֐�.
void CGameClearEvent::CameraUpdate()
{
	m_pEventCamera->SetState(m_stCamera);
	CCameraManager::SetCamera(m_pEventCamera);
}

// �V�[���̐ݒ�.
void CGameClearEvent::SceneSetting()
{
	switch (m_EventStep)
	{
	case EEventStep::RunTowards_UFO:	// UFO�Ɍ������đ���.
		RunTowardsUFO();
		break;
	case EEventStep::SuckedInto_UFO:	// UFO�ɋz�����܂��.
		SuckedIntoUFO();
		break;
	case  EEventStep::Up_UFO:			//UFO�̃A�b�v.
		UpUFO();
		break;
	case EEventStep::Fuss_UFO:			// UFO�̂ǂ�����΂�����呛��.
		FussUFO();
		break;
	case EEventStep::KickedOut_Alien:	// �F���l�ǂ��o�����.
		FussUFO();
		KickedOutAlien();
		FallAlien();
		break;
	case EEventStep::Fall_Alien:		// �F���l����.
		FallAlien();
		break;
	case EEventStep::Move_UFO:			// UFO�ړ�.
		MoveUFO();
		break;
	case EEventStep::NextScene:			//���̃V�[���Ɉړ�.
		NextScene();
		break;
	default:
		break;
	}

}

// ���̃V�[���ɐi�߂�.
void CGameClearEvent::NextStep()
{
	int step = static_cast<int>(m_EventStep);
	step++;
	m_EventStep = static_cast<EEventStep>(step);
}

// �X�L�b�v.
void CGameClearEvent::Skip()
{
	if(m_EventStep == EEventStep::NextScene) return;
	if(m_IsSkip == true) return;

	CFade::SetFadeIn();
	int step = static_cast<int>(EEventStep::NextScene) - 1;
	m_EventStep = static_cast<EEventStep>(step);
	NextStep();

	m_IsSkip = true;
}

// �A�N�^�̏k��.
void CGameClearEvent::ScaleDownActor(D3DXVECTOR3& scale, const float& speed)
{
	if (scale.x <= 0.0f) return;
	scale.x -= speed;
	scale.y -= speed;
	scale.z -= speed;
}

// UFO�Ɍ������đ���.
void CGameClearEvent::RunTowardsUFO()
{
	m_stCamera.vLookPosition = m_stPlayer.vPosition;
	m_stPlayer.vPosition.z -= m_stPlayer.MoveSpeed;
	m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;

	if (m_stPlayer.vPosition.z > m_vUFOPosition.z) return;
	NextStep();
}

// UFO�ɋz�����܂��.
void CGameClearEvent::SuckedIntoUFO()
{
	// �J�����̒����ړ�.
	if (m_stCamera.vLookPosition.y <= MOVE_LIMIT_Z_SUCKED_INTO_UFO)
	{
		m_stCamera.vLookPosition.y += MOVESPEED_Z_SUCKED_INTO_UFO;
	}

	// �v���C���[�Ə��̎q���k��.
	ScaleDownActor(m_stPlayer.vScale, SCALEDOWN_SPEED);
	ScaleDownActor(m_stGirl.vScale, SCALEDOWN_SPEED);

	// �����蔻��.
	m_stPlayer.vPosition = m_pSpawnUFO->Collision(m_pPlayer.get());
	m_stGirl.vPosition = m_pSpawnUFO->Collision(m_pGirl.get());

	if (m_stPlayer.vScale.x > 0.0f) return;
	m_stPlayer.IsDisp = false;
	m_stGirl.IsDisp = false;
	NextStep();
}

// UFO�̃A�b�v.
void CGameClearEvent::UpUFO()
{
	m_WaitCount++;

	if (m_WaitCount < WAITCOUNT_UP_UFO) return;
	m_stCamera.ViewingAngle -= VIEWING_ANGLE_MOVESPEED;

	if (m_stCamera.ViewingAngle > VIEWING_ANGLE_UP_UFO) return;
	m_WaitCount = 0;
	NextStep();
}

// UFO�̑呛��.
void CGameClearEvent::FussUFO()
{
	switch (m_UFOStep)
	{
	case 0:
		m_vUFOScale.x += UFO_SCALE_SPEED_DEFAULT;
		if(m_vUFOScale.y >= UFO_SCALE_MIN) m_vUFOScale.y -= UFO_SCALE_SPEED_Y;
		if(m_vUFOScale.x >= UFO_SCALE_MAX_X) m_UFOStep++;
		break;
	case 1:
		if(m_vUFOScale.x >= SCALE_DEFAULT.x) m_vUFOScale.x -= UFO_SCALE_SPEED_DEFAULT;
		m_vUFOScale.y += UFO_SCALE_SPEED_DEFAULT;
		if (m_vUFOScale.y >= UFO_LIMIT_SCALE_Y) m_UFOStep++;
		break;
	case 2:
		m_vUFOScale.x += UFO_SCALE_SPEED_X;
		m_vUFOScale.x += UFO_SCALE_SPEED_X;
		m_vUFOScale.y -= UFO_SCALE_SPEED_Y;
		if (m_vUFOScale.y < UFO_SCALE_MIN) m_UFOStep++;
		break;
	case 3:
		if (m_vUFOScale.x > SCALE_DEFAULT.x) m_vUFOScale.x -= UFO_SCALE_SPEEDMAX_X;
		m_vUFOScale.y += UFO_SCALE_SPEED_DEFAULT;

		if (m_vUFOScale.y < UFO_SCALE_MAX_Y) return;
		m_UFOStep++;
		m_stAlien.IsDisp = true;
		NextStep();
		break;
	case 4:
		m_vUFOScale.y -= UFO_SCALE_SPEEDMAX_Y;
		if (m_vUFOScale.y <= SCALE_DEFAULT.y) m_UFOStep++;
		break;

	case 5:
		m_vUFOScale = SCALE_DEFAULT;
		break;
	default:
		break;
	}
}

// �F���l�ǂ��o�����.
void CGameClearEvent::KickedOutAlien()
{
	m_stCamera.ViewingAngle += VIEWING_ANGLE_MOVESPEED;
	if (m_stCamera.vPosition.y >= CAMERA_LIMITPOS_Y_KICKEDOUT_ALIEN) m_stCamera.vPosition.y -= CAMERA_SPEED_Y_KICKEDOUT_ALIEN;

	if (m_stCamera.ViewingAngle <= m_pEventCamera->ResetViewingAngle()) return;
	NextStep();
}

// �F���l����.
void CGameClearEvent::FallAlien()
{
	// �F���l�̗���,
	if (m_stAlien.vPosition.y > ALIEN_FALL_POSITION_Y) {
		m_stAlien.vPosition.x += sinf(m_stAlien.vRotation.y + static_cast<float>(D3DX_PI)) * ALIEN_FALL_SPEED_X;
		m_stAlien.vPosition.z += cosf(m_stAlien.vRotation.y + static_cast<float>(D3DX_PI)) * ALIEN_FALL_SPEED_Z;
		m_stAlien.vPosition.y += ALIEN_FALL_SPEED_Y + m_Gravity;
		m_Gravity -= ADD_GRAVITY;
	}
	else
	{
		if (m_SwingCameraCount != 0) m_SwingCameraCount--;
		// �J�����h�炷.
		m_stCamera.vLookPosition.y 
			= m_stCamera.vLookPosition.y + static_cast<float>(sin(D3DX_PI * TWO / FREQUENCY_LOOKPOS_Y * m_SwingCameraCount) * (m_SwingCameraCount * AMPLITUDE_LOOKPOS));
	}

	// �F���l�̉�].
	if (m_stAlien.vPosition.y < m_vAlienOldPosition.y) {
		if (m_stAlien.vRotation.x <= ALIEN_FALL_ROTATION) m_stAlien.vRotation.x += ALIEN_FALL_ROTATION_SPEED;
	}
	m_vAlienOldPosition = m_stAlien.vPosition;

	// �J�����̒����ʒu�ړ�.
	if (m_stAlien.vPosition.y < LOOK_POSITION_Y_FALL_ALIEN) {
		if (m_stCamera.vLookPosition.y >= m_vUFOPosition.y)
		{
			m_stCamera.vLookPosition.y -= LOOK_SPEED_Y_FALL_ALIEN;
		}
	}

	// ���̃V�[����.
	if (m_SwingCameraCount == 0) m_WaitCount++;
	if (m_WaitCount < WAIT_COUNT_FALL_ALIEN) return;
	m_WaitCount = 0;
	NextStep();
}

// UFO�̈ړ�.
void CGameClearEvent::MoveUFO()
{
	m_WaitCount++;

	m_stCamera.vPosition = CAMERAPOSITION;
	m_stCamera.vLookPosition = m_vUFOPosition;
	m_stCamera.vLookPosition.y = m_vUFOPosition.y + CORRECTION_PLAYER_LOOKPOS_Y;

	if( m_WaitCount <= WAIT_COUNT_MOVE_UFO ) return;
	if( m_vUFOPosition.y <= UFO_SURFACING_POSITION_Y) { m_vUFOPosition.y += UFO_SURFACING_SPEED; }

	if(m_WaitCount <= WAIT_COUNT_MOVE_UFO_MAX ) return;
	m_vUFOPosition.x -= UFO_MOVE_SPEED_X;
	m_vUFOPosition.y += UFO_MOVE_SPEED_Y;

	ScaleDownActor(m_vUFOScale, SCALEDOWN_SPEED);
	if (m_vUFOScale.x <= 0.0f)
	{
		CFade::SetFadeIn();
		NextStep();
	}
}

// ���̃V�[���Ɉړ�.
void CGameClearEvent::NextScene()
{
	if (CFade::GetIsFade() == true) return;
	m_IsEventEnd = true;
}

void CGameClearEvent::DebugRender()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameClearEvent -");
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

void CGameClearEvent::DebugOperation()
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