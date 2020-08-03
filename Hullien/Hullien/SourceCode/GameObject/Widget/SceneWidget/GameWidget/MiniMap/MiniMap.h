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
public:
	CMiniMap();
	virtual ~CMiniMap();

	// 初期化関数.
	virtual bool Init() override;

	// 更新関数.
	virtual void Update() override;
	void SetPosition(CGameActorManager* pObj);

	// 描画関数.
	virtual void Render() override;
	void Render(const D3DXMATRIX& view, const D3DXMATRIX& proj);

private:
	// 初期スプライト設定関数.
	bool InitSpriteSetting();

private:
	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;

	std::vector<std::shared_ptr<CSprite>> m_pSprite;					
};

#endif	//#ifndef MINIMAP_H.
