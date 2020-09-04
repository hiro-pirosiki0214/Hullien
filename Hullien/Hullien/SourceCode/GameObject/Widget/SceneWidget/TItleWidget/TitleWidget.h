#ifndef CTITLEWIDGET_H
#define CTITLEWIDGET_H

#include "..\SceneWidget.h"
#include <vector>

/************************************
*	タイトルUIクラス.
**/
class CTitleWidget : public CSceneWidget
{
private:
	const char* SPRITE_BACKGROUND	= "BackGround";	//背景スプライト名.
	const char* SPRITE_SELECTSTART	= "yessize";		//開始スプライト名.
	const char* SPRITE_SELECTEXIT	= "nosize";	  			//終了スプライト名.
	const char* SPRITE_TITLE		= "titlesize";				//タイトルスプライト名.

	const int BACKGROUND	= 0;	//背景配列番号.
	const int START			= 1;	//開始配列番号.
	const int END			= 2;	//終了配列番号.

public:
	//選択状態.
	enum class enSelectState
	{
		Start,		//開始.
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
	ESelectState GetSelectState() const { return m_SelectState; }

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
