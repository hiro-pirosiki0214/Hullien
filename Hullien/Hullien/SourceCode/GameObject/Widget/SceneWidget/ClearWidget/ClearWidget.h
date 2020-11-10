#ifndef CLEARWIDGET_H
#define CLEARWIDGET_H

#include "..\SceneWidget.h"

class CReturnTitle;

/***********************************************
*	クリアUIクラス.
**/
class CClearWidget : public CSceneWidget
{
	const float SPRITE_DIDSPPEAR_TIME = 30.0f;
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
	std::unique_ptr<CReturnTitle> m_pReturTitle;	//タイトルに戻るボタンクラス.
	bool	m_IsSTGEnd;
	float	m_SpriteDisappearCount;
	float	m_SpriteAlpha;
};

#endif	//#ifndef CLEARWIDGET_H.
