#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"

// シーンの種類.
enum class enSceneNo
{
	None,

	Title,		// タイトル.
	GameMain,	// ゲームメイン.S
	GameClear,	// ゲームクリア.
	GameOver,	// ゲームオーバー.

	Edit,		// エディット.

	Max,

	ResultCheck,	// ゲームクリアかゲームオーバーの確認.
	Start = GameMain,	// 初めのシーン.

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
	// ゲームオーバーの設定.
	void OnGameOver(){ m_IsGameOver = true; }

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