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
	const float GIRL_LIMIT_POS_Y = 5.0f;	//女の子がUFOに吸い込まれた時の限界値.
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
	inline std::vector<std::pair<EObjectTag, std::pair<D3DXVECTOR3, float>>>	GetObjPositionList() const { return m_ObjPositionList; }
	// ゲームオーバーかどうか.
	bool IsGameOver();
	// 女の子を連れ去っているか.
	bool IsGirlAbduct();
	// 女の子が危険な状態か.
	bool IsDanger();
	// 宇宙人が帰還したか.
	bool IsReturnAlien();

private:
	// 座標リストの設定.
	void SetPositionList( CGameObject* pObj );
	// アニメーションを止める.
	void AnimationStop();

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

	std::vector<std::pair<EObjectTag, std::pair<D3DXVECTOR3, float>>>	m_ObjPositionList;
	int		m_ObjPosListCount;		// オブジェクトの座標リストの数.
	bool	m_IsOllAnimationStop;	// 全部のアニメーションが停止したか.
};

#endif	// #ifndef CHARACTER_MANAGER_H.