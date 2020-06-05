#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "..\..\SceneBase\SceneBase.h"

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
};

#endif	// #ifndef GAME_OVER_H.