#include "AlienParamEdit.h"
#include "..\..\GameObject\SpawnUFO\SpawnUFOParam.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\GameObject\Actor\Character\Alien\AlienList.h"
#include "..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

#include "..\..\GameObject\Actor\Character\Alien\EditAlien\Alien_A\EditAlien_A.h"
#include "..\..\GameObject\Actor\Character\Alien\EditAlien\Alien_B\EditAlien_B.h"
#include "..\..\GameObject\Actor\Character\Alien\EditAlien\Alien_C\EditAlien_C.h"
#include "..\..\GameObject\Actor\Character\Alien\EditAlien\Alien_D\EditAlien_D.h"

#include <filesystem>

namespace fs = std::filesystem;

CAlienParamEdit::CAlienParamEdit()
	: m_AlienParamList		()
	, m_AlienPathList		()
	, m_AlienNameList		()
	, m_pEditAliens			()
	, m_pMotherShipUFO		( nullptr )
	, m_MotherShipUFOParam	()
	, m_pSpawnUFO			( nullptr )
	, m_SpawnUFOParam		()
	, m_AlienIndex			( 0 )
	, m_NowParamIndex		( 0 )
	, m_IsRisingMotherShip	( false )
{
	m_pMotherShipUFO = std::make_unique<CMotherShipUFO>();
	m_pSpawnUFO = std::make_unique<CSpawnUFO>();
}

CAlienParamEdit::~CAlienParamEdit()
{
}

// �������֐�.
bool CAlienParamEdit::Init()
{
	if( FileAllReading()			== false ) return false;
	if( InitAlien()					== false ) return false;
	if( m_pMotherShipUFO->Init()	== false ) return false;
	if( m_pSpawnUFO->Init()			== false ) return false;
	std::vector<stSpawnUFOParam> tempPram;
	if( CFileManager::BinaryVectorReading( SPAWN_UFO_PARAM_FILE_PATH, tempPram ) == false ) return false;
	m_SpawnUFOParam = tempPram[0];
	m_pSpawnUFO->SetSpawnParameter( m_SpawnUFOParam );
	m_pSpawnUFO->DischargePreparation()
		;
	m_pMotherShipUFO->DischargePreparation();

	return true;
}

// �X�V�֐�.
void CAlienParamEdit::Update()
{
	m_pEditAliens[m_AlienIndex]->SetParamter(m_AlienParamList[m_NowParamIndex]);
	m_pEditAliens[m_AlienIndex]->Update();

	// �}�U�[�V�b�v�ɏ�鏈��.
	m_pMotherShipUFO->Update();
	if( m_IsRisingMotherShip == true ){
		m_pEditAliens[m_AlienIndex]->SetOtherAbduct( &m_IsRisingMotherShip );
		m_pMotherShipUFO->Collision( m_pEditAliens[m_AlienIndex].get() );
		m_IsRisingMotherShip = m_pEditAliens[m_AlienIndex]->IsPlaying();
	}
}

