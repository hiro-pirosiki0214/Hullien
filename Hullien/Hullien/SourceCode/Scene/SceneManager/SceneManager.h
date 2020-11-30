#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"
#include "..\..\GameObject\Widget\Fade\Fade.h"

class CCommand;

// シーンの種類.
enum class enSceneNo
{
	None,

	Title,		// タイトル.
	GameMain,	// ゲームメイン.
	GameClear,	// ゲームクリア.
	GameOver,	// ゲームオーバー.
	Config,		// 設定.

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
	// 設定シーンへ移動.
	void ConfigSceneMove();
	// ゲームオーバーの設定.
	inline void OnGameOver(){ m_IsGameOver = true; }
	// ゲームリトライ.
	void RetryGame();
	// HWND設定.
	inline void SethWnd(HWND hWnd) { m_hWnd = hWnd; }
	inline HWND GethWnd(){ return m_hWnd; }

	// リトライしたか.
	inline bool GetRetry() const { return m_IsRetry; }

	// 現在再生されているBGMの名前の設定.
	inline void SetNowBGMName( const char* name ){ m_NowBGMName = name; }

	// エディットシーンに切り替えれるか.
	inline void OnEditSceneChangeActive(){ m_IsEditSceneChangeActive = true; }
	inline void OffEditSceneChangeActive(){ m_IsEditSceneChangeActive = false; }

private:
	// エディットシーンに変更.
	void ChangeEditScene();
	
private:
	HWND m_hWnd;
	std::shared_ptr<CSceneBase> m_pScene;
	std::unique_ptr<CCommand>	m_pCommand;
	std::string					m_NowBGMName;
	EScene	m_NowScene;					// 現在のシーン.
	EScene	m_NextScene;				// 次のシーン.
	bool	m_IsLoadEnd;				// ロードが終了したか.
	bool	m_IsGameOver;				// ゲームオーバーか.
	bool	m_IsGameEnd;				// ゲームを終了したか.
	bool	m_IsRetry;					// リトライしたか.
	bool	m_IsChangeEditScene;		// エディットシーンに切り替えるか.
	bool	m_IsEditSceneChangeActive;	// エディットシーンに切り替えれるか.
};

#endif // #ifndef SCENE_MANAGER_H.