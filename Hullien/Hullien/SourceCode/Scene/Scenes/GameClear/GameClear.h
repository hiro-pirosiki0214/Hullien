#ifndef GAME_CLEAR_H
#define GAME_CLEAR_H

#include "..\..\SceneBase\SceneBase.h"

/**********************************
*	ゲームクリアクラス.
*/
class CGameClear : public CSceneBase
{
public:
	CGameClear( CSceneManager* pSceneManager );
	virtual ~CGameClear();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
};

#endif	// #ifndef GAME_CLEAR_H.