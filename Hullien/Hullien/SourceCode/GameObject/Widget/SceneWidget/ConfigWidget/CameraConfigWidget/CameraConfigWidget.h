#ifndef CAMERA_CONFIG_WIDGET_H
#define CAMERA_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include <vector>

class CCursor;

class CCameraConfigWidget : public CWidget
{
	const char* SPRITE_CONTROL_NAME = "CmaeraControl";
	const char* SPRITE_SPEED_NAME = "CmaeraSpeed";
	const char* SPRITE_NORMAL_NAME = "CameraNormal";
	const char* SPRITE_INVERSION_NAME = "CameraInversion";
	const char* SPRITE_CHOICE_NAME = "Choice";

	// 選択状態.
	enum enSelectState
	{
		ESelectState_None,

		ESelectState_CameraControl,
		ESelectState_CameraSpeed,

		ESelectState_Max,
	} typedef ESelectState;

	// 設定状態.
	enum enConfigState
	{
	};
public:
	CCameraConfigWidget();
	virtual ~CCameraConfigWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;	// 画像.
	std::unique_ptr<CCursor>				m_pCursor;
};

#endif	// #ifndef CAMERA_CONFIG_WIDGET_H.