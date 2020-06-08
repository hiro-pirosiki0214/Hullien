#ifndef IMGUI_MANAGER_H
#define IMGUI_MANAGER_H

#include "..\..\Global.h"

#include "..\..\..\Library\ImGui\imgui.h"
#include "..\..\..\Library\ImGui\imgui_impl_dx11.h"
#include "..\..\..\Library\ImGui\imgui_impl_win32.h"

class CImGuiManager
{
	inline static const char* FONT_FILE_PATH  = "Data\\Texture\\_Font\\mplus-1p-medium.ttf";
	inline static const float FONT_SIZE = 18.0f;

public:
	struct stSuccess
	{
		int		FrameCount;		// カウント.
		int		RenderTime;		// 描画時間.
		bool	IsSucceeded;	// 成功かどうか.
		stSuccess()
			: FrameCount	( 0 )
			, RenderTime	( 60 )
			, IsSucceeded	( false )
		{}
		void Render()
		{
			if( IsSucceeded == false ) return;
			ImGui::Text( u8"成功" );
			FrameCount++;
			if( FrameCount != RenderTime ) return;
			FrameCount = 0;
			IsSucceeded = false;
		}
	} typedef SSuccess;

public:
	CImGuiManager();
	~CImGuiManager();

	// 初期化関数.
	static HRESULT Init( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// 解放関数.
	static HRESULT Release();

	// フレームの設定.
	static void SetingNewFrame();

	// 描画.
	static void Render();
};

#endif	// #ifndef IMGUI_MANAGER_H.