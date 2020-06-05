#ifndef LOAD_MANAGER_H
#define LOAD_MANAGER_H

#include "..\..\Global.h"
#include <thread>

/*********************************
*	各リソース類の読み込みをまとめるクラス.
*		Load画像などの表示をここに入れてもいいかも.
*/
class CLoadManager
{
public:
	CLoadManager();
	~CLoadManager();

	// 読み込み.
	void LoadResource( HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9  );
	// スレッドの解放.
	bool ThreadRelease();

private:
	std::thread m_Thread;		// スレッド.

	bool m_isLoadEnd;			// ロードが終了したか.
	bool m_isThreadJoined;		// スレッドが解放されたか.
};

#endif	// #ifndef LOAD_MANAGER_H.