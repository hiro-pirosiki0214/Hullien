#include "EditRenderer.h"
#include "..\..\Utility\ImGuiManager\ImGuiManager.h"

CEditRenderer::CEditRenderer()
	: m_RenderList	()
	, m_IsRender	( false )
{
}

CEditRenderer::~CEditRenderer()
{
}

CEditRenderer* CEditRenderer::GetInstance()
{
	static std::unique_ptr<CEditRenderer> pInstance = 
		std::make_unique<CEditRenderer>();
	return pInstance.get();
}

void CEditRenderer::PushRenderProc( std::function<void()> proc )
{
	// 関数の追加.
	GetInstance()->m_RenderList.emplace_back( proc );
}

void CEditRenderer::Render()
{
	if( GetInstance()->m_RenderList.empty() == true ) return;

	if(( GetAsyncKeyState(VK_F7) & 0x0001 ) && ( GetAsyncKeyState(VK_SHIFT) & 0x8000 )){
		GetInstance()->m_IsRender = !GetInstance()->m_IsRender;
	}

	if( GetInstance()->m_IsRender == false ){
		GetInstance()->m_RenderList.clear();	// リストの消去.
		return;
	}
	
	// ImGuiのフレーム初期化.
	CImGuiManager::SetingNewFrame();
	// リスト内を描画.
	for( auto& p : GetInstance()->m_RenderList ) p();
	// ImGui最終描画.
	CImGuiManager::Render();
	// リストの消去.
	GetInstance()->m_RenderList.clear();
}