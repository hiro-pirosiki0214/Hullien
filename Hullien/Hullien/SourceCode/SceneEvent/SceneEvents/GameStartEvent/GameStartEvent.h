#ifndef GAMESTART_EVENT_H
#define GAMESTART_EVENT_H

#include "..\..\EventBase\EventBase.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventCharacter.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"

class CGroundStage;				// 地面クラス.
class CSpawnUFO;				// イベント用UFOクラス.
class CEventPlayer;				// イベント用プレイヤークラス.
class CEventGirl;				// イベント用女の子クラス.
class CEventAlienA;				// イベント用宇宙人Aクラス.
class CBarrier;					// バリアクラス.
class CMotherShipUFO;			// マザーシップクラス.
class CGameStartEventWidget;	// イベント用UIクラス.
class CEventCamera;				// イベント用カメラクラス.
class CEventManager;			// イベント管理クラス.

								/***********************************
								*	スタートイベントクラス.
								**/
class CGameStartEvent : public CEventBase
{
	const float TWO			= 2.0f;	// 二.
	const int   WAIT_COUNT	= 200;	// 待機時間.
	const int	SKIP_SCENE  = static_cast<int>(EEventStep::Disp_Preserve_Girl) - 1;	// スキップするシーン.

																					// カメラ関係.
	const D3DXVECTOR3 CAMERA_INITPOSITION = D3DXVECTOR3(23.0f, 7.5f, 20.0f);			// カメラ初期位置.
	const D3DXVECTOR3 CAMERA_POSITION_MOVEUFO = D3DXVECTOR3(3.5f, 7.0f, 10.5f);			// MoveUFOの時のカメラ位置.
	const D3DXVECTOR3 CAMERA_POSITION_APP_ALIEN = D3DXVECTOR3(0.0f, 8.5f, -15.0f);		// 宇宙人登場時のカメラ位置.
	const D3DXVECTOR3 CAMERA_POSITION_CAUGHT_GIRL = D3DXVECTOR3(-20.0f, 5.0f, -10.0f);	// 女の子捕獲時のカメラ位置.
	const D3DXVECTOR3 CAMERA_POSITION_ORDER_BARRIER = D3DXVECTOR3(-6.5f, 8.4f, -10.0f);	// バリア準備時のカメラ位置.
	const D3DXVECTOR3 CAMERA_POSITION_PLAYER_UP		= D3DXVECTOR3(0.0f, 8.0f, 7.0f);	// プレイヤーのアップ時のカメラ位置.
	const float AMPLITUDE_COUNT						= 50.0f;							// カメラの揺れ用カウント.
	const float CAMERA_CORRECTION_PLAYERPOS_Y		= 3.0f;								// プレイヤーに対するカメラy座標補正値.
	const float CAMERA_CORRECTION_UFOPOS_Y			= 5.0f;								// UFOに対するカメラy座標補正値.
	const float CAMERA_CORRECTION_UFOPOS_Z			= 5.0f;								// UFOに対するカメラz座標補正値.
	const float CAMERA_CORRECTIONALIENPOS_Y			= 1.0f;								// 宇宙人に対するカメラy座標補正値.
	const float CAMERA_CORRECTION_PLAYERLOOK_Z		= 1.5f;								// プレイヤーに対するカメラの注視位置z座標補正値.
	const float CAMERASWITCHING_POS_Z				= 40.0f;							// カメラ切り替えz座標,
	const float CAMERA_ROTAION_MOVING_LIMIT_X = static_cast<float>(D3DXToRadian(-150)); // カメラのx座標回転値の限界.
	const float CAMERA_ROTAION_DEC_START = static_cast<float>(D3DXToRadian(-100));		// カメラの減速開始値.
	const float CAMERA_DECELERATION_SPEED			= 0.0005f;							// カメラの減速度.
	const float CAMERA_ROTATION_SPEED				= 0.01f;							// カメラの回転速度.
	const float CAMERA_MOVE_SPEED					= 0.1f;								// カメラ移動速度.
	const float CAMERA_MOVE_SPEED_Y					= 0.2f;								// カメラ移動速度.
	const float CAMERA_MOVE_SPEED_Z					= 1.0f;								// カメラ移動速度.
	const float CAMERA_LENGHT_Z						= 16.0f;							// カメラとプレイヤーの距離z座標.
	const float CAMERA_LOOKPOS_Z_PLAYER_UP			= 9.5f;								// プレイヤーアップ時のカメラの注視位置.
	const float FREQUENCY_LOOKPOS_Y					= 10.0f;							// カメラ注視位置の周波数.
	const float FREQUENCY_LOOKPOS_Z					= 20.0f;							// カメラ注視位置の周波数.
	const float AMPLITUDE_LOOKPOS					= 0.01f;							// カメラ注視位置の振幅.

