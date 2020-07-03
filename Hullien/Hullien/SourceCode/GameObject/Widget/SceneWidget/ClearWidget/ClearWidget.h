#ifndef CLEARWIDGET_H
#define CLEARWIDGET_H

#include "..\SceneWidget.h"

class CReturnTitle;

/***********************************************
*	クリアUIクラス.
**/
class CClearWidget : public CSceneWidget
{
public:
	CClearWidget();
	virtual ~CClearWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

private:
	std::unique_ptr<CReturnTitle> m_pReturTitle;	//タイトルに戻るボタンクラス.
};

#endif	//#ifndef CLEARWIDGET_H.
