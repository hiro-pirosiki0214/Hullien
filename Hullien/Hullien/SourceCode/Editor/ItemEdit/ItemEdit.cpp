#include "ItemEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\GameObject\Actor\Item\ItemList.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\GameObject\Actor\Item\EditItem\EditItem.h"

CItemEdit::CItemEdit()
	: m_pItem			( nullptr )
	, m_Prameter		()
	, m_EachItemEffect	()
	, m_ItemNo			( 0 )
{
	m_pItem = std::make_unique<CEditItem>();
}

CItemEdit::~CItemEdit()
{
}

// �������֐�.
bool CItemEdit::Init()
{
	if( FileReading()	== false ) return false;
	if( m_pItem->Init()	== false ) return false;
	return true;
}

// �X�V�֐�.
void CItemEdit::Update()
{
	m_pItem->SetItemNo( m_ItemNo );
	m_pItem->SetParamter( m_Prameter );
	m_pItem->Update();
}

// �`��֐�.
void CItemEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();

	ImGui::Begin( u8"�A�C�e���ݒ�" );

	ImGui::PushItemWidth(200.0f);

	if( ImGui::BeginTabBar( "TabBarID" ) == true ){
		for( int i = 0; i < (int)TAG_LIST.size(); i++ ){
			if( ImGui::BeginTabItem( TAG_LIST[i].c_str() ) == false ) continue;
			if( TAG_LIST[i] != "Parameter" ) m_ItemNo = i-1;
			TagRender( i );
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopItemWidth();
	ImGui::End();
}

// ���f���̕`��.
void CItemEdit::ModelRender()
{
	m_pItem->Render();
}

// �G�t�F�N�g�̕`��.
void CItemEdit::EffectRender()
{
	m_pItem->EffectRender();
}

// �e�^�O�̕`��.
void CItemEdit::TagRender( const int& index )
{
	EItemList itemNo = static_cast<EItemList>(index);
	switch( itemNo )
	{
	case EItemList::None:
		ParamRender();
		return;
	case EItemList::LifeRecovery:
		CImGuiManager::DragFloat( u8"�񕜗�", &m_EachItemEffect.LifeRecoveryValue );
		break;
	case EItemList::SPEffectTime:
		CImGuiManager::DragFloat( u8"����\�͉񕜗�", &m_EachItemEffect.SPRecoveryValue );
		CImGuiManager::DragFloat( u8"����\�͉񕜎���", &m_EachItemEffect.SPRecoveryTime );
		break;
	case EItemList::AttackUp:
		CImGuiManager::DragFloat( u8"�U����", &m_EachItemEffect.AttackPower );
		CImGuiManager::DragFloat( u8"�U����UP����", &m_EachItemEffect.AttackUpTime );
		break;
	case EItemList::MoveSpeedUp:
		CImGuiManager::DragFloat( u8"�ړ����x", &m_EachItemEffect.MovePower );
		CImGuiManager::DragFloat( u8"�ړ����xUP����", &m_EachItemEffect.MoveUpTime );
		break;
	default:
		break;
	}
	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"�Ǎ�") ) 
		s_success.IsSucceeded = CFileManager::BinaryReading( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect );
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ) 
		s_success.IsSucceeded = CFileManager::BinaryWriting( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect );
	ImGui::SameLine();
	s_success.Render();
	ImGui::NewLine();
	if( ImGui::Button(u8"�Đ�") )
		m_pItem->Drop( { 0.0f, 5.0f, 0.0f } );
}

// �p�����[�^�̕`��.
void CItemEdit::ParamRender()
{
	CImGuiManager::DragFloat( u8"���������l",					&m_Prameter.InitAccelerationValue );
	CImGuiManager::DragFloat( u8"�����d��",					&m_Prameter.InitGravity );
	CImGuiManager::DragInt(   u8"�o�E���h�ő吔",				&m_Prameter.BoundCountMax );
	CImGuiManager::DragFloat( u8"���f���T�C�Y�ő�",			&m_Prameter.ModelScaleMax );
	CImGuiManager::DragFloat( u8"���f���T�C�Y���Z�l",			&m_Prameter.ModelScaleAddValue );
	CImGuiManager::DragFloat( u8"�ŏI�I�ȕ`��̍���",			&m_Prameter.FinalRenderHeight );
	CImGuiManager::DragFloat( u8"�ŏI�I�Ȉړ����x",			&m_Prameter.FinalMoveSpeed );
	CImGuiManager::DragFloat( u8"��]���x",					&m_Prameter.RotationSpeed );
	CImGuiManager::DragFloat( u8"�A�N�e�B�u����",				&m_Prameter.ActiveTime );
	CImGuiManager::DragFloat( u8"�����鎞��",					&m_Prameter.DisappearTime );
	CImGuiManager::DragFloat( u8"�_�ŉ��Z�l",					&m_Prameter.FlashingAddValue );
	CImGuiManager::DragFloat( u8"���f�����ߒl�ő�",			&m_Prameter.ModelAlphaMax );
	CImGuiManager::DragFloat( u8"�q�b�g���̃G�t�F�N�g����",	&m_Prameter.HitEffectTime );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"�Ǎ�") ) 
		s_success.IsSucceeded = CFileManager::BinaryReading( ITEM_PARAM_FILE_PATH, m_Prameter );
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ) 
		s_success.IsSucceeded = CFileManager::BinaryWriting( ITEM_PARAM_FILE_PATH, m_Prameter );
	ImGui::SameLine();
	s_success.Render();
	ImGui::NewLine();
	if( ImGui::Button(u8"�Đ�") )
		m_pItem->Drop( { 0.0f, 5.0f, 0.0f } );
}

// �t�@�C���̓ǂݍ���.
bool CItemEdit::FileReading()
{
	if( CFileManager::BinaryReading( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect ) == false ) return false;
	if( CFileManager::BinaryReading( ITEM_PARAM_FILE_PATH, m_Prameter ) == false ) return false;
	return true;
}