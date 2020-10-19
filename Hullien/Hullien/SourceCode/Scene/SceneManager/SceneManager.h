#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"
#include "..\..\GameObject\Widget\Fade\Fade.h"

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

	ResultCheck,		// ゲームクリアかゲームオーバーの確認.
	Start = Title,		// 初めのシーン.

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
	// HWND設定.
	void SethWnd(HWND hWnd) { m_hWnd = hWnd; }
	HWND GethWnd(){ return m_hWnd; }

	// リトライしたか.
	bool GetRetry() const { return m_IsRetry; }

private:
	// エディットシーンに変更.
	void ChangeEditScene();
	
private:
	HWND m_hWnd;
	std::shared_ptr<CSceneBase> m_pScene;
	EScene	m_NowScene;		// 現在のシーン.
	EScene	m_NextScene;	// 次のシーン.
	bool	m_IsLoadEnd;	// ロードが終了したか.
	bool	m_IsGameOver;	// ゲームオーバーか.
	bool	m_IsGameEnd;	// ゲームを終了したか.
	bool	m_IsRetry;		// リトライしたか.
};

#endif // #ifndef SCENE_MANAGER_H.