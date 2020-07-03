#ifndef MINIMAP_H
#define MINIMAP_H

#include "..\GameWidget.h"

/**********************************
*	ミニマップクラス.
**/
class CMiniMap : public CGameWidget
{
public:
	CMiniMap();
	virtual ~CMiniMap();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

private:
};

#endif	//#ifndef MINIMAP_H.
