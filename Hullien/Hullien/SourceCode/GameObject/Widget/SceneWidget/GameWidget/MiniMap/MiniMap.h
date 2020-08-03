#ifndef MINIMAP_H
#define MINIMAP_H

#include "..\GameWidget.h"
#include <vector>

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
	// 描画関数.
	virtual void Render() override;

private:
	// スプライト設定関数.
	bool SpriteSetting();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	//スプライトクラス.


};

#endif	//#ifndef MINIMAP_H.