	const float VIEWING_ANGLE_PLAYER_UP				= 1.6f;								// プレイヤーのアップ時の視野角.
	const float VIEWING_ANGLE_MOVING_LIMIT			= 0.7f;								// 視野角の移動限界値.
	const float VIEWING_ANGLE_DEC_START_POS			= 1.2f;								// 視野角の減速開始位置.
	const float VIEWING_ANGLE_MOVE_SPEED			= 0.03f;							// 視野角の移動速度.
	const float VIEWING_ANGLE_DECELERATION_SPEED	= 0.0008f;							// 視野角の減速度.

																						// UFO関係.
	const D3DXVECTOR3 UFO_INITPOSITION	= D3DXVECTOR3(0.0f, 10.0f, 120.0f);			// UFO初期位置.
	const D3DXVECTOR3 UFO_POSITION		= D3DXVECTOR3(0.0f, 10.0f, -100.0f);			// UFO初期位置.
	const float UFO_STOP_COUNT			= 80.0f;										// UFO停止のカウント.
	const float UFO_MOVE_SPEED			= 0.3f;										// UFOの移動速度.
	const float UFO_STOP_DECELERATION_Y	= 0.001f;									// UFOの停止減速度.
	const float UFO_STOP_SPEED			= 0.01f;									// UFO停止速度.
	const float UFO_MOVEING_LIMIT_Y		= 11.5f;									// UFOの移動限界値y座標.
	const float UFO_MOVEING_LIMIT_Z		= -30.0f;									// UFOの移動限界値z座標.
	const float FREQUENCY_UFO_STOP		= 100.0f;									// UFO停止の周波数.
	const float AMPLITUDE_UFO_STOP		= 0.1f;										// UFO停止の振幅.

																					// プレイヤー関係.
	const float PLAYER_INITPOSITION_Z = 60.0f;										// プレイヤー初期位置.
	const float PLAYER_ROTATION_Y = static_cast<float>(D3DXToRadian(180));			// 宇宙人のy座標回転値.
	const float PLAYER_DEFAULT_ROTATION_Y = static_cast<float>(D3DXToRadian(0));			// 宇宙人のy座標回転値.

																							// 女の子関係.
	D3DXVECTOR3 GIRL_POSITION = D3DXVECTOR3(0.0f, 12.0f, 30.0f);					// 女の子の位置.
	const float GIRL_INITPOSITION_Z = 64.5f;										// 女の子初期位置.
	const float GIRL_CONSTANT_POSITION_Z = 4.5f;									// 女の子の定位置z座標.
	const float CAMERASWITCHING_GIRLPOS_Z = 20.0f;									// カメラ切り替え用の女の子のz座標,
	const float PRESERVE_GIRL_DISP_POSITION = 15.0f;								// 女の子を守れ指示の表示位置.

																					// 宇宙人関係.
	const float ALIEN_INITROTATION_Y = static_cast<float>(D3DXToRadian(180));		// 宇宙人のy座標回転値.
	const float ALIEN_MOVEING_LIMIT_Y = 5.0f;										// 宇宙人の移動限界値y座標.
	const float ALIEN_MOVEING_LIMIT_Z = 50.0f;										// 宇宙人の移動限界値z座標.
	const float ALIEN_FALL_SPEED = 0.05f;											// 宇宙人の降下速度.
	const float ALIEN_MOVE_SPEED = 0.3f;											// 宇宙人の移動速度.
	const float ALIEN_RUN_SPEED = 0.5f;												// 宇宙人の移動速度.
	const float FREQUENCY_ALIEN_UPDOWN = 90.0f;										// 宇宙人の周波数.
	const float AMPLITUDE_ALIEN_UPDOWN = 0.01f;										// 宇宙人の振幅.
	const float GET_EXPELLED_SPEED = 1.0f;											// バリアからはじき出される速度.

