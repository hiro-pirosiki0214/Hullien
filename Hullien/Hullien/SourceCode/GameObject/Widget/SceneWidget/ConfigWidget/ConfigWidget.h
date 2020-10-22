#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include "..\SceneWidget.h"
#include <vector>

class CVolumeConfigWidget;	// 音量設定UIクラス.

/********************************************
*	設定UIクラス.
**/
class CConfigWidget : public CSceneWidget
{
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
	std::unique_ptr<CVolumeConfigWidget> m_pVolumeConfig;	// 音量設定UI.
};

#endif	// #ifndef CONFIG_WIDGET_H.