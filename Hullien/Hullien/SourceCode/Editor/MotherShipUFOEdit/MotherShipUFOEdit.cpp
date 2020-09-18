#include "MotherShipUFOEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

CMotherShipUFOEdit::CMotherShipUFOEdit()
	: m_pMotherShipUFO		( nullptr )
	, m_MotherShipUFOParam	()
{
	m_pMotherShipUFO = std::make_unique<CMotherShipUFO>();
}

CMotherShipUFOEdit::~CMotherShipUFOEdit()
{
}

// 初期化関数.
bool CMotherShipUFOEdit::Init()
{
	if( m_pMotherShipUFO->Init() == false ) return false;
	if( CFileManager::BinaryReading( MOTHER_SHIP_UFO_PARAM_FILE_PATH, m_MotherShipUFOParam ) == false ) return false;
	return true;
}

// 更新関数.
void CMotherShipUFOEdit::Update()
{
	m_pMotherShipUFO->Update();
}

// 描画関数.
void CMotherShipUFOEdit::Render()
{

	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"マザーシップUFO設定" );
	ImGui::PushItemWidth(200.0f);

	ImGui::InputFloat( u8"初期座標 : X",			&m_MotherShipUFOParam.Position.x );
	ImGui::InputFloat( u8"初期座標 : Y",			&m_MotherShipUFOParam.Position.y );
	ImGui::InputFloat( u8"初期座標 : Z",			&m_MotherShipUFOParam.Position.z );
	ImGui::InputFloat( u8"持ち上げる力",			&m_MotherShipUFOParam.AddPosYPower );
	ImGui::InputFloat( u8"スフィアの調整用座標 : X",&m_MotherShipUFOParam.SphereAdjPos.x );
	ImGui::InputFloat( u8"スフィアの調整用座標 : Y",&m_MotherShipUFOParam.SphereAdjPos.y );
	ImGui::InputFloat( u8"スフィアの調整用座標 : Z",&m_MotherShipUFOParam.SphereAdjPos.z );
	ImGui::InputFloat( u8"スフィアの半径",			&m_MotherShipUFOParam.CollisionRadius );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"読込") ){
		s_success.IsSucceeded = CFileManager::BinaryReading( MOTHER_SHIP_UFO_PARAM_FILE_PATH, m_MotherShipUFOParam );
		m_pMotherShipUFO->SetParameter( m_MotherShipUFOParam );
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") ){
		s_success.IsSucceeded = CFileManager::BinaryWriting( MOTHER_SHIP_UFO_PARAM_FILE_PATH, m_MotherShipUFOParam );
		m_pMotherShipUFO->SetParameter( m_MotherShipUFOParam );
	}
	ImGui::SameLine();
	s_success.Render();

	ImGui::PopItemWidth();
	ImGui::End();
}

// モデルの描画.
void CMotherShipUFOEdit::ModelRender()
{
	m_pMotherShipUFO->Render();
}