#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include "..\SceneWidget.h"
#include <vector>

class CCursor;
class CVolumeConfigWidget;		// 音量設定UIクラス.
class CCameraConfigWidget;		// カメラ設定UIクラス.
class CControllerConfigWidget;	// コントローラー設定UIクラス.

/********************************************
*	設定UIクラス.
**/
class CConfigWidget : public CSceneWidget
{

	const char* SPRITE_BACK = "ConfigBack";
	const char* SPRITE_VOLUME = "Volume";
	const char* SPRITE_CAMERA = "Camera";
	const char* SPRITE_CONTROLLER = "Controller";

	const int BACKGROUND	= 0;	//背景配列番号.
	const int VOLUME		= 1;	//開始配列番号.
	const int CAMERA		= 2;	//開始配列番号.
	const int CONTROLLER	= 3;	// 

	enum enConfigState
	{
		EConfigState_None,

		EConfigState_Volume,	// 音量の設定.
		EConfigState_Camera,	// カメラの操作.
		EConfigState_Controller,// コントローラーの設定.

		EConfigState_End,

		EConfigState_Max,
	} typedef EConfigState;

public:
	CConfigWidget();
	virtual ~CConfigWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// 音量の設定をできないようにする.
	void OffVolumeSeting();

	// 設定を終了したか.
	bool IsEndConfig();

private:
	// カーソルの設定.
	void CursorSetting();
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
	std::unique_ptr<CCursor>				m_pCursor;			// カーソルクラス.
	std::unique_ptr<CVolumeConfigWidget>	m_pVolumeConfig;	// 音量設定UI.
	std::unique_ptr<CCameraConfigWidget>	m_pCameraConfig;	// カメラ設定UI.
	std::unique_ptr<CControllerConfigWidget>m_pControllerConfig;
	int m_SelectState;		// 選択状態.
	int	m_NowConfigState;	// 現在の設定の状態.
};

#endif	// #ifndef CONFIG_WIDGET_H.