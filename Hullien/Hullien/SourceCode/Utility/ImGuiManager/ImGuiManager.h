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
	CImGuiManager();
	~CImGuiManager();

	// ‰Šú‰»ŠÖ”.
	static HRESULT Init( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// ‰ğ•úŠÖ”.
	static HRESULT Release();

	// ƒtƒŒ[ƒ€‚Ìİ’è.
	static void SetingNewFrame();

	// •`‰æ.
	static void Render();
};

#endif	// #ifndef IMGUI_MANAGER_H.