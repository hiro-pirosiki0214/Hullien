#ifndef GAME_CLEAR_H
#define GAME_CLEAR_H

#include "..\..\SceneBase\SceneBase.h"

class CClearWidget;	// クリアUIクラス.
class CSTGManager;	// シューティング管理クラス.

/**********************************
*	ゲームクリアクラス.
*/
class CGameClear : public CSceneBase
{
	const int   SKIP_WAIT_COUNT = 100;
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
	// モデルの描画.
	void ModelRender();
	// シーンを切り替える.
	void OnChangeScene( const bool& isPlaySE );
	// シーン切り替え関数.
	void ChangeScene();
	// スキップの更新関数.
	void SkipUpdate();

private:
	std::unique_ptr<CClearWidget>	m_pClearWidget;	// クリアUIクラス.
	std::unique_ptr<CSTGManager>	m_pSTGManager;	// シューティング管理クラス.
	bool m_IsChangeScene;							//シーン切り替え可能か.
	int	m_SkipWaitCount;
};

#endif	// #ifndef GAME_CLEAR_H.