#include "ExplosionEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

CExplosionEdit::CExplosionEdit()
	: m_ExplosionParam	()
{
}

CExplosionEdit::~CExplosionEdit()
{
}

// 初期化関数.
bool CExplosionEdit::Init()
{
	if( FileReading() == false ) return false;
	return true;
}

// 描画関数.
void CExplosionEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE, ImGuiCond_::ImGuiCond_Once );
	ImGui::SetNextWindowPos( RENDER_POSITION, ImGuiCond_::ImGuiCond_Once );
	ImGui::GetWindowSize();

	bool isOpen = true;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.3f, 0.3f, 0.3f, 0.9f };
	ImGui::Begin( u8"爆発パラメーター設定", &isOpen );

	ImGui::InputFloat( u8"爆発力", &m_ExplosionParam.AttackPower );
	ImGui::InputFloat( u8"爆発時間", &m_ExplosionParam.ExplosionTime );
	ImGui::InputFloat( u8"スフィアの調整用座標 : X", &m_ExplosionParam.SphereAdjPos.x );
	ImGui::InputFloat( u8"スフィアの調整用座標 : Y", &m_ExplosionParam.SphereAdjPos.y );
	ImGui::InputFloat( u8"スフィアの調整用座標 : Z", &m_ExplosionParam.SphereAdjPos.z );
	ImGui::InputFloat( u8"スフィアの調整用半径", &m_ExplosionParam.SphereAdjRadius );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"読込") ) s_success.IsSucceeded = FileReading();
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") ) s_success.IsSucceeded = FileWriting();
	ImGui::SameLine();
	s_success.Render();

	ImGui::End();
}

// ファイルの読み込み.
bool CExplosionEdit::FileReading()
{
	return CFileManager::BinaryReading( FILE_PATH, m_ExplosionParam );
}

// ファイルの書き込み.
bool CExplosionEdit::FileWriting()
{
	return CFileManager::BinaryWriting( FILE_PATH, m_ExplosionParam );
}