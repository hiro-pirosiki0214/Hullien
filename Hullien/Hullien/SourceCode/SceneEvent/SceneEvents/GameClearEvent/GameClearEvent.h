#ifndef GAMECLEAR_EVENT_H
#define GAMECLEAR_EVENT_H

#include "..\..\EventBase\EventBase.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventCharacter.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"

class CGroundStage;				// 地面クラス.
class CSpawnUFO;				// イベント用UFOクラス.
class CEventPlayer;				// イベント用プレイヤークラス.
class CEventGirl;				// イベント用女の子クラス.
class CEventAlienA;				// イベント用宇宙人Aクラス.
class CEventCamera;				// イベント用カメラクラス.
class CEventManager;			// イベント管理クラス.
class CEventWidget;				// イベントのUIクラス.
class CSkyDome;					// 背景.


/************************************
*	クリアイベントクラス.
**/
class CGameClearEvent : public CEventBase
{
	const D3DXVECTOR3 SCALE_DEFAULT				= D3DXVECTOR3(1.0f, 1.0f, 1.0f);		//標準拡大値.
	const float TWO								= 2.0f;		// 二.
	const float MOVE_SPEED						= 0.2f;		// 移動速度.
	const float SCALEDOWN_SPEED					= 0.01f;	// 縮小速度.
	const int WAIT_COUNT_FALL_ALIEN				= 150;		// 宇宙人の落下時の待機時間.
	const int WAIT_COUNT_MOVE_UFO				= 100;		// UFO移動時の待機時間.
	const int WAIT_COUNT_MOVE_UFO_MAX			= 350;		// UFO移動時の待機時間.


	// カメラ.
	const D3DXVECTOR3 INIT_CAMERAPOSITION		= D3DXVECTOR3(-27.0f, 7.5f, -65.5f);	// カメラの初期位置.
	const D3DXVECTOR3 CAMERAPOSITION			= D3DXVECTOR3(-8.0f, 19.0f, -114.5f);	// カメラの位置.
	const float CORRECTION_PLAYER_LOOKPOS_Y		= 3.0f;									// プレイヤー注視位置y座標補正.
	const float INIT_VIEWING_ANGLE				= 0.5f;									// 視野角の初期値.
	const float MOVE_LIMIT_Z_SUCKED_INTO_UFO	= 18.0f;								// UFOに吸い込まれるときの移動限界.
	const float MOVESPEED_Z_SUCKED_INTO_UFO		= 0.15f;								// UFOに吸い込まれるときの移動速度.
	const float VIEWING_ANGLE_MOVESPEED			= 0.01f;								// 視野角移動速度.
	const float VIEWING_ANGLE_UP_UFO			= 0.34f;								// UFOアップ時の視野角.
	const float CAMERA_LIMITPOS_Y_KICKEDOUT_ALIEN = 1.3f;	// 宇宙人が追い出されるときのカメラの限界値.
	const float CAMERA_SPEED_Y_KICKEDOUT_ALIEN	= 0.3f;		// 宇宙人が追い出されるときのカメラの移動速度.
	const float FREQUENCY_LOOKPOS_Y				= 10.0f;	// カメラ注視位置の周波数.
	const float AMPLITUDE_LOOKPOS				= 0.1f;		// カメラ注視位置の振幅.
	const float LOOK_POSITION_Y_FALL_ALIEN		= 9.0f;		// 宇宙人の落下時の注視位置.
	const float LOOK_SPEED_Y_FALL_ALIEN			= 1.0f;		// 宇宙人の落下時の注視位置移動速度.

	// プレイヤー.
	const float INIT_PLAYERPOSITION_Z			= -30.0f;	//プレイヤーの初期z座標.
	// 女の子.
	const float INIT_GIRLPOSITION_Z				= 2.5f;	//女の子の初期z座標.

	// 宇宙人.
	const float INIT_ALIENPOSITION_Y			= 9.0f;	//宇宙人の初期y座標.
	const float ALIEN_ROTATION_Y				= static_cast<float>(D3DXToRadian(145)); // 宇宙人のy座標回転値.
	const float ALIEN_FALL_POSITION_Y			= 2.0f;		//宇宙人の落下位置.
	const float ALIEN_FALL_SPEED_X				= 0.19f;	//宇宙人の落下速度.
	const float ALIEN_FALL_SPEED_Y				= 0.4f;		//宇宙人の落下速度.
	const float ALIEN_FALL_SPEED_Z				= 0.2f;		//宇宙人の落下速度.
	const float ADD_GRAVITY						= 0.005f;	//重力の加算値.
	const float ALIEN_FALL_ROTATION				= static_cast<float>(D3DXToRadian(160));	//宇宙人の落下時の回転値.
	const float ALIEN_FALL_ROTATION_SPEED		= static_cast<float>(D3DXToRadian(20));		//宇宙人の落下時の回転速度.

