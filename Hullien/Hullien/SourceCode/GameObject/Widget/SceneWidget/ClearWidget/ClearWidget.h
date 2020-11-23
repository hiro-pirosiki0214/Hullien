#ifndef CLEARWIDGET_H
#define CLEARWIDGET_H

#include "..\SceneWidget.h"
#include <vector>

class CReturnTitle;

/***********************************************
*	クリアUIクラス.
**/
class CClearWidget : public CSceneWidget
{
	const float SPRITE_DIDSPPEAR_TIME = 30.0f;
	const char* SPRITE_BUTTON_NAME = "buttonB";	//スプライトのファイル名.
	const char* SPRITE_TITLE_NAME = "skip";	//スプライトのファイル名.
	const char* SPRITE_PUSH_NAME = "hold";	//スプライトのファイル名.
public:
	CClearWidget();
	virtual ~CClearWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

	inline bool IsSpriteRenderEnd(){ return m_SpriteAlpha <= 0.0f; }
	inline void SetIsSTGEnd(){ m_IsSTGEnd = true; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>> m_pSkipSprites;	// スプライトクラス.
	bool	m_IsSTGEnd;
	float	m_SpriteDisappearCount;
	float	m_SpriteAlpha;
};

#endif	//#ifndef CLEARWIDGET_H.
