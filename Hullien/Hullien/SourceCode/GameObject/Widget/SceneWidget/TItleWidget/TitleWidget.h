#ifndef CTITLEWIDGET_H
#define CTITLEWIDGET_H

#include "..\SceneWidget.h"
#include <vector>

//#define IS_CONFIG_RENDER

/************************************
*	タイトルUIクラス.
**/
class CTitleWidget : public CSceneWidget
{
private:
	const char* SPRITE_BACKGROUND	= "BackGround";	//背景スプライト名.
	const char* SPRITE_SELECTSTART	= "start";	//開始スプライト名.
	const char* SPRITE_SELECTCONFIG	= "Config";		//設定スプライト名.
	const char* SPRITE_SELECTEXIT	= "exit";	  	//終了スプライト名.
	const char* SPRITE_TITLE		= "hulianTitle";	//タイトルスプライト名.

	const int BACKGROUND	= 0;	//背景配列番号.
	const int START			= 1;	//開始配列番号.
#ifndef IS_CONFIG_RENDER
	const int CONFIG		= 2;	//開始配列番号.
	const int END			= 3;	//終了配列番号.
#else	// #ifndef IS_CONFIG_RENDER.
	const int END			= 2;	//終了配列番号.
#endif	// #ifndef IS_CONFIG_RENDER.

public:
	//選択状態.
	enum class enSelectState
	{
		Start,		//開始.
#ifndef IS_CONFIG_RENDER
		Config,		//設定.
#endif	// #ifndef IS_CONFIG_RENDER.
		End,		//終了.
	} typedef ESelectState;

public:
	CTitleWidget();
	virtual ~CTitleWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

	// 選択状態の取得関数.
	inline ESelectState GetSelectState() const { return m_SelectState; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	// カーソル設定関数.
	void CursorSetting();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprite;				//スプライトクラス.
	std::shared_ptr<CCursor>					m_pCursor;				//カーソルクラス.
	ESelectState									m_SelectState;		//選択状態.
	ESelectState									m_OldSelectState;	//選択状態.

};

#endif	//#ifndef CTITLEWIDGET_H
