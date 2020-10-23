#include "GameOverEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\MotherShipUFO\MotherShipUFO.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventGirl\EventGirl.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\EventManager\EventManager.h"
#include "..\..\..\GameObject\Widget\EventWidget\EventWidget.h"
#include "..\..\..\GameObject\SkyDome\SkyDome.h"

#include "..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Common\DebugText\DebugText.h"

/************************************
*	ゲームオーバーイベントクラス.
**/
CGameOverEvent::CGameOverEvent()
	: m_pGroundStage	( nullptr )
	, m_pUFO			( nullptr )
	, m_pGirl			( nullptr )
	, m_pEventCamera	( nullptr )
	, m_pEventWidget	( nullptr )
	, m_pSkyDome		( nullptr )
	, m_stGirl			()
	, m_stCamera		()
	, m_vUFOPosition	( D3DXVECTOR3(0.0f, 0.0f, 0.0f) )
	, m_vUFOScale		( D3DXVECTOR3(0.0f, 0.0f, 0.0f) )
	, m_EventStep		( EEventStep::Start )
	, m_WaitCount		( 0 )
	, m_MoveCount		( 0.0f )
{
	m_pGroundStage	= std::make_shared<CGroundStage>();
	m_pUFO			= std::make_shared<CMotherShipUFO>();
	m_pGirl			= std::make_shared<CEventGirl>();
	m_pEventCamera	= std::make_shared<CEventCamera>();
	m_pEventWidget	= std::make_shared<CEventWidget>();
	m_pSkyDome		= std::make_shared<CSkyDome>();
	
}

CGameOverEvent::~CGameOverEvent()
{
}

// 読込関数.
bool CGameOverEvent::Load()
{
	CFade::SetFadeOut();
	CSoundManager::ThreadPlayBGM("GameOverEvent");
	CSoundManager::FadeInBGM("GameOverEvent");

	if( m_pGroundStage->Init() == false ) return false; // ステージの初期化.
	if( SpawnUFOInit() == false ) return false;			// UFOの初期化.
	if( GirlInit() == false ) return false;				// 女の子の初期化.
	if( CameraInit() == false ) return false;			// カメラの初期化.
	if( m_pEventWidget->Init() == false ) return false;	// UIの初期化.
	if( m_pSkyDome->Init() == false ) return false;		// 背景の初期化.

	m_IsSkip = false;
	m_IsEventEnd = false;

#if _DEBUG
	m_Speed = static_cast<float>(D3DX_PI) * 0.05f;
#endif
	return true;
}

// 更新関数.
void CGameOverEvent::Update()
{
	m_pSkyDome->SetPosition(m_stGirl.vPosition);
	// シーンの設定.
	SceneSetting();
	// アクタの更新.
	ActorUpdate();
	// カメラの更新.
	CameraUpdate();

	// デバッグ用.
#if _DEBUG
	DebugOperation();
#endif

}

// 描画関数.
void CGameOverEvent::Render()
{
	m_pSkyDome->Render();
	m_pGroundStage->Render();
	m_pUFO->Render();
	m_pGirl->Render();

	// デバッグ用.
#if _DEBUG
	DebugRender();
#endif

}

// スプライトの描画関数.
void CGameOverEvent::SpriteRender()
{
	if (m_IsSkip == true) return;
	m_pEventWidget->Update();
	m_pEventWidget->Render();
}

// カメラ初期化関数.
bool CGameOverEvent::CameraInit()
{
	m_stCamera.vPosition = CAMERA_INITPOSITION;
	m_stCamera.vLookPosition.y = LOOKPOS_Y;
	return true;
}

// UFO初期化関数.
bool CGameOverEvent::SpawnUFOInit()
{
	if(m_pUFO->Init() == false ) return false;
	m_vUFOPosition = UFO_INITPOSITION;
	return true;
}

// 女の子初期化関数.
bool CGameOverEvent::GirlInit()
{
	if( m_pGirl->Init() == false ) return false;
	m_stGirl.vPosition = GIRL_INITPOSITION;
	m_pGirl->SetAnimation( girl::EAnimNo_Wait );
	return true;
}

// アクタの更新関数.
void CGameOverEvent::ActorUpdate()
{
	// UFO.
	m_pUFO->SetPosition( m_vUFOPosition );

	// 女の子.
	m_pGirl->SetOptionalState( m_stGirl );
}

// カメラの更新関数.
void CGameOverEvent::CameraUpdate()
{
	m_pEventCamera->SetState( m_stCamera );
	CCameraManager::SetCamera( m_pEventCamera );
}

