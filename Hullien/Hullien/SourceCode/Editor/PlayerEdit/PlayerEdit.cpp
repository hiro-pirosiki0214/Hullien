#include "PlayerEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\XInput\XInput.h"

CPlayerEdit::CPlayerEdit()	
	: m_pPlayer			( nullptr )
	, m_pPlayerParam	()
{
	m_pPlayer = std::make_unique<CPlayer>();
}

CPlayerEdit::~CPlayerEdit()
{
}

// �������֐�.
bool CPlayerEdit::Init() 
{
	if( m_pPlayer->Init() == false ) return false;
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_pPlayerParam ) == false ) return false;
	return true;
}

// �X�V�֐�.
void CPlayerEdit::Update()
{
	if( m_IsSetCamera == true ){
		m_pPlayer->Update();
	}
	if( CXInput::Back_Button() == CXInput::enSEPARATED ){
		m_IsSetCamera = false;
		// �Q�[���p�b�h�̎g�p��������.
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	}
}

// �`��֐�.
void CPlayerEdit::Render() 
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"�v���C���[�̐ݒ�" );

	ImGui::PushItemWidth(200.0f);

	// �e�p�����[�^�̐ݒ�.
	CImGuiManager::DragFloat( u8"�ړ����x",				&m_pPlayerParam.MoveSpeed );
	CImGuiManager::DragFloat( u8"�̗�",					&m_pPlayerParam.LifeMax );
	CImGuiManager::DragFloat( u8"�U����",				&m_pPlayerParam.AttackPower );
	CImGuiManager::DragInt(   u8"���G����",				&m_pPlayerParam.InvincibleTime );
	CImGuiManager::DragFloat( u8"����\�͍ő�l",		&m_pPlayerParam.SpecialAbilityMax );
	CImGuiManager::DragFloat( u8"����\�͉񕜒l",		&m_pPlayerParam.SpecialAbilityValue );
	CImGuiManager::DragInt(   u8"�U���R���{�ő吔",		&m_pPlayerParam.AttackComboMax );
	CImGuiManager::DragInt(   u8"�U���L���[�ǉ��ő吔",	&m_pPlayerParam.AttackQueueMax );
	CImGuiManager::DragFloat( u8"����̈ړ�����",		&m_pPlayerParam.AvoidMoveDistance );
	CImGuiManager::DragFloat( u8"���p�̈ړ����x",		&m_pPlayerParam.AvoidMoveSpeed );
	CImGuiManager::DragFloat( u8"�J�����̈ړ����x",		&m_pPlayerParam.CameraMoveSpeed );
	CImGuiManager::DragFloat( u8"�J�����̋���",			&m_pPlayerParam.CameraDistance );
	CImGuiManager::DragFloat( u8"�J�����̍���",			&m_pPlayerParam.CameraHeight );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W X",	&m_pPlayerParam.SphereAdjPos.x );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W Y",	&m_pPlayerParam.SphereAdjPos.y );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W Z",	&m_pPlayerParam.SphereAdjPos.z );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������a",	&m_pPlayerParam.SphereAdjRadius );
	


	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"�Ǎ�") == true ){
		// �f�[�^�̓ǂݍ���.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_pPlayerParam );
		if( s_Success.IsSucceeded == true ){
//			ColliderSetting();
		}
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") == true ){
		// �f�[�^�̏�������.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_pPlayerParam );
	}
	ImGui::SameLine();
	s_Success.Render();	// �������ǂ�����`��.
	ImGui::NewLine();
	if( ImGui::Button(u8"�v���r���[") == true ){
		m_IsSetCamera = true;
		// �Q�[���p�b�h�̎g�p����߂�.
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
	}

	ImGui::PopItemWidth();
	ImGui::End();
}

// ���f���̕`��.
void CPlayerEdit::ModelRender() 
{
	m_pPlayer->Render();
}

// �G�t�F�N�g�̕`��.
void CPlayerEdit::EffectRender()
{
	m_pPlayer->EffectRender();
}