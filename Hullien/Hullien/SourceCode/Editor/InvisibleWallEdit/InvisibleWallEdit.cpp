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

// �������֐�.
bool CInvisibleWallEdit::Init()
{
	if( m_pInvisibleWall->Init() == false ) return false;
	return true;
}

// �X�V�֐�.
void CInvisibleWallEdit::Update()
{
}

// �`��֐�.
void CInvisibleWallEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"���Ȃ��ǂ̐ݒ�" );
	ImGui::PushItemWidth(200.0f);
	
	CImGuiManager::DragFloat( u8"�������W : X", &m_BoxWall.Length.x );
	CImGuiManager::DragFloat( u8"�������W : Z", &m_BoxWall.Length.z );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"�Ǎ�") ){
		s_success.IsSucceeded = CFileManager::BinaryReading( INVISIBLE_WALL_PARAM_FILE_NAME, m_BoxWall );
		m_pInvisibleWall->SetBoxWall( m_BoxWall );
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ){
		s_success.IsSucceeded = CFileManager::BinaryWriting( INVISIBLE_WALL_PARAM_FILE_NAME, m_BoxWall );
		m_pInvisibleWall->SetBoxWall( m_BoxWall );
	}
	ImGui::SameLine();
	s_success.Render();

	ImGui::PopItemWidth();
	ImGui::End();
}

// ���f���̕`��.
void CInvisibleWallEdit::ModelRender()
{
	m_pInvisibleWall->Render();
}

// �G�t�F�N�g�̕`��.
void CInvisibleWallEdit::EffectRender()
{
}