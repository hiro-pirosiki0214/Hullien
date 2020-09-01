#ifndef GAME_CLEAR_H
#define GAME_CLEAR_H

#include "..\..\SceneBase\SceneBase.h"

class CClearWidget;	// クリアUIクラス.
class CFade;			// フェードクラス.

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

private:
	std::unique_ptr<CClearWidget> m_pClearWidget;	// クリアUIクラス.
	std::unique_ptr<CFade>			  m_pFade;				// フェードクラス.
	bool									  m_IsChangeScene;	//シーン切り替えが可能か.
};

#endif	// #ifndef GAME_CLEAR_H.