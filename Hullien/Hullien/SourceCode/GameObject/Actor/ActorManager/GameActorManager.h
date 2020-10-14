#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "..\..\..\Global.h"
#include "..\..\ObjectList.h"

#include <vector>

class CGameObject;
class CSkyDome;			// 背景クラス.
class CGroundStage;		// 地面クラス.
class CPlayer;			// プレイヤークラス.
class CGirl;			// 女の子クラス.
class CMotherShipUFO;	// マザーシップUFO.
class CAlienManager;	// 宇宙人管理クラス.
class CItemManager;		// アイテム管理クラス.
class CBarrier;			// バリアクラス.
class CWarning;			// 警告クラス.
class CInvisibleWall;	// 見えない壁.

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
	// 女の子を連れ去っているか.
	bool IsGirlAbduct();
	// 女の子が危険な状態か.
	bool IsDanger();

private:
	// 座標リストの設定.
	void SetPositionList( CGameObject* pObj );

private:
	std::unique_ptr<CSkyDome>		m_pSkyDome;			// 背景.
	std::shared_ptr<CGroundStage>	m_pGroundStage;		// 地面.
	std::shared_ptr<CPlayer>		m_pPlayer;			// プレイヤー.
	std::shared_ptr<CGirl>			m_pGirl;			// 女の子.
	std::unique_ptr<CMotherShipUFO>	m_pMotherShipUFO;	// マザーシップUFO.
	std::shared_ptr<CAlienManager>	m_pAlienManager;	// 宇宙人マネージャー.
	std::shared_ptr<CItemManager>	m_pItemManager;		// アイテムマネージャー.
	std::shared_ptr<CBarrier>		m_pBarrier;			// バリア.
	std::unique_ptr<CInvisibleWall>	m_pInvisibleWall;	// 見えない壁.

	std::vector<std::pair<EObjectTag, D3DXVECTOR3>>	m_ObjPositionList;
	int m_ObjPosListCount;
};

#endif	// #ifndef CHARACTER_MANAGER_H.