#ifndef CONTINUEWIDGET_H
#define CONTINUEWIDGET_H

#include "..\SceneWidget.h"
#include <vector>

/********************************************
*	コンテニューUIクラス.
**/
class CContinueWidget : public CSceneWidget
{
public:
	const char* SPRITE_BACK				= "CBackGround";	//背景
	const char* SPRITE_CONTINUE		= "continuesize";		//コンテニュー.
	const char* SPRITE_YES					= "yessize";			//イエス
	const char* SPRITE_NO					= "nosize";				//ノー.
	const int BACKGROUND					= 0;						//背景配列番号.
	const int YES									= 1;						//イエス配列番号.
	const int NO									= 2;						//ノー配列番号.
	const float BACKGROUND_ALPHA		= 0.5f;					//背景透過値.

	//選択状態.
	enum class enSelectState
	{
		Yes,		//イエス.
		No,		//ノー..
	} typedef ESelectState;

public:
	CContinueWidget();
	virtual ~CContinueWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// 選択状態の取得関数.
	ESelectState GetSelectState() const { return m_SelectState; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	// カーソル設定関数.
	void CursorSetting();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprite;			//スプライトクラス.
	std::shared_ptr<CCursor>					m_pCursor;			//カーソルクラス.
	ESelectState									m_SelectState;	//選択状態.
};

#endif	//#ifndef CONTINUEWIDGET_H
