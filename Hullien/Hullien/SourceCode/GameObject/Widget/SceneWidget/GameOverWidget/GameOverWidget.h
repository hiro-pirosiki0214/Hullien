#ifndef GAMEOVERWIDGET_H
#define GAMEOVERWIDGET_H

#include "..\SceneWidget.h"

class CReturnTitle;	//タイトルに戻るボタンクラス.

/********************************************
*	ゲームオーバーUIクラス.
**/
class CGameOverWidget : public CSceneWidget
{
public:
	CGameOverWidget();
	virtual ~CGameOverWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

private:
	std::unique_ptr<CReturnTitle> m_pReturnTitle;	//タイトルに戻るボタンクラス.
};

#endif	//#ifndef GAMEOVERWIDGET_H.