// シーンの設定.
void CGameOverEvent::SceneSetting()
{
	switch ( m_EventStep )
	{
	case EEventStep::Wait:					// 待機.
		Wait();
		break;
	case EEventStep::Sucked_Girl:			// 女の子吸い込まれる.
		SuckedGirl();
		break;
	case EEventStep::MoveRight_UFO_First:	// UFOの移動_右Part1.
		MoveRightUFOFirst();
		break;
	case EEventStep::MoveLeft_UFO:			// UFOの移動_左.
		MoveLeftUFO();
		break;
	case EEventStep::MoveRight_UFO_Second:	// UFOの移動_右Part2.
		MoveRightUFOSecond();
		break;
	case EEventStep::Move_Back_UFO:			// UFO奥に飛んでいく.
		MoveBackUFO();
		break;
	case EEventStep::EventEnd:				// イベント終了.
		EventEnd();
		break;
	default:
		break;
	}

	// スキップ.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD) {
		m_SkipWaitCount++;

	}
	else {
		if (m_SkipWaitCount < SKIP_WAIT_COUNT) m_SkipWaitCount = 0;
	}

	if (m_SkipWaitCount < SKIP_WAIT_COUNT) return;
	Skip();
}

// 次のシーンに進める.
void CGameOverEvent::NextStep()
{
	m_WaitCount = 0;

	int step = static_cast<int>(m_EventStep) + ONE;
	m_EventStep = static_cast<EEventStep>( step );
}

// スキップ.
void CGameOverEvent::Skip()
{
	if (m_IsSkip == true) return;
	CFade::SetFadeIn();

	if(CFade::GetIsFade() == true) return;
	m_stGirl.IsDisp = false;
	m_vUFOPosition = DESTINATION_BACK;
	m_stCamera.vLookPosition = m_vUFOPosition;
	m_EventStep = EEventStep::Skip;
	NextStep();
	m_IsSkip = true;
	CFade::SetFadeOut();
}

// 待機.
void CGameOverEvent::Wait()
{
	m_WaitCount++;
	if (m_WaitCount >= WAITCOUNT_WAIT) NextStep();
	
}

// 女の子が吸い込まれる.
void CGameOverEvent::SuckedGirl()
{
	if( m_stGirl.vPosition.y <= m_vUFOPosition.y + CORRECTION_UFOPOSITION_Y )
	{
		m_stGirl.vPosition.y += GIRL_RISE_SPEED;
		m_stCamera.vLookPosition.y += CAMERA_RISE_SPEED;
	}

	if( m_stGirl.vPosition.y <= GIRL_SCALEDOWN_STARTPOS ) return;
	if( m_stGirl.vScale.x > 0.0f)
	{
		if(m_stGirl.vScale.x == SCALE_MAX) CSoundManager::PlaySE("UFOSucked");
		m_stGirl.vScale.x -= GIRL_SCALEDOWN_SPEED;
		m_stGirl.vScale.y -= GIRL_SCALEDOWN_SPEED;
		m_stGirl.vScale.z -= GIRL_SCALEDOWN_SPEED;
	}
	else
	{
		m_WaitCount++;
	}

	if (m_WaitCount < WAITCOUNT_DEFAULT)return;
	m_stGirl.IsDisp = false;	//女の子を非表示にする.
	NextStep();
	CSoundManager::PlaySE("UFOEscape");
}

// UFOの右移動Part1.
void CGameOverEvent::MoveRightUFOFirst()
{
	m_stCamera.vLookPosition = m_vUFOPosition;

	if (MoveDestination( m_vUFOPosition, DESTINATION_RUGHTFIRST, UFO_MOVE_SPEED_RUGHTFIRST) == true) m_WaitCount++;

	if (m_WaitCount < WAITCOUNT_DEFAULT)return;
	NextStep();
	CSoundManager::PlaySE("UFOEscape");
}

// UFOの左移動.
void CGameOverEvent::MoveLeftUFO()
{
	m_stCamera.vLookPosition = m_vUFOPosition;

	if (MoveDestination(m_vUFOPosition, DESTINATION_LEFT, UFO_MOVE_SPEED_LEFT) == true) m_WaitCount++;
	if (m_WaitCount < WAITCOUNT_DEFAULT)return;
	NextStep();
	CSoundManager::PlaySE("UFOEscape");
}

// UFOの右移動Part1.
void CGameOverEvent::MoveRightUFOSecond()
{
	m_stCamera.vLookPosition = m_vUFOPosition;

	if (MoveDestination(m_vUFOPosition, DESTINATION_RUGHTSECOND, UFO_MOVE_SPEED_RUGHTSECOND) == true) m_WaitCount++;
	if (m_WaitCount < WAITCOUNT_DEFAULT)return;
	NextStep();
	CSoundManager::PlaySE("UFOPullout");
}

// UFO奥に移動.
void CGameOverEvent::MoveBackUFO()
{
	m_stCamera.vLookPosition = m_vUFOPosition;
	if (MoveDestination(m_vUFOPosition, DESTINATION_BACK, UFO_MOVE_SPEED_BACK) == false) return;
	m_pEventWidget->SetSkip(true);
	NextStep();
}

// イベント終了.
void CGameOverEvent::EventEnd()
{
	m_IsEventEnd = true;
}

void CGameOverEvent::DebugRender()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameOverEvent -");
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

void CGameOverEvent::DebugOperation()
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
