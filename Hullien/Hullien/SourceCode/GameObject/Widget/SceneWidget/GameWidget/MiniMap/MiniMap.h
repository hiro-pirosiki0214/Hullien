#ifndef MINIMAP_H
#define MINIMAP_H

#include "..\GameWidget.h"
#include <vector>

class CGameActorManager;	//アクタ管理クラス.

/**********************************
*	ミニマップクラス.
**/
class CMiniMap : public CGameWidget
{
private:
	typedef std::vector<std::pair<EObjectTag, D3DXVECTOR3>> OBJLIST;

	const char* SPRITE_MAP_BACK = "minimapsize";		//ミニマップ背景スプライト名.
	const char* SPRITE_DEFAULT_ICON = "mapiconsize";	//標準アイコンスプライト名.
	const int	 MAP_BACK = 0;										//ミニマップ背景配列番号.

public:
	CMiniMap();
	virtual ~CMiniMap();

	// 初期化関数.
	virtual bool Init() override;

	// 更新関数.
	virtual void Update() override;
	// オブジェクト位置設定関数.
	void SetObjPosition(CGameActorManager* pObj);

	// 描画関数.
	virtual void Render() override;

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	//スプライト設定関数.
	void SpriteSetting(OBJLIST objList);
	// オブジェクトのスプライト設定関数.
	void ObjSpriteSetting(const char* spriteName);

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;
	std::vector<D3DXVECTOR3>	m_vPosition;
	size_t m_ObjPosListCount;

};

#endif	//#ifndef MINIMAP_H.