	// UFO.
	const D3DXVECTOR3 INIT_UFOPOSITION			= D3DXVECTOR3(0.0f, 10.0f, -100.0f);	// UFOの初期位置.
	const int WAITCOUNT_UP_UFO					= 50;									// UFOアップ時の待機時間.
	const float UFO_SCALE_SPEED_DEFAULT			= 0.1f;		// UFOの拡大速度の標準.
	const float UFO_SCALE_MIN					= 0.5f;		// UFOの拡大値最小.
	const float UFO_SCALE_MAX_X					= 3.0f;		// UFOの拡大値最大x座標.
	const float UFO_SCALE_MAX_Y					= 2.5f;		// UFOの拡大値最大y座標.
	const float UFO_LIMIT_SCALE_Y				= 2.0f;		// UFOの拡大値移動限界.
	const float UFO_SCALE_SPEED_X				= 0.01f;	// x座標拡大速度.
	const float UFO_SCALE_SPEEDMAX_X			= 0.05f;	// x座標拡大速度最大.
	const float UFO_SCALE_SPEED_Y				= 0.05f;	// y座標拡大速度.
	const float UFO_SCALE_SPEEDMAX_Y			= 0.08f;	// y座標拡大速度最大.
	const float UFO_SURFACING_SPEED				= 0.1f;		// UFO浮上速度.
	const float UFO_SURFACING_POSITION_Y		= 25.0f;	// UFO浮上位置y座標.
	const float UFO_MOVE_SPEED_X				= 3.0f;		// UFO移動速度.
	const float UFO_MOVE_SPEED_Y				= 0.5f;		// UFO移動速度.

private:
	// イベントのステップ.
	enum class enEventStep
	{
		RunTowards_UFO,		// UFOに向かって走る.
		SuckedInto_UFO,		// UFOに吸い込まれる.
		Up_UFO,				// UFOのアップ.
		Fuss_UFO,			// UFOの大騒ぎ.
		KickedOut_Alien,	// 宇宙人追い出される.
		Fall_Alien,			// 宇宙人落下.
		Move_UFO,			// UFOの移動.
		NextScene,			// 次のシーンに移動.

		Max,
		Start = RunTowards_UFO,
		Skip = NextScene - 1,

	}typedef EEventStep;

public:
	CGameClearEvent();
	virtual ~CGameClearEvent();

	// 読み込み関数.
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

	// アクタの縮小.
	void ScaleDownActor(D3DXVECTOR3& scale, const float& speed);

	// 以下イベントステップ関数.
	void RunTowardsUFO();		// UFOに向かって走る.
	void SuckedIntoUFO();		// UFOに吸い込まれる.
	void UpUFO();				// UFOのアップ.
	void FussUFO();				// UFOの大騒ぎ.
	void KickedOutAlien();		// 宇宙人追い出される.
	void FallAlien();			// 宇宙人落下.
	void MoveUFO();				// UFOの移動.
	void NextScene();			// 次のシーンに移動.


	// 以下デバッグ用.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>			m_pGroundStage;
	std::shared_ptr<CSpawnUFO>				m_pSpawnUFO;
	std::shared_ptr<CEventPlayer>			m_pPlayer;
	std::shared_ptr<CEventGirl>				m_pGirl;
	std::shared_ptr<CEventAlienA>			m_pAlienA;
	std::shared_ptr<CEventCamera>			m_pEventCamera;
	std::shared_ptr<CSkyDome>				m_pSkyDome;
	std::shared_ptr<CEventWidget>			m_pEventWidget;
	CEventCharacter::SOptionalState			m_stPlayer;			//プレイヤーの情報.
	CEventCharacter::SOptionalState			m_stGirl;			//女の子の情報.
	CEventCharacter::SOptionalState			m_stAlien;			//宇宙人の情報.
	CEventCamera::SCameraState				m_stCamera;			//カメラの情報.

	D3DXVECTOR3								m_vUFOPosition;		// UFOの位置.
	D3DXVECTOR3								m_vUFOScale;		// UFOの大きさ.
	D3DXVECTOR3								m_vAlienOldPosition;// 宇宙人の前座標.
	EEventStep								m_EventStep;		// イベントのステップ.
	int										m_WaitCount;		// 待機カウント.
	int										m_SwingCameraCount;	// カメラを揺らすカウント.
	float									m_Gravity;			// 重力.

	float									m_Speed;
	int										m_UFOStep;			// UFOのステップ.

};


#endif	//#ifndef GAMECLEAR_EVENT_H