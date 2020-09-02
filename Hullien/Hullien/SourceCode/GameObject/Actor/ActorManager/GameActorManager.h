#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "..\..\..\Global.h"
#include "..\..\ObjectList.h"

#include <vector>

class CActor;
class CGroundStage;		// 地面クラス.
class CPlayer;					// プレイヤークラス.
class CGirl;						// 女の子クラス.
class CAlienManager;		// 宇宙人管理クラス.
class CItemManager;		// アイテム管理クラス.
class CBarrier;				// バリアクラス.
class CWarning;				//警告クラス.

class CGameActorManager
{
public:
	CGameActorManager();
	~CGameActorManager();

	// 初期化関数.
	bool Init();
	// 更新関数.
	void Update();
	// 描画関数.
	void Render();
	// スプライトの描画.
	void SpriteRender();
	// オブジェクト座標リストの取得.
	std::vector<std::pair<EObjectTag, D3DXVECTOR3>>	GetObjPositionList() const { return m_ObjPositionList; }
	// ゲームオーバーかどうか.
	bool IsGameOver();

private:
	// 座標リストの設定.
	void SetPositionList( CActor* pActor );

private:
	std::shared_ptr<CGroundStage>	m_pGroundStage;
	std::shared_ptr<CPlayer>		m_pPlayer;
	std::shared_ptr<CGirl>			m_pGirl;
	std::shared_ptr<CAlienManager>	m_pAlienManager;
	std::shared_ptr<CItemManager>	m_pItemManager;
	std::shared_ptr<CBarrier>		m_pBarrier;
	std::vector<std::pair<EObjectTag, D3DXVECTOR3>>	m_ObjPositionList;
	int m_ObjPosListCount;
};

#endif	// #ifndef CHARACTER_MANAGER_H.