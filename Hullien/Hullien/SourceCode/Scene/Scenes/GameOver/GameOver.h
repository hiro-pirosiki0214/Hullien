#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "..\..\SceneBase\SceneBase.h"

class CGameOverWidget;

/**********************************
*	ゲームオーバークラス.
*/
class CGameOver : public CSceneBase
{
public:
	CGameOver( CSceneManager* pSceneManager );
	virtual ~CGameOver();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	std::unique_ptr<CGameOverWidget> m_pGameOverWidget;
};

#endif	// #ifndef GAME_OVER_H.