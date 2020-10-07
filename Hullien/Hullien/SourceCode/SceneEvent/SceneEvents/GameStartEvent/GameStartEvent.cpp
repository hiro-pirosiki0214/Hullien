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

#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Common\DebugText\DebugText.h"

/***********************************
*	スタートイベントクラス.
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
	CFade::SetFadeOut();

	m_pGroundStage		= std::make_shared<CGroundStage>();
	m_pSpawnUFO			= std::make_shared<CSpawnUFO>();
	m_pPlayer			= std::make_shared<CEventPlayer>();
	m_pGirl				= std::make_shared<CEventGirl>();
	m_pAlienA			= std::make_shared<CEventAlienA>();
	m_pBarrier			= std::make_shared<CBarrier>();
	m_pMotherShipUFO	= std::make_shared<CMotherShipUFO>();
	m_pWidget			= std::make_unique<CGameStartEventWidget>();
	m_pEventCamera		= std::make_shared<CEventCamera>();
}

CGameStartEvent::~CGameStartEvent()
{
}

// 読込関数.
bool CGameStartEvent::Load()
{
	if( m_pGroundStage->Init() == false )	return false;	// 地面の初期化.
	if( SpawnUFOInit() == false )			return false;	// UFOの初期化.
	if( PlayerInit() == false )				return false;	// プレイヤーの初期化.
	if( GirlInit() == false )				return false;	// 女の子の初期化.
	if( AlienInit() == false )				return false;	// 宇宙人Aの初期化.
	if( MotherShipUFOInit() == false )		return false;	// マザーシップの初期化.
	if( CameraInit() == false )				return false;	// カメラの初期化.
	if (m_pWidget->Init() == false)			return false;	// UIの設定.

	m_IsEventEnd = false;
	m_IsSkip = false;
	m_Speed = static_cast<float>(D3DX_PI) * 0.05f;
	return true;
}

// 更新関数.
void CGameStartEvent::Update()
{
#if 0
	DebugOperation();
#endif

	// シーンの設定.
	SceneSetting();
	// アクタの更新.
	ActorUpdate();
	// カメラの更新.
	CameraUpdate();
	// UIの更新.
	m_pWidget->Update();

	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		// 長押しされたら遷移するようにしたい.
		Skip();
	}
}

// 描画関数.
void CGameStartEvent::Render()
{
	m_pGroundStage->Render();	// ステージの描画.
	m_pSpawnUFO->Render();		// UFOの描画.
	m_pPlayer->Render();		// プレイヤーの描画.
	m_pGirl->Render();			// 女の子の描画.
	m_pAlienA->Render();		// 宇宙Aの描画.
	m_pBarrier->Render();
	m_pMotherShipUFO->Render();

}

// スプライト描画関数.
void CGameStartEvent::SpriteRender()
{
	m_pWidget->Render();
	DebugRender();
}

// カメラ初期化関数.
bool CGameStartEvent::CameraInit()
{
	m_stCamera.vPosition = CAMERA_INITPOSITION;
	m_stCamera.vLookPosition = m_pPlayer->GetPosition();
	m_stCamera.vLookPosition.y = m_pPlayer->GetPosition().y + CAMERA_CORRECTION_PLAYERPOS_Y;
	return true;
}

// UFO初期化関数.
bool CGameStartEvent::SpawnUFOInit()
{
	if (m_pSpawnUFO->Init() == false) return false;	
	m_vUFOPosition = UFO_INITPOSITION;

	return true;
}

// プレイヤー初期化関数.
bool CGameStartEvent::PlayerInit()
{
	if (m_pPlayer->Init() == false) return false;	
	m_stPlayer.vPosition.z = PLAYER_INITPOSITION_Z;
	return true;
}

// 女の子初期化関数.
bool CGameStartEvent::GirlInit()
{
	if (m_pGirl->Init() == false) return false;
	m_stGirl.vPosition.z = GIRL_INITPOSITION_Z;
	return true;
}

// 宇宙人初期化関数.
bool CGameStartEvent::AlienInit()
{
	if (m_pAlienA->Init() == false) return false;
	m_stAlien.vRotation.y = ALIEN_INITROTATION_Y;
	m_stAlien.IsDisp = false;
	m_stAlien.ModelAlpha = 0.0f;
	m_stAlien.vScale = D3DXVECTOR3(0.0f,0.0f,0.0f);
	return true;
}

// マザーシップ初期化関数.
bool CGameStartEvent::MotherShipUFOInit()
{
	if (m_pMotherShipUFO->Init() == false) return false;
	m_pMotherShipUFO->SetPosition(MOTHERSHIP_INITPOSITION);
	m_pMotherShipUFO->SetDisp(false);
	return true;
}

// アクタの更新関数.
void CGameStartEvent::ActorUpdate()
{
	// プレイヤー.
	m_pPlayer->SetOptionalState( m_stPlayer );
	// 女の子.
	m_pGirl->SetOptionalState( m_stGirl );
	// 宇宙人.
	m_pAlienA->SetOptionalState( m_stAlien );
	// UFOの位置設定.
	m_pSpawnUFO->SetPosition( m_vUFOPosition );
}

// カメラの更新関数.
void CGameStartEvent::CameraUpdate()
{
	m_pEventCamera->SetState( m_stCamera );
	CCameraManager::SetCamera( m_pEventCamera );
}

// シーンの設定.
void CGameStartEvent::SceneSetting()
{
	switch (m_EventStep)
	{
	case EEventStep::Escape_PlayerAndGirl:	//逃げるプレイヤーと女の子.
		EscapePlayerAndGirl();
		break;
	case EEventStep::Viewpoint_UFO:	//2人を追うUFOの視点.
		ViewpointUFO();
		break;
	case EEventStep::Move_UFO:	//UFO定位置につく.
		MoveUFO();
		break;
	case EEventStep::Stop_UFO: // UFO停止.
		StopUFO();
		break;
	case EEventStep::Appearance_Alien: //宇宙人登場.
		AppearanceAlien();
		break;
	case EEventStep::Move_Alien: // 宇宙人ダッシュ.
		MoveAlien();
		break;
	case EEventStep::GetCaught_Girl: // 女の子捕まる.
		GetCaughtGirl();
		break;
	case EEventStep::InvocatingOrder_Barrier:	//バリア発動指示.
		InvocatingOrderBarrier();
		break;
	case EEventStep::Player_Up:				//バリア発動.
		PlayerUp();
		break;
	case EEventStep::Invocating_Barrier:	//バリア発動.
		InvocatingBarrier();
		break;
	case EEventStep::Return_Girl:			// 女の子帰還.
		ReturnGirl();
		break;
	case EEventStep::Disp_Preserve_Girl:	// 女の子を守る指示の表示.
		DispPreserveGirl();
		break;
	case EEventStep::GameStart:	//ゲームシーンへ.
		GameStart();
		break;
	default:
		break;
	}

}

// 次のシーンに進める.
void CGameStartEvent::NextStep()
{
	m_NowStep++;
	m_EventStep = static_cast<EEventStep>(m_NowStep);
}

// スキップ.
void CGameStartEvent::Skip()
{
	if (m_EventStep == EEventStep::Disp_Preserve_Girl) return;
	if (m_EventStep == EEventStep::GameStart) return;
	if (m_IsSkip == true) return;
	// プレイヤー.
	m_stPlayer.vPosition = { 0.0f,m_pPlayer->GetPosition().y,0.0f };
	m_stPlayer.vRotation.y = 0.0f;
	// 女の子.
	m_stGirl.vPosition.z = PRESERVE_GIRL_DISP_POSITION;
	// 宇宙人.
	m_stAlien.IsDisp = false;
	// UFO.
	m_pSpawnUFO->SetPosition(UFO_POSITION);
	// カメラ.
	m_stCamera.vLookPosition = m_stPlayer.vPosition;
	m_stCamera.ViewingAngle = m_pEventCamera->ResetViewingAngle();
	m_stCamera.vPosition = GIRL_POSITION;

	m_NowStep = SKIP_SCENE;
	NextStep();

	m_IsSkip = true;
	// UI.
	m_pWidget->SetSkip(m_IsSkip);

}

// 逃げるプレイヤーと女の子.
void CGameStartEvent::EscapePlayerAndGirl()
{
	m_stCamera.vPosition.z = m_stPlayer.vPosition.z;
	m_stCamera.vLookPosition = m_stPlayer.vPosition;
	m_stPlayer.vPosition.z -= m_stPlayer.MoveSpeed;
	m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;

	// 次のステップへ.
	if (m_stPlayer.vPosition.z <= CAMERASWITCHING_POS_Z) NextStep();
}

// UFO視点.
void CGameStartEvent::ViewpointUFO()
{
	// カメラの設定.
	m_stCamera.vPosition = m_vUFOPosition;
	m_stCamera.vPosition.y = m_vUFOPosition.y + CAMERA_CORRECTION_UFOPOS_Y;
	m_stCamera.vPosition.z = m_vUFOPosition.z - CAMERA_CORRECTION_UFOPOS_Z;
	m_stCamera.vLookPosition.z = m_pPlayer->GetPosition().z - CAMERA_CORRECTION_PLAYERLOOK_Z;
	// プレイヤーと女の子の移動.
	if (m_stPlayer.vPosition.z > 0.0f)
	{
		m_stPlayer.vPosition.z -= m_stPlayer.MoveSpeed;
		m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;
	}

	// UFO移動.
	m_vUFOPosition.z -= UFO_MOVE_SPEED;

	// 次のステップへ.
	if (m_pSpawnUFO->GetPosition().z <= CAMERASWITCHING_POS_Z) NextStep();
}

// UFO移動.
void CGameStartEvent::MoveUFO()
{
	// UFO移動.
	if (m_pSpawnUFO->GetPosition().z >= UFO_MOVEING_LIMIT_Z)
	{
		m_vUFOPosition.z -= UFO_MOVE_SPEED;
	}

	// カメラ位置.
	m_stCamera.vPosition = CAMERA_POSITION_MOVEUFO;
	// カメラの視点移動.
	m_stCamera.vLookPosition = m_pSpawnUFO->GetPosition();
	// プレイヤーの位置設定.
	m_stPlayer.vPosition.z = 0.0f;
	// 女の子の位置設定.
	m_stGirl.vPosition.z = GIRL_CONSTANT_POSITION_Z;

	// 次のステップ.
	if (m_pSpawnUFO->GetPosition().z < UFO_MOVEING_LIMIT_Z)
	{
		NextStep();
	}
}

// UFO停止.
void CGameStartEvent::StopUFO()
{
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
		m_stAlien.vPosition = m_pSpawnUFO->GetPosition();
		m_Count = 0.0f;
		NextStep();
	}
}

// 宇宙人登場.
void CGameStartEvent::AppearanceAlien()
{
	m_stCamera.vPosition = CAMERA_POSITION_APP_ALIEN;
	m_stCamera.vLookPosition.y = m_pAlienA->GetPosition().y + CAMERA_CORRECTIONALIENPOS_Y;

	m_pAlienA->Spawn(m_stAlien.vPosition);
	m_stAlien.vScale = m_pAlienA->GetScale();
	m_stAlien.ModelAlpha = m_pAlienA->GetParameter().ModelAlpha;
	m_stAlien.IsDisp = m_pAlienA->GetParameter().IsDisp;


	if (m_stAlien.vPosition.y >= ALIEN_MOVEING_LIMIT_Y)
	{
		m_stAlien.vPosition.y -= ALIEN_FALL_SPEED;
	}
	else
	{
		NextStep();
	}
}

// 宇宙人移動.
void CGameStartEvent::MoveAlien()
{
	m_Count++;
	m_stAlien.vPosition.y = m_pAlienA->GetPosition().y + static_cast<float>(sin(D3DX_PI * TWO / FREQUENCY_ALIEN_UPDOWN * m_Count) * AMPLITUDE_ALIEN_UPDOWN);
	if (m_Count > WAIT_COUNT)
	{
		m_stAlien.vPosition.z += ALIEN_RUN_SPEED;
	}

	if (m_stAlien.vPosition.z >= 0.0f)
	{
		m_Count = 0;
		NextStep();
	}
}

// 女の子捕まる.
void CGameStartEvent::GetCaughtGirl()
{
	// カメラの設定.
	m_stCamera.vPosition = CAMERA_POSITION_CAUGHT_GIRL;
	m_stCamera.vLookPosition = m_pPlayer->GetPosition();
	// 宇宙人の設定.
	m_stAlien.vPosition.z += ALIEN_MOVE_SPEED;
	// UFOの設定.
	m_pSpawnUFO->SetDisp(false);
	// 女の子が捕まったらプレイヤーを回転させる.
	if (m_pGirl->GetIsDanger() == true)
	{
		m_stPlayer.vRotation.y = m_pPlayer->RotationMoveRight(PLAYER_ROTATION_Y, m_stPlayer.RotationalSpeed);
	}

	// 当たり判定.
	m_pGirl->Collision(m_pAlienA.get());
	m_pAlienA->Collision( m_pGirl.get() );
	m_stGirl.vPosition = m_pGirl->GetPosition();

	// 宇宙人が画面外に出たら次のステップへ.
	if (m_stAlien.vPosition.z > ALIEN_MOVEING_LIMIT_Z) NextStep();
}

// バリア発動準備.
void CGameStartEvent::InvocatingOrderBarrier()
{
	// カメラの設定.
	m_stCamera.vPosition = CAMERA_POSITION_ORDER_BARRIER;
	m_stCamera.vLookPosition = m_pGirl->GetPosition();
	// マザーシップの描画.
	m_pMotherShipUFO->SetDisp(true);
	// プレイヤーの更新.
	m_pPlayer->Update();
	// UIの設定.
	m_pWidget->SetWidgetState(CGameStartEventWidget::EWidgetState::Push_YButton);

	// Yボタンが押された場合.
	if (m_pPlayer->IsSpecialAbility() == true)
	{
		// カメラの設定.
		m_stCamera.vLookPosition = m_pPlayer->GetPosition();
		m_stCamera.vLookPosition.y = m_pPlayer->GetPosition().y + CAMERA_CORRECTION_PLAYERPOS_Y;
		m_stCamera.vPosition = CAMERA_POSITION_PLAYER_UP;
		m_stCamera.ViewingAngle = VIEWING_ANGLE_PLAYER_UP;
		// UIの設定.
		m_pWidget->SetWidgetState(CGameStartEventWidget::EWidgetState::None);

		m_pBarrier->Init();	// バリアの初期化.
		NextStep();			// 次のステップへ.
	}
}

// プレイヤーのアップ.
void CGameStartEvent::PlayerUp()
{
	// 視野角が移動限界値よりも大きい場合.
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
		// カメラをプレイヤーの後ろ側に移動.
		if (m_stCamera.vRotation.x > CAMERA_ROTAION_MOVING_LIMIT_X)
		{
			m_stCamera.vRotation.x -= static_cast<float>(D3DX_PI) * m_stCamera.MoveSpeed;
			// カメラの加速・減速の設定.
			if (m_stCamera.vRotation.x > CAMERA_ROTAION_DEC_START)
			{
				if (m_stCamera.MoveSpeed <= CAMERA_ROTATION_SPEED) m_stCamera.MoveSpeed += CAMERA_DECELERATION_SPEED;
			}
			else
			{
				if (m_stCamera.MoveSpeed >= CAMERA_ROTATION_SPEED) m_stCamera.MoveSpeed -= CAMERA_DECELERATION_SPEED;
			}
		}
		else
		{
			NextStep();
			// カメラの揺れ用カウント.
			m_Count = AMPLITUDE_COUNT;
		}

		// カメラとプレイヤーの距離.
		if (m_stCamera.vLenght.z < CAMERA_LENGHT_Z) m_stCamera.vLenght.z += CAMERA_MOVE_SPEED;
		if (m_stCamera.vLookPosition.z < CAMERA_LOOKPOS_Z_PLAYER_UP) m_stCamera.vLookPosition.z += CAMERA_MOVE_SPEED;

		// カメラ位置を算出.
		m_stCamera.vPosition.x = m_stCamera.vLookPosition.x + (sinf(m_stCamera.vRotation.x) * m_stCamera.vLenght.x);
		m_stCamera.vPosition.z = m_stCamera.vLookPosition.z + (cosf(m_stCamera.vRotation.x) * m_stCamera.vLenght.z);
	}
}

// バリア発動.
void CGameStartEvent::InvocatingBarrier()
{
	// カメラの揺れ.
	m_stCamera.vLookPosition.x = m_stCamera.vLookPosition.x + static_cast<float>(sin(D3DX_PI * TWO / FREQUENCY_LOOKPOS_Y * m_Count) * (m_Count * AMPLITUDE_LOOKPOS));
	m_stCamera.vLookPosition.y = m_stCamera.vLookPosition.y + static_cast<float>(sin(D3DX_PI * TWO / FREQUENCY_LOOKPOS_Z * m_Count) * (m_Count * AMPLITUDE_LOOKPOS));
	if (m_Count != 0) {
		m_Count--;
	}

	// バリア.
	m_pBarrier->SetTargetPos(*m_pGirl.get());
	m_pBarrier->Update();

	// 宇宙人とバリアの当たり判定.
	m_pAlienA->Collision(m_pBarrier.get());
	m_stAlien.vPosition = m_pAlienA->GetPosition();
	if (m_pAlienA->IsBarrierHit() == true) { m_stAlien.vPosition.x -= GET_EXPELLED_SPEED; }
	m_pAlienA->SetPosition(m_stAlien.vPosition);

	if (m_pBarrier->IsActive() == false)
	{
		NextStep();
	}
}

// 女の子帰還.
void CGameStartEvent::ReturnGirl()
{
	m_pSpawnUFO->SetDisp( true );
	m_vUFOPosition.y = UFO_POSITION.y;
	m_vUFOPosition.z = UFO_POSITION.z;

	m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;

	if (m_stGirl.vPosition.z >= CAMERASWITCHING_GIRLPOS_Z) return;

	// プレイヤーの回転値設定.
	m_stPlayer.vRotation.y = m_pPlayer->RotationMoveRight(PLAYER_DEFAULT_ROTATION_Y, m_stPlayer.RotationalSpeed);

	// カメラの設定.
	m_stCamera.vLookPosition = m_stPlayer.vPosition;
	m_stCamera.ViewingAngle = m_pEventCamera->ResetViewingAngle();

	if (m_stCamera.vPosition.x < GIRL_POSITION.x) { m_stCamera.vPosition.x += CAMERA_MOVE_SPEED; }
	else { m_stCamera.vPosition.x = GIRL_POSITION.x; }

	if (m_stCamera.vPosition.y < GIRL_POSITION.y) { m_stCamera.vPosition.y += CAMERA_MOVE_SPEED_Y; }
	else { m_stCamera.vPosition.y = GIRL_POSITION.y; }

	if (m_stCamera.vPosition.z < GIRL_POSITION.z) { m_stCamera.vPosition.z += CAMERA_MOVE_SPEED_Z; }
	else { m_stCamera.vPosition.z = GIRL_POSITION.z; }

	if (m_stGirl.vPosition.z <= PRESERVE_GIRL_DISP_POSITION)  NextStep();
}

// 女の子を守る指示の表示.
void CGameStartEvent::DispPreserveGirl()
{
	// UIの設定.
	m_pWidget->SetWidgetState(CGameStartEventWidget::EWidgetState::Preserve_Girl);

	m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;
	if (m_stGirl.vPosition.z <= 0.0f)  NextStep();
}

// ゲーム開始.
void CGameStartEvent::GameStart()
{
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

