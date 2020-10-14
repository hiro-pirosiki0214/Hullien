#include "InvisibleWallEdit.h"
#include "..\..\GameObject\InvisibleWall\InvisibleWall.h"
#include "..\..\Utility\FileManager\FileManager.h"

CInvisibleWallEdit::CInvisibleWallEdit()
	: m_pInvisibleWall	( nullptr )
	, m_BoxWall			()
{
	m_pInvisibleWall	= std::make_unique<CInvisibleWall>();
	m_BoxWall.Length.y	= 50.0f;
}

CInvisibleWallEdit::~CInvisibleWallEdit()
{
}

// 初期化関数.
bool CInvisibleWallEdit::Init()
{
	if( m_pInvisibleWall->Init() == false ) return false;
	return true;
}

// 更新関数.
void CInvisibleWallEdit::Update()
{
}

// 描画関数.
void CInvisibleWallEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"見ない壁の設定" );
	ImGui::PushItemWidth(200.0f);
	
	ImGui::InputFloat( u8"初期座標 : X", &m_BoxWall.Length.x );
	ImGui::InputFloat( u8"初期座標 : Z", &m_BoxWall.Length.z );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"読込") ){
		s_success.IsSucceeded = CFileManager::BinaryReading( INVISIBLE_WALL_PARAM_FILE_NAME, m_BoxWall );
		m_pInvisibleWall->SetBoxWall( m_BoxWall );
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") ){
		s_success.IsSucceeded = CFileManager::BinaryWriting( INVISIBLE_WALL_PARAM_FILE_NAME, m_BoxWall );
		m_pInvisibleWall->SetBoxWall( m_BoxWall );
	}
	ImGui::SameLine();
	s_success.Render();

	ImGui::PopItemWidth();
	ImGui::End();
}

// モデルの描画.
void CInvisibleWallEdit::ModelRender()
{
	m_pInvisibleWall->Render();
}