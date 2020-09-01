#ifndef FADE_H
#define FADE_H

#include "..\Widget.h"

/************************************
*	フェードクラス.
**/
class CFade : public CWidget
{

public:
	CFade();
	virtual ~CFade();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;
	// フェード中か取得関数.
	bool GetIsFade() const { return m_IsFade; }
	// フェード状態取得関数.
	EFadeState GetFadeState() const { return m_FadeState; }


private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	bool m_IsFade;		//フェード中か.

};

#endif	//#ifndef FADE_H.