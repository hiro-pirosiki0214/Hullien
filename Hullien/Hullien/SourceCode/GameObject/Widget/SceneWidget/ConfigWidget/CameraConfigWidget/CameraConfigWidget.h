#ifndef CAMERA_CONFIG_WIDGET_H
#define CAMERA_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include "..\..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include <vector>

class CCursor;
class CSlinder;

class CCameraConfigWidget : public CWidget
{
	const char* SPRITE_CONTROL_NAME		= "CmaeraControl";
	const char* SPRITE_SPEED_NAME		= "CmaeraSpeed";
	const char* SPRITE_CHOICE_NAME		= "Choice";
	const char* SPRITE_NORMAL_NAME		= "CameraNormal";
	const char* SPRITE_INVERSION_NAME	= "CameraInversion";
	const char* SPRITE_ICON_NAME		= "SelectIcon";
	const float INPUT_WAIT_TIME_MAX		= 30.0;
	// 選択状態.
	enum enSelectState
	{
		ESelectState_None,

		ESelectState_Select,
		ESelectState_CameraControl,
		ESelectState_CameraSpeed,

		ESelectState_Max,
	} typedef ESelectState;

	enum enCameraControlState
	{
		ECameraControlState_None,

		ECameraControlState_Normal,		// ノーマル.
		ECameraControlState_Inversion,	// 反転.

		ECameraControlState_Max,
	} typedef ECameraControlState;

public:
	CCameraConfigWidget();
	virtual ~CCameraConfigWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// 終了したか.
	bool IsEnd();

private:
	// 種類の選択.
	void SelectType();
	// 決定.
	void Determination();
	// カメラ操作の更新.
	void CameraControlUpdate();
	// カメラ速度の更新.
	void CameraSpeedUpdate();
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;	// 画像.
	std::unique_ptr<CCursor>				m_pCursor;
	std::unique_ptr<CSlinder>				m_pSpeedSlinder;
	CRotLookAtCenter::SConfigState			m_ConfigState;
	D3DXVECTOR3								m_SlinderPosition;
	bool	m_IsOlsReverse;
	float	m_OldMoveSpeed;
	int		m_NowSelectState;
	int		m_OldNowSelectState;
	int		m_NowConfigState;
	int		m_NowControlState;
	float	m_InputWaitTime;
};

#endif	// #ifndef CAMERA_CONFIG_WIDGET_H.