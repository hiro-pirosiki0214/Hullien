#ifndef IMGUI_MANAGER_H
#define IMGUI_MANAGER_H

#include "..\..\Global.h"

#include "..\..\..\Library\ImGui\imgui.h"
#include "..\..\..\Library\ImGui\imgui_impl_dx11.h"
#include "..\..\..\Library\ImGui\imgui_impl_win32.h"
#include "..\..\..\Library\ImGui\imgui_internal.h"

class CImGuiManager
{
	inline static const char* FONT_FILE_PATH  = "Data\\Texture\\_Font\\mplus-1p-medium.ttf";
	inline static const float FONT_SIZE = 18.0f;

public:
	struct stSuccess
	{
		int		FrameCount;		// �J�E���g.
		int		RenderTime;		// �`�掞��.
		bool	IsSucceeded;	// �������ǂ���.
		stSuccess()
			: FrameCount	( 0 )
			, RenderTime	( 60 )
			, IsSucceeded	( false )
		{}
		void Render()
		{
			if( IsSucceeded == false ) return;
			ImGui::Text( u8"����" );
			ImGui::SameLine();
			FrameCount++;
			if( FrameCount != RenderTime ) return;
			FrameCount = 0;
			IsSucceeded = false;
		}
	} typedef SSuccess;

public:
	CImGuiManager();
	~CImGuiManager();

	// �������֐�.
	static HRESULT Init( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// ����֐�.
	static HRESULT Release();

	// �t���[���̐ݒ�.
	static void SetingNewFrame();

	// �`��.
	static void Render();

	// �h���b�O�̕\��.
	static void DragFloat( const char* label, float* v, const float& speed = 0.001f );
	static void DragInt( const char* label, int* v, const float& speed = 1.0f );
};

#endif	// #ifndef IMGUI_MANAGER_H.