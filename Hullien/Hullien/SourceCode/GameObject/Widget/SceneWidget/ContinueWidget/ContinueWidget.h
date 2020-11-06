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
	const char* SPRITE_CONTINUE			= "continuesize";	//コンテニュー.
	const char* SPRITE_YES				= "yes";		//イエス
	const char* SPRITE_NO				= "no";			//ノー.
	const int BACKGROUND				= 0;				//背景配列番号.
	const int YES						= 1;				//イエス配列番号.
	const int NO						= 2;				//ノー配列番号.
	const float BACKGROUND_ALPHA		= 0.7f;				//背景透過値.
	const float BACKGROUND_ALPHA_SPEED	= 0.005f;			//背景透過速度.
	const float TEXT_ALPHA_SPEED		= 0.005f;			//文字透過速度.

																			//選択状態.
	enum class enSelectState
	{
		Yes,		//イエス.
		No,		//ノー..
	} typedef ESelectState;

	// 描画順.
	enum class enDrawTurn
	{
		BackGround,	//背景.
		Text,				//文字.
	}typedef EDrawTurn;

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
	// 描画中か取得関数.
	bool GetIsDrawing() const { return m_IsDrawing; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	// カーソル設定関数.
	void CursorSetting();
	// スプライトの描画.
	void SpriteDrawing();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprite;				//スプライトクラス.
	std::shared_ptr<CCursor>					m_pCursor;				//カーソルクラス.
	float												m_TextAlpha;			//文字の透過値.
	bool												m_IsDrawing;			//描画中か.
	ESelectState									m_SelectState;		//選択状態.
	ESelectState									m_OldSelectState;	//選択前状態.
	EDrawTurn										m_DrawTurn;			//描画順.
};

#endif	//#ifndef CONTINUEWIDGET_H
