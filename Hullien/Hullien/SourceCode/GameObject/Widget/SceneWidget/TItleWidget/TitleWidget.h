#ifndef CTITLEWIDGET_H
#define CTITLEWIDGET_H

#include "..\SceneWidget.h"
#include <vector>

/************************************
*	タイトルUIクラス.
**/
class CTitleWidget : public CSceneWidget
{
	const int BACKGROUND	= 0;	//背景配列番号.
	const int START				= 1;   //開始配列番号.
	const int END					= 2;   //終了配列番号.

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
	// カーソル設定関数.
	void SettingCursor();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprites;			//スプライトクラス.
	std::unique_ptr<CCursor>					m_pCursor;			//カーソルクラス.
	ESelectState									m_SelectState;	//選択状態.

};

#endif	//#ifndef CTITLEWIDGET_H
