#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

class CGameActorManager;	// ゲームオブジェクト管理クラス.
class CGameWidgetManager;	// UI管理クラス.
class CContinueWidget;		// コンテニューUIクラス.
class CSkyDome;					// 背景クラス.
class CEventManager;			//イベント管理クラス.


/**********************************
*	ゲームシーンクラス.
*/
class CGame : public CSceneBase
{
private:
	// シーン切り替え状態.
	enum class enChangeSceneState
	{
		None, 

		Game,			//ゲーム.
		Clear,				//クリア.
		GameOver,		//ゲームオーバー.
	}typedef EChangeSceneState;

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
	// フェード初期化関数.
	bool InitFade();	
	// コンテニュー処理関数.
	void UpdateContinue();
	// シーン切り替え関数.
	void ChangeScene();
	// シーンの選択.
	void SelectScene();
	// シーン切り替え設定関数.
	void SetChangeScene( const EChangeSceneState& changeState );


private:
	std::unique_ptr<CGameActorManager>		m_GameObjManager;	// ゲームオブジェクト管理クラス.
	std::unique_ptr<CGameWidgetManager>	m_WidgetManager;		// ゲームUI管理クラス.
	std::unique_ptr<CContinueWidget>			m_ContinueWidget;		// コンテニューUIクラス.
	std::unique_ptr<CSkyDome>					m_pSkyDome;				// 背景.
	std::unique_ptr<CEventManager>			m_pEventManager;		//	イベント管理クラス.
	EChangeSceneState							m_ChangeSceneState;	// シーン切り替え状態.
	bool													m_IsChangeScene;		// シーン切り替えが可能か.

};

#endif	// #ifndef GAME_H.