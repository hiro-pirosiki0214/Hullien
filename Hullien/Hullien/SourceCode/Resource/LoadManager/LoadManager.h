#ifndef LOAD_MANAGER_H
#define LOAD_MANAGER_H

#include "..\..\Global.h"
#include <thread>
#include <mutex>

#include <memory>
#include <vector>

/*********************************
*	各リソース類の読み込みをまとめるクラス.
*		Load画像などの表示をここに入れてもいいかも.
*/
class CLoadManager
{
	const char* LOAD_BACK_IMAGE = "Data\\Texture\\_LoadTexture\\LoadBackImage.png";
	const char* LOAD_ANIM_IMAGE = "Data\\Texture\\_LoadTexture\\LoadAnim.png";
	const char* LOAD_TEXT_IMAGE = "Data\\Texture\\_LoadTexture\\LoadText.png";

public:
	CLoadManager();
	~CLoadManager();

	// 読み込み.
	void LoadResource( 
		HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9  );
	// スレッドの解放.
	bool ThreadRelease();

	// 描画.
	void Render();

private:
	// スプライトの取得.
	bool GetSprite( 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11 );

private:
	std::thread m_Thread;		// スレッド.
	std::vector<std::shared_ptr<CSprite>>	m_Sprites;
	std::mutex	m_Mutex;
	
	bool m_isLoadEnd;			// ロードが終了したか.
	bool m_isThreadJoined;		// スレッドが解放されたか.
};

#endif	// #ifndef LOAD_MANAGER_H.