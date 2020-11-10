#ifndef EVENT_WIDGET_H
#define EVENT_WIDGET_H

#include "..\Widget.h"

#include <vector>

/*************************************
*	イベントUI元クラス.
**/
class CEventWidget : public CWidget
{
	const char* SPRITE_BUTTON_NAME = "buttonY";	//スプライトのファイル名.
	const char* SPRITE_TITLE_NAME = "skip";	//スプライトのファイル名.
	const char* SPRITE_PUSH_NAME = "hold";	//スプライトのファイル名.
public:
	CEventWidget();
	virtual ~CEventWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// スキップしたか設定関数.
	inline void SetSkip(const bool& skip) { m_IsSkip = skip; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSkipSprites;	// スプライトクラス.
	bool					 m_IsSkip;	// スキップしたか.
	float					 m_Alpha;	// 透過値.
};

#endif	//#ifndef EVENT_WIDGET_H.
