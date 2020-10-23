#ifndef VOLUME_CONFIG_WIDGET_H
#define VOLUME_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include <vector>

class CSlinder;	// スライダー.
class CCursor;	// カーソル.

/********************************************
*	音量設定UIクラス.
**/
class CVolumeConfigWidget : public CWidget
{
	const char* SPRITE_MASTER_NAME	= "MasterVolume";
	const char* SPRITE_BGM_NAME		= "BGMVolume";
	const char* SPRITE_SE_NAME		= "SEVolume";
	const char* SPRITE_ICON_NAME	= "SelectIcon";
	const char* SPRITE_RESET		= "ConfigReset";
	const char* SPRITE_SAVE			= "ConfigSave";

	const float BAR_POSITION_X = 200.0f;
	const float DEFALUT_VOLUME = 1.0f;

	inline static const float INPUT_WAIT_TIME_MAX = 30.0f;	// 入力時の待機フレーム.

	// 設定の状態.
	enum enConfigState
	{
		EConfigState_None,

		EConfigState_Select,	// 選択状態.
		EConfigState_Seting,	// 設定状態.

		EConfigState_Max,
	} typedef EConfigState;

	// 選択の種類.
	enum enSelectType
	{
		ESelectType_None = -1,

		ESelectType_Master,	// マスター.
		ESelectType_BGM,	// BGM.
		ESelectType_SE,		// SE.

		ESelectType_Reset,	// リセット.
		ESelectType_Save,	// 保存.

		EVolumeType_End,

		ESelectType_Max,

		ESelectType_Begin	= ESelectType_Master,	// 始まり.
		ESelectType_End		= ESelectType_Max,		// 終了.
		ESelectType_VolumeMax = ESelectType_SE+1
	} typedef EVolumeType;

	// スプライトの種類.
	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Master,		// マスター.
		ESpriteNo_BGM,			// BGM.
		ESpriteNo_SE,			// SE.
		ESpriteNo_Reset,		// リセット.
		ESpriteNo_Save,			// 保存.
		ESpriteNo_SelectIcon,	// 選択中のアイコン.

		ESpriteNo_Max,
	} typedef ESpriteNo;

public:
	CVolumeConfigWidget();
	virtual ~CVolumeConfigWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// 音量の設定をできるようにする.
	void OnVolumeSeting();
	// 音量の設定をできないようにする.
	void OffVolumeSeting();
	// 保存終了.
	bool IsSaveEnd();

private:
	// 種類の選択.
	void SelectType();
	// 決定.
	void Determination();
	// 音量の設定.
	void VolumeSeting();

	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::unique_ptr<CCursor>				m_pCursor;			// カーソルクラス.
	std::vector<std::shared_ptr<CSlinder>>	m_pVolumeSlinders;	// 音量スライダー.
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;			// 画像.
	std::vector<D3DXVECTOR3>				m_SlinderPositions;	// スライダーの座標.
	D3DXVECTOR3	m_CursorPosition;	// カーソルの座標.
	int			m_NowConfigState;	// 現在の設定状態.
	int			m_NowSelectVolume;	// 現在の選択している音量種類.
	int			m_OldSelectVolume;
	float		m_InputWaitTime;	// 入力した際の待機時間.
	bool		m_IsOneStep;		// 一回だけ動作.
};

#endif	// #ifndef VOLUME_CONFIG_WIDGET_H.