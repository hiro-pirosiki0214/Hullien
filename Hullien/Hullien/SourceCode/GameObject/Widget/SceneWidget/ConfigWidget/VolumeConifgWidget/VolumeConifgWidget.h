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

	const float BAR_POSITION_X = 200.0f;

	// ボリュームの種類.
	enum enVolumeType
	{
		EVolumeType_None = -1,

		EVolumeType_Master,	// マスター.
		EVolumeType_BGM,	// BGM.
		EVolumeType_SE,		// SE.

		EVolumeType_Max,

		EVolumeType_Begin	= EVolumeType_Master,	// 始まり.
		EVolumeType_End		= EVolumeType_Max,		// 終了.
	} typedef EVolumeType;

public:
	CVolumeConfigWidget();
	virtual ~CVolumeConfigWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	// 音量種類の選択.
	void SelectVolumeType();
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
	int			m_NowSelectVolume;	// 現在の選択している音量種類.
};

#endif	// #ifndef VOLUME_CONFIG_WIDGET_H.