																					// マザーシップ関係.
	const D3DXVECTOR3 MOTHERSHIP_INITPOSITION = D3DXVECTOR3(0.0f, 20.0f, 100.0f);	// マザーシップ初期位置.

private:
	// イベントの各ステップ.
	enum class enEventStep
	{
		Escape_PlayerAndGirl = 0,	// 逃げるプレイヤーと女の子.
		Viewpoint_UFO,				// UFOの視点.
		Move_UFO,					// UFO定位置まで移動.
		Stop_UFO,					// UFO停止.
		Appearance_Alien,			// 宇宙人登場.
		Move_Alien,					// 宇宙人前進.
		GetCaught_Girl,				// 女の子が捕まる.
		InvocatingOrder_Barrier,	// バリア発動準備.
		Player_Up,					// プレイヤーのアップ.
		Invocating_Barrier,			// バリア発動.
		Return_Girl,				// 女の子帰還.
		Disp_Preserve_Girl,			// 女の子を守る指示の表示.
		GameStart,					// ゲーム開始.

		Max,
		EventStart = Escape_PlayerAndGirl,
	} typedef EEventStep;

public:
	CGameStartEvent();
	virtual ~CGameStartEvent();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// スプライト描画関数.
	virtual void SpriteRender() override;

private:
	// カメラ初期化関数.
	bool CameraInit();
	// UFO初期化関数.
	bool SpawnUFOInit();
	// プレイヤー初期化関数.
	bool PlayerInit();
	// 女の子初期化関数.
	bool GirlInit();
	// 宇宙人初期化関数.
	bool AlienInit();
	// マザーシップ初期化関数.
	bool MotherShipUFOInit();
	// アクタの更新関数.
	void ActorUpdate();
	// カメラの更新関数.
	void CameraUpdate();
	// シーンの設定.
	void SceneSetting();
	// 次のシーンに進める.
	virtual void NextStep() override;
	// スキップ.
	virtual void Skip() override;

	// 以下イベントのステップ.
	void EscapePlayerAndGirl();
	void ViewpointUFO();
	void MoveUFO();
	void StopUFO();
	void AppearanceAlien();
	void MoveAlien();
	void GetCaughtGirl();
	void InvocatingOrderBarrier();
	void PlayerUp();
	void InvocatingBarrier();
	void ReturnGirl();
	void DispPreserveGirl();
	void GameStart();

	// 以下デバッグ用.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>			m_pGroundStage;
	std::shared_ptr<CSpawnUFO>				m_pSpawnUFO;
	std::shared_ptr<CEventPlayer>			m_pPlayer;
	std::shared_ptr<CEventGirl>				m_pGirl;
	std::shared_ptr<CEventAlienA>			m_pAlienA;
	std::shared_ptr<CBarrier>				m_pBarrier;
	std::shared_ptr<CMotherShipUFO>			m_pMotherShipUFO;
	std::unique_ptr<CGameStartEventWidget>	m_pWidget;
	std::shared_ptr<CEventCamera>			m_pEventCamera;
	std::shared_ptr<CEventManager>			m_pEventManager;
	D3DXVECTOR3								m_vUFOPosition;
	EEventStep								m_EventStep;
	int										m_NowStep;
	float									m_Speed;
	float									m_DecelerationY;	// y座標減速度.
	float									m_DecelerationZ;	// z座標減速度.
	float									m_Count;			// カウント.

	CEventCharacter::SOptionalState			m_stPlayer;			//プレイヤーの情報.
	CEventCharacter::SOptionalState			m_stGirl;			//女の子の情報.
	CEventCharacter::SOptionalState			m_stAlien;			//宇宙人の情報.
	CEventCamera::SCameraState				m_stCamera;			//カメラの情報.
};

#endif //#ifndef START_EVENT_H.