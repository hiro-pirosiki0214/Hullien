#ifndef GAMEOVER_EVENT_H
#define GAMEOVER_EVENT_H

#include "..\..\EventBase\EventBase.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventCharacter.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"

class CGroundStage;				// 地面クラス.
class CMotherShipUFO;			// イベント用UFOクラス.
class CEventGirl;				// イベント用女の子クラス.
class CEventCamera;				// イベント用カメラクラス.
class CEventManager;			// イベント管理クラス.
class CEventWidget;				// イベントのUIクラス.
class CSkyDome;					// 背景.

/************************************
*	ゲームオーバーイベントクラス.
**/
class CGameOverEvent : public CEventBase
{
	const int ONE				= 1;	//一.
	const int WAITCOUNT_WAIT	= 150;	//待機時の待機時間.
	const int WAITCOUNT_DEFAULT	= 30;	//標準待機時間.

	// カメラ.
	const D3DXVECTOR3 CAMERA_INITPOSITION	= D3DXVECTOR3(0.3f, 4.0f, -15.5f);	// カメラ初期位置.
	const float CORRECTION_GIRLLOOKPOS_Y	= 1.0f;								// 女の子の注視位置補正.
	const float CAMERA_RISE_SPEED = 0.005f; //カメラの上昇速度.

	// UFO.
	const D3DXVECTOR3 UFO_INITPOSITION			= D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	const D3DXVECTOR3 DESTINATION_RUGHTFIRST	= D3DXVECTOR3(10.0f, 10.0f, 10.0f);		// 右移動Part1時の目的地.
	const D3DXVECTOR3 DESTINATION_LEFT			= D3DXVECTOR3(-20.0f, 30.0f, 40.0f);	// 左移動時の目的地.
	const D3DXVECTOR3 DESTINATION_RUGHTSECOND	= D3DXVECTOR3(10.0f, 50.0f, 100.0f);	// 右移動Part2時の目的地.
	const D3DXVECTOR3 DESTINATION_BACK			= D3DXVECTOR3(-100.0f, 200.0f, 500.0f);	// 奥移動時の目的地.
	const float CORRECTION_UFOPOSITION_Y		= 3.0f;									// UFO位置補正.
	const float UFO_MOVE_SPEED_RUGHTFIRST		= 0.5f;									// 右移動Part1時のUFO移動速度.
	const float UFO_MOVE_SPEED_LEFT				= 1.0f;									// 左移動時のUFO移動速度.
	const float UFO_MOVE_SPEED_RUGHTSECOND		= 2.5f;									// 右移動Part2時のUFO移動速度.
	const float UFO_MOVE_SPEED_BACK				= 8.0f;									// 奥移動時のUFO移動速度.

	// 女の子.
	const D3DXVECTOR3 GIRL_INITPOSITION = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
	const float GIRL_RISE_SPEED			= 0.03f;								//女の子の上昇速度.
	const float GIRL_SCALEDOWN_SPEED	= 0.005f;								//女の子の縮小速度.
	const float GIRL_SCALEDOWN_STARTPOS	= 7.0f;									//女の子の縮小開始位置.

private:
	// イベントステップ.
	enum class enEventStep
	{
		Wait = 0,					// 待機.
		Sucked_Girl,			// 女の子が吸い込まれる.
		MoveRight_UFO_First,	// UFO右移動Part1.
		MoveLeft_UFO,			// UFO左移動.
		MoveRight_UFO_Second,	// UFO右移動Part2.
		Move_Back_UFO,			// UFO奥に移動.
		EventEnd,				// イベント終了.

		Start = Wait,
		Skip = EventEnd - 1,	
	}typedef EEventStep;

public:
	CGameOverEvent();
	virtual ~CGameOverEvent();

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
	// 女の子初期化関数.
	bool GirlInit();
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
	
	// UFO移動.
	bool MoveUFO(const D3DXVECTOR3& vDestination, const float& speed);

	// 以下イベントステップ.
	void Wait();				// 待機.
	void SuckedGirl();			// 女の子が吸い込まれる.
	void MoveRightUFOFirst();	// UFOの右移動Part1.
	void MoveLeftUFO();			// UFOの左移動.
	void MoveRightUFOSecond();	// UFOの右移動Part1.
	void MoveBackUFO();			// UFO奥に移動.
	void EventEnd();			// イベント終了.

	// 以下デバッグ用.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>			m_pGroundStage;
	std::shared_ptr<CMotherShipUFO>			m_pUFO;
	std::shared_ptr<CEventGirl>				m_pGirl;
	std::shared_ptr<CEventCamera>			m_pEventCamera;
	std::shared_ptr<CEventWidget>			m_pEventWidget;
	std::shared_ptr<CSkyDome>				m_pSkyDome;
	CEventCharacter::SOptionalState			m_stGirl;			// 女の子の情報.
	CEventCamera::SCameraState				m_stCamera;			// カメラの情報.
	D3DXVECTOR3								m_vUFOPosition;		// UFOの位置.
	D3DXVECTOR3								m_vUFOScale;		// UFOの大きさ.
	EEventStep								m_EventStep;		// イベントステップ.
	int										m_WaitCount;		// 待機カウント.
	float									m_MoveCount;		// 移動カウント.

private:
	float m_Speed;
};

#endif	//#ifndef GAMEOVER_EVENT_H.