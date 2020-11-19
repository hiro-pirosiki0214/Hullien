#include "ImGuiManager.h"

CImGuiManager::CImGuiManager()
{
}

CImGuiManager::~CImGuiManager()
{
}

//----------------------.
// 初期化関数.
//----------------------.
HRESULT CImGuiManager::Init( 
	HWND hWnd, 
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// フォントを読み込む.
	ImFont* font = io.Fonts->AddFontFromFileTTF(
		FONT_FILE_PATH, FONT_SIZE,
		nullptr, 
		io.Fonts->GetGlyphRangesJapanese());

	// ゲームパッドの使用を許可する.
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGuiStyle& style = ImGui::GetStyle();

	style.ScaleAllSizes( FONT_SIZE*0.06f ); // UIの大きさを一括で変更できます。
	io.FontGlobalScale = FONT_SIZE*0.06f; // フォントの大きさを一括で変更できます。
	ImGui::StyleColorsDark();

	style.WindowRounding = 0.2f;
	style.Colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.54f);
	style.Colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.54f);
	style.Colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
	style.Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg]                = ImVec4(0.16f, 0.29f, 0.29f, 0.54f);
	style.Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.26f, 0.59f, 0.59f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.26f, 0.59f, 0.59f, 0.67f);
	style.Colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark]              = ImVec4(0.99f, 0.99f, 0.99f, 0.51f);
	style.Colors[ImGuiCol_SliderGrab]             = ImVec4(0.59f, 0.59f, 0.59f, 0.31f);
	style.Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_Button]                 = ImVec4(0.59f, 0.59f, 0.59f, 0.31f);
	style.Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.59f, 0.59f, 0.70f, 0.80f);
	style.Colors[ImGuiCol_ButtonActive]           = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.98f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered]          = style.Colors[ImGuiCol_ButtonHovered];
	style.Colors[ImGuiCol_HeaderActive]           = style.Colors[ImGuiCol_ButtonActive];
	style.Colors[ImGuiCol_Separator]              = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	style.Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_Tab]                    = ImVec4(0.59f, 0.59f, 0.59f, 0.31f);
	style.Colors[ImGuiCol_TabHovered]             = style.Colors[ImGuiCol_HeaderHovered];
	style.Colors[ImGuiCol_TabActive]              = style.Colors[ImGuiCol_ButtonActive];
	style.Colors[ImGuiCol_TabUnfocused]           = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
	style.Colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.59f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.59f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	style.Colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	if( ImGui_ImplWin32_Init( hWnd ) == false ) return E_FAIL;
	if( ImGui_ImplDX11_Init( pDevice11, pContext11 ) == false ) return E_FAIL;

	return S_OK;
}

//----------------------.
// 解放関数.
//----------------------.
HRESULT CImGuiManager::Release()
{
	// ImGuiの解放.
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	return S_OK;
}

//----------------------.
// フレームの設定.
//----------------------.
void CImGuiManager::SetingNewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

//----------------------.
// 描画.
//----------------------.
void CImGuiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
}

void CImGuiManager::DragFloat( const char* label, float* v, const float& speed  )
{
	ImGui::DragFloat( label, v, speed, -1000.0f, 1000.0f, "%.5f" );
}

void CImGuiManager::DragInt( const char* label, int* v, const float& speed )
{
	ImGui::DragInt( label, v, speed, -1000, 1000 );
}
