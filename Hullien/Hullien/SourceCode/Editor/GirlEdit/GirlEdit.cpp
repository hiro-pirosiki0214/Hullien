#include "GirlEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

CGirlEdit::CGirlEdit()	
	: m_pGirl		( nullptr )
	, m_GirlParam	()
{
	m_pGirl = std::make_unique<CGirl>();
}

CGirlEdit::~CGirlEdit()
{
}

// �������֐�.
bool CGirlEdit::Init() 
{
	if( m_pGirl->Init() == false ) return false;
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_GirlParam ) == false ) return false;
	return true;
}

// �X�V�֐�.
void CGirlEdit::Update()
{
	m_pGirl->Update();
}

// �`��֐�.
void CGirlEdit::Render() 
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"���̎q�ݒ�" );

	ImGui::PushItemWidth(200.0f);

	// �e�p�����[�^�̐ݒ�.
	CImGuiManager::DragFloat( u8"�ړ����x",		&m_GirlParam.MoveSpeed );
	CImGuiManager::DragFloat( u8"��]���x",		&m_GirlParam.RotatlonalSpeed );
	CImGuiManager::DragFloat( u8"�������W : X",	&m_GirlParam.InitPosition.x );
	CImGuiManager::DragFloat( u8"�������W : Y",	&m_GirlParam.InitPosition.y );
	CImGuiManager::DragFloat( u8"�������W : Z",	&m_GirlParam.InitPosition.z );
	CImGuiManager::DragFloat( u8"�������W�̋��e�̒���",	&m_GirlParam.InitPosLenght );
	CImGuiManager::DragFloat( u8"���G�͈͂̔��a",			&m_GirlParam.SearchCollRadius );
	CImGuiManager::DragFloat( u8"�X�t�B�A�����p���W : X",	&m_GirlParam.SphereAdjPos.x );
	CImGuiManager::DragFloat( u8"�X�t�B�A�����p���W : Y",	&m_GirlParam.SphereAdjPos.y );
	CImGuiManager::DragFloat( u8"�X�t�B�A�����p���W : Z",	&m_GirlParam.SphereAdjPos.z );
	CImGuiManager::DragFloat( u8"�X�t�B�A�����p���a",		&m_GirlParam.SphereAdjRadius );

	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"�Ǎ�") == true ){
		// �f�[�^�̓ǂݍ���.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_GirlParam );
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") == true ){
		// �f�[�^�̏�������.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_GirlParam );
	}
	ImGui::SameLine();
	s_Success.Render();	// �������ǂ�����`��.

	ImGui::PopItemWidth();
	ImGui::End();
}

// ���f���̕`��.
void CGirlEdit::ModelRender() 
{
	m_pGirl->Render();
}

// �G�t�F�N�g�̕`��.
void CGirlEdit::EffectRender()
{
	m_pGirl->EffectRender();
}