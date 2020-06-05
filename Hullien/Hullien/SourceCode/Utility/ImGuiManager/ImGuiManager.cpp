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
	ImFont* font = io.Fonts->AddFontFromFileTTF(
		FONT_FILE_PATH, FONT_SIZE,
		nullptr, 
		io.Fonts->GetGlyphRangesJapanese());

	ImGuiStyle& style = ImGui::GetStyle();

	style.ScaleAllSizes( FONT_SIZE*0.06f ); // UIの大きさを一括で変更できます。
	io.FontGlobalScale = FONT_SIZE*0.06f; // フォントの大きさを一括で変更できます。

	if( ImGui_ImplWin32_Init( hWnd ) == false ) return E_FAIL;
	if( ImGui_ImplDX11_Init( pDevice11, pContext11 ) == false ) return E_FAIL;

	ImGui::StyleColorsDark();

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