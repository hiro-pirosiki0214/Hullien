#ifndef WARNING_H
#define WARNING_H

#include "..\GameWidget.h"

/*************************************
*	警告クラス.
**/
class CWarning : public CGameWidget
{
public:
	CWarning();
	virtual ~CWarning();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

private:
};


#endif	//#ifndef WARNING_H.