#include "SpawnEdit.h"
#include "..\..\GameObject\SpawnUFO\SpawnUFOParam.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Utility\FileManager\FileManager.h"

CSpawnEdit::CSpawnEdit()
	: m_SpawnPramList		()
	, m_pStaticMesh			( nullptr )
	, m_IsSucceeded			( false )
	, m_SucceedRenderCount	( 0 )
{
}

CSpawnEdit::~CSpawnEdit()
{
}

// �������֐�.
bool CSpawnEdit::Init()
{
	if( FileReading() == false ) return false;
	if( m_pStaticMesh != nullptr ) return true;
	if( m_pStaticMesh == nullptr ){
		m_pStaticMesh = CMeshResorce::GetStatic( MODEL_NAME );
	}
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}

// �X�V�֐�.
void CSpawnEdit::Update()
{
}

// �`��֐�.
void CSpawnEdit::Render()
{

	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"�X�|�[�����ݒ�" );

	ImGui::PushItemWidth(200.0f);

	std::vector<std::string> tagNameList;
	tagNameList.emplace_back("Index");
	for( size_t i = 0; i < m_SpawnPramList.size(); i++ ){
		tagNameList.emplace_back("SpawnNo." + std::to_string(i) );
	}
	if( ImGui::BeginTabBar( "TabBarID" ) == true ){
		for( size_t i = 0; i < tagNameList.size(); i++ ){
			if( ImGui::BeginTabItem( tagNameList[i].c_str() ) == false ) continue;
			if( i == 0 ){
				IndexRender();
			} else {
				SpawnParamRender( i-1 );
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopItemWidth();
	ImGui::End();
}

// ���f���̕`��.
void CSpawnEdit::ModelRender()
{
	// ���f���̕`��.
	if( m_pStaticMesh == nullptr ) return;
	if( m_SpawnPramList.empty() == false ){
		for( const auto& p : m_SpawnPramList ){
			m_pStaticMesh->SetPosition( p.Position );
			m_pStaticMesh->SetScale( 1.0f );
			m_pStaticMesh->Render();
		}
	}
}

// �G�t�F�N�g�̕`��.
void CSpawnEdit::EffectRender()
{
}

// �t�@�C���̓ǂݍ���.
bool CSpawnEdit::FileReading()
{
	return CFileManager::BinaryVectorReading( FILE_PATH, m_SpawnPramList );
}

// �t�@�C���̏�������.
bool CSpawnEdit::FileWriting()
{
	return CFileManager::BinaryVectorWriting( FILE_PATH, m_SpawnPramList );
}

// �C���f�b�N�X�̕`��.
void CSpawnEdit::IndexRender()
{
	ImGui::TextWrapped( u8"�G�͊e�ݒ肵���ő�E�ŏ��̒l���A" );
	ImGui::TextWrapped( u8"�����_���Ŏ擾���X�|�[������" );

	ImGui::Separator();

	ImGui::Text( u8"Position : �X�|�[���|�C���g�̍��W" );
	ImGui::Text( u8"SpawnTime : �X�|�[�����鎞��" );

	ImGui::Separator();

	ImGui::Text( u8"���݂̃X�|�[������" );
	ImGui::SameLine();
	if( ImGui::Button(u8"���₷") ) m_SpawnPramList.emplace_back();
	ImGui::SameLine();
	if( ImGui::Button(u8"���炷") ) m_SpawnPramList.pop_back();
	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"�Ǎ�") == true ){
		// �f�[�^�̓ǂݍ���.
		s_Success.IsSucceeded =  FileReading();
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") == true ){
		// �f�[�^�̏�������.
		s_Success.IsSucceeded = FileWriting();
	}
	ImGui::SameLine();
	s_Success.Render();	// �������ǂ�����`��.
}

// �X�|�[�����̕`��.
void CSpawnEdit::SpawnParamRender( const int& index )
{
	if( m_SpawnPramList.empty() == true ) return;

	auto& s = m_SpawnPramList[index];

	CImGuiManager::DragFloat( u8"���W : X", &s.Position.x );
	CImGuiManager::DragFloat( u8"���W : Y", &s.Position.y );
	CImGuiManager::DragFloat( u8"���W : Z", &s.Position.z );
	CImGuiManager::DragFloat( u8"�X�|�[���|�C���g�̍���", &s.SpawnPointHight );
	CImGuiManager::DragInt( u8"�X�|�[���Ԋu����", &s.SpawnTime );
	CImGuiManager::DragInt( u8"�F���l�̍ő吔", &s.MaxAlienCount );
	CImGuiManager::DragInt( u8"�F���lC�̏o����", &s.ProbabilityC );
	CImGuiManager::DragInt( u8"�F���lD�̏o����", &s.ProbabilityD );
	CImGuiManager::DragInt( u8"�A�C�e���𗎂Ƃ��m��", &s.ProbabilityItem );

	ImGui::PushItemWidth(100); // ���ꂩ����UI�p�[�c�̕���70�ŌŒ肵�܂�.

	ImGui::PopItemWidth();
}