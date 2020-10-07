#ifndef EVENT_WIDGET_H
#define EVENT_WIDGET_H

#include "..\Widget.h"

/*************************************
*	イベントUI元クラス.
**/
class CEventWidget : public CWidget
{
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
	void SetSkip(const bool& skip) { m_IsSkip = skip; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() { return true; }

private:
	std::shared_ptr<CSprite> m_pSprite;	// スプライトクラス.
	bool					 m_IsSkip;	// スキップしたか.
	float					 m_Alpha;	// 透過値.
};

#endif	//#ifndef EVENT_WIDGET_H.
