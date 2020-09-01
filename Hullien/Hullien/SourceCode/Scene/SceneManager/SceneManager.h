#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"

// シーンの種類.
enum class enSceneNo
{
	None,

	Title,		// タイトル.
	GameMain,	// ゲームメイン.
	GameClear,	// ゲームクリア.
	GameOver,	// ゲームオーバー.

	Edit,		// エディット.

	Max,

	ResultCheck,	// ゲームクリアかゲームオーバーの確認.
	Start = Title,	// 初めのシーン.

} typedef EScene;

/**********************************
*	シーン管理クラス.
*/
class CSceneManager
{	
public:
	CSceneManager();
	~CSceneManager();

	// 更新関数.
	void Update();
	// 次のシーンに移動.
	void NextSceneMove();
	// ゲーム終了.
	void EndGameClose();
	// ゲームオーバーの設定.
	void OnGameOver(){ m_IsGameOver = true; }
	// ゲームリトライ.
	void RetryGame();

private:
	// エディットシーンに変更.
	void ChangeEditScene();
	
private:
	std::shared_ptr<CSceneBase> m_pScene;
	EScene	m_NowScene;		// 現在のシーン.
	EScene	m_NextScene;	// 次のシーン.
	bool	m_IsLoadEnd;	// ロードが終了したか.
	bool	m_IsGameOver;	// ゲームオーバーか.
};

#endif // #ifndef SCENE_MANAGER_H.