#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include "..\SceneWidget.h"
#include <vector>

class CVolumeBar;

/********************************************
*	設定UIクラス.
**/
class CConfigWidget : public CSceneWidget
{
	const char* SPRITE_MASTER_NAME = "MasterVolume";
	const char* SPRITE_BGM_NAME = "BGMVolume";
	const char* SPRITE_SE_NAME = "SEVolume";
	const char* SPRITE_CURSOR_NAME = "VolumeCursor";

	// スプライト番号.
	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Master,
		ESpriteNo_BGM,
		ESpriteNo_SE,

		ESpriteNo_Max,
	} typedef ESpriteNo;

public:
	CConfigWidget();
	virtual ~CConfigWidget();

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
	std::vector<std::shared_ptr<CVolumeBar>>	m_pVolumeBars;
	std::vector<std::shared_ptr<CSprite>>		m_pSprites;
	std::shared_ptr<CSprite>					m_pCursor;
	D3DXVECTOR3 m_CursorPosition;
	int	m_NowSelectVolume;
};

#endif	// #ifndef CONFIG_WIDGET_H.