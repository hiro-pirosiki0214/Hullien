#ifndef RETURNTITLE_H
#define RETURNTITLE_H

#include "..\Widget.h"

/*************************************
*	タイトルに戻るボタンクラス.
**/
class CReturnTitle : public CWidget
{
private:
	const char* SPRITE_NAME = "title";	//スプライトのファイル名.
	const float	 ALPHA_SPEED = 0.01f;				//透過速度.

public:
	CReturnTitle();
	virtual ~ CReturnTitle();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;


private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

	
};
#endif	//#ifndef RETURNTITLE_H.