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
	// �֐��̒ǉ�.
	GetInstance()->m_RenderList.emplace_back( proc );
}

void CEditRenderer::Render()
{
	if( GetInstance()->m_RenderList.empty() == true ) return;

	if(( GetAsyncKeyState(VK_F7) & 0x0001 ) && ( GetAsyncKeyState(VK_SHIFT) & 0x8000 )){
		GetInstance()->m_IsRender = !GetInstance()->m_IsRender;
	}

	if( GetInstance()->m_IsRender == false ){
		GetInstance()->m_RenderList.clear();	// ���X�g�̏���.
		return;
	}
	
	// ImGui�̃t���[��������.
	CImGuiManager::SetingNewFrame();
	// ���X�g����`��.
	for( auto& p : GetInstance()->m_RenderList ) p();
	// ImGui�ŏI�`��.
	CImGuiManager::Render();
	// ���X�g�̏���.
	GetInstance()->m_RenderList.clear();
}