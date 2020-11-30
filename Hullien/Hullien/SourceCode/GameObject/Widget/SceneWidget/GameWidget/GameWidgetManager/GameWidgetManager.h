#ifndef GAMEWIDGETMANAGER_H
#define GAMEWIDGETMANAGER_H

#include "..\..\..\..\..\Global.h"

class CGameActorManager;	// ゲームオブジェクト管理クラス.
class CLimitTime;				//制限時間クラス.
class CMiniMap;					//ミニマップクラス.
class CUltemateSing;			//アルティメット出現サインクラス.

/******************************************
*	ゲームUI管理クラス.
**/
class CGameWidgetManager
{
public:
	CGameWidgetManager();
	virtual ~CGameWidgetManager();

	// 初期化関数,
	bool Init();
	// 更新関数.
	void Update(CGameActorManager* pActorManager);
	// 描画関数.
	void Render();
	// ゲームが終了したか.
	bool IsGameFinish();

private:
	std::unique_ptr<CLimitTime>	m_pLimitTime;
	std::unique_ptr<CMiniMap>	m_pMiniMap;
	// スポーンUFOクラスに持って行った.
//	std::unique_ptr<CUltemateSing> m_pUltemateSing;
	
};

#endif	//#ifndef GAMEWIDGETMANAGER_H.
