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

};

#endif	//#ifndef FADE_H.