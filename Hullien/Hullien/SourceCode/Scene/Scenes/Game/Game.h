#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

class CGroundStage;		// 地面クラス.
class CPlayer;			// プレイヤークラス.
class CGirl;			// 女の子クラス.S
class CAlienManager;	// 宇宙人管理クラス.
class CItemManager;		// アイテム管理クラス.

/**********************************
*	ゲームシーンクラス.
*/
class CGame : public CSceneBase
{
public:
	CGame( CSceneManager* pSceneManager );
	virtual ~CGame();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	std::shared_ptr<CGroundStage>	m_pGroundStage;
	std::shared_ptr<CPlayer>		m_pPlayer;
	std::shared_ptr<CGirl>			m_pGirl;
	std::shared_ptr<CAlienManager>	m_pAlienManager;
	std::shared_ptr<CItemManager>	m_pItemManager;
};

#endif	// #ifndef GAME_H.