// �`��֐�.
void CAlienParamEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"�F���l�p�����[�^�ݒ�" );

	if( ImGui::BeginTabBar("TabBarID") == true ){
		for( size_t i = 0; i < m_AlienNameList.size(); i++ ){
			if( ImGui::BeginTabItem( m_AlienNameList[i].c_str() ) == false ) continue;
			SpawnParamRender( i );
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

// ���f���̕`��.
void CAlienParamEdit::ModelRender()
{
	m_pEditAliens[m_AlienIndex]->Render();
	m_pSpawnUFO->Render();
	m_pMotherShipUFO->Render();
}

// �G�t�F�N�g�̕`��.
void CAlienParamEdit::EffectRender()
{
	m_pEditAliens[m_AlienIndex]->EffectRender();
}

// �S�t�@�C���̓ǂݍ���.
bool CAlienParamEdit::FileAllReading()
{
	m_AlienPathList = CFileManager::TextLoading( FILE_PATH );
	
	for( const auto& f : m_AlienPathList ){
		const fs::path fileName = f;	// �t�@�C���l�[�����p�X�ɐݒ�.
		// �F���l�̖��O�ɕϊ����ă��X�g�ɒǉ�.
		m_AlienNameList.emplace_back( fileName.stem().string() );
		SAlienParam alienParam = {};
		// �o�C�i���ǂݍ���.
		if( FileReading( f.c_str(), alienParam ) == false ) return false; 
		m_AlienParamList.emplace_back( alienParam );	// �p�����[�^���X�g�ɒǉ�.
	}

	return true;
}

// �t�@�C���̓ǂݍ���.
bool CAlienParamEdit::FileReading( const char* fileName, SAlienParam& param )
{
	return CFileManager::BinaryReading( fileName, param );
}

// �t�@�C���̏�������.
bool CAlienParamEdit::FileWriting( const char* fileName, SAlienParam& param )
{
	return CFileManager::BinaryWriting( fileName, param );
}

// �X�|�[�����̕`��.
void CAlienParamEdit::SpawnParamRender( const int& index )
{
	if( m_AlienParamList.empty() == true ) return;

	auto& s = m_AlienParamList[index];
	m_NowParamIndex = index;

	EAlienList alienType = static_cast<EAlienList>(index);

	ImGui::PushItemWidth(200.0f);

	//  �e�p�����[�^�̐ݒ�.
	CImGuiManager::DragFloat( u8"�ړ����x", &s.MoveSpeed );
	CImGuiManager::DragFloat( u8"�̗�", &s.LifeMax );
	CImGuiManager::DragFloat( u8"�U����", &s.AttackPower );
	CImGuiManager::DragInt( u8"���G����", &s.InvincibleTime );

	CImGuiManager::DragFloat( u8"�ړ���]���x", &s.RotationalSpeed );
	CImGuiManager::DragFloat( u8"�X�|�[�����̃X�P�[���̉��Z�l", &s.SpawnScaleAddValue );
	CImGuiManager::DragFloat( u8"�X�|�[�����̍~�����x", &s.SpawnDownSpeed );
	CImGuiManager::DragFloat( u8"�}�U�[�V�b�v�ɏ㏸���鎞�̃X�P�[�����Z�l", &s.MotherShipUpScaleSubValue );
	CImGuiManager::DragInt( u8"�ҋ@����", &s.WaitTime );
	CImGuiManager::DragInt( u8"�m�b�N�o�b�N�̎���", &s.KnockBackTime );

	CImGuiManager::DragFloat( u8"���S�J�E���g�̉��Z�l", &s.DeadCountAddValue );
	CImGuiManager::DragFloat( u8"���S���̃X�P�[���̌��Z�l", &s.DeadScaleSubValue );

	switch( alienType )
	{
	case EAlienList::Ada:
	case EAlienList::A:
		m_AlienIndex = index / 2;
		break;
	case EAlienList::B:
	case EAlienList::Bda:
		CImGuiManager::DragFloat( u8"�v���C���[��_���͈�", &s.PlayerAimLenght );
		CImGuiManager::DragFloat( u8"�U�����鋗��", &s.AttackLenght );
		CImGuiManager::DragFloat( u8"�U�����̏�����]��", &s.AttackRotInitPower );
		CImGuiManager::DragFloat( u8"�U�����̉�]��", &s.AttackRotPower );
		CImGuiManager::DragFloat( u8"�U�����̉�]���Z�l", &s.AttackRotAddValue );
		CImGuiManager::DragFloat( u8"�U���ړ����x", &s.AttackMoveSpeed );
		CImGuiManager::DragFloat( u8"�U���ړ��͈�", &s.AttackMoveRange );
		m_AlienIndex = index / 2;
		break;
	case EAlienList::C:
		m_AlienIndex = index / 2;
		break;
	case EAlienList::D:
		CImGuiManager::DragFloat( u8"���[�U�[�̈ړ����x", &s.LaserMoveSpeed );
		CImGuiManager::DragFloat( u8"��Ⴢ̎���", &s.ParalysisTime );
		CImGuiManager::DragFloat( u8"�U�����鋗��", &s.AttackLenght );
		CImGuiManager::DragFloat( u8"�ēx�������鋗��", &s.ResearchLenght );
		CImGuiManager::DragFloat( u8"�U�����͈͕̔\���̒����p", &s.AttackRangeSpritePosY );
		CImGuiManager::DragFloat( u8"�U�����͈͕̔\���̉摜�T�C�Y", &s.AttackRangeSpriteScale );
		CImGuiManager::DragFloat( u8"�U�����͈͕̔\���̃A���t�@���������l", &s.AttackRangeAddValue );
		CImGuiManager::DragFloat( u8"�U�����͈͕̔\���̃A���t�@���������l", &s.AttackRangeSubValue );
		CImGuiManager::DragFloat( u8"�x�W�F�Ȑ��̈�ڑ�����W�̋���", &s.ControlPointOneLenght );
		CImGuiManager::DragFloat( u8"�x�W�F�Ȑ��̈�ڑ�����W��y���W�̋���", &s.ControlPointOneLenghtY );
		CImGuiManager::DragFloat( u8"�x�W�F�Ȑ��̓�ڑ�����W�̋���", &s.ControlPointTwoLenght );
		CImGuiManager::DragFloat( u8"�x�W�F�Ȑ��̓�ڑ�����W��y���W�̋���", &s.ControlPointTwoLenghtY );
		m_AlienIndex = index / 2+1;
		break;
	default:
		break;
	}

	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W X", &s.SphereAdjPos.x );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W Y", &s.SphereAdjPos.y );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W Z", &s.SphereAdjPos.z );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������a", &s.SphereAdjRadius );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"�Ǎ�") ) 
		s_success.IsSucceeded = FileReading( m_AlienPathList[index].c_str(), s );
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ) 
		s_success.IsSucceeded = FileWriting( m_AlienPathList[index].c_str(), s );
	ImGui::SameLine();
	s_success.Render(); ImGui::NewLine();

	if( ImGui::Button(u8"�X�|�[���v���r���[") ){
		m_pEditAliens[m_AlienIndex]->Spawn(
			{
				m_SpawnUFOParam.Position.x,
				m_SpawnUFOParam.SpawnPointHight,
				m_SpawnUFOParam.Position.z
			} );
	}
	if( ImGui::Button(u8"�A�^�b�N�v���r���[") ) m_pEditAliens[m_AlienIndex]->PlayAttack();
	if( ImGui::Button(u8"���݃v���r���[") ) m_pEditAliens[m_AlienIndex]->PlayFright();
	if( ImGui::Button(u8"���S�v���r���[") ) m_pEditAliens[m_AlienIndex]->PlayDeath();
	if( ImGui::Button(u8"�㏸�v���r���[") ){
		m_IsRisingMotherShip = true;
		m_pEditAliens[m_AlienIndex]->PlayRisingMotherShip( 
			{
				m_pMotherShipUFO->GetPosition().x, 
				m_pEditAliens[m_AlienIndex]->GetPosition().y, 
				m_pMotherShipUFO->GetPosition().z
			} );
	}
	if( ImGui::Button(u8"UFO�̃p�����[�^�X�V") ){
		if( CFileManager::BinaryReading( MOTHER_SHIP_UFO_PARAM_FILE_PATH, m_MotherShipUFOParam ) == false ) return;
		m_pMotherShipUFO->SetParameter( m_MotherShipUFOParam );
		std::vector<stSpawnUFOParam> tempPram;
		if( CFileManager::BinaryVectorReading( SPAWN_UFO_PARAM_FILE_PATH, tempPram ) == false ) return;
		m_SpawnUFOParam = tempPram[0];
		m_pSpawnUFO->SetSpawnParameter( m_SpawnUFOParam );
		m_pSpawnUFO->DischargePreparation();
	}

	ImGui::PopItemWidth();
}

// �F���l�̏�����.
bool CAlienParamEdit::InitAlien()
{
	int i = 0;
	m_pEditAliens.clear();
	for( auto& n : m_AlienNameList ){
		EObjectTag tag = static_cast<EObjectTag>(i+3);
		switch( tag )
		{
		case EObjectTag::Alien_A:
			m_pEditAliens.emplace_back( std::make_shared<CEditAlienA>() );
			break;
		case EObjectTag::Alien_B:
			m_pEditAliens.emplace_back( std::make_shared<CEditAlienB>() );
			break;
		case EObjectTag::Alien_C:
			m_pEditAliens.emplace_back( std::make_shared<CEditAlienC>() );
			break;
		case EObjectTag::Alien_D:
			m_pEditAliens.emplace_back( std::make_shared<CEditAlienD>() );
			break;
		default:
			break;
		}
		if( m_pEditAliens.back()->Init() == false ) return false;
		i++;
	}
	return true;
}
