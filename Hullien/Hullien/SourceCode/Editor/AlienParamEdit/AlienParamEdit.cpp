#include "AlienParamEdit.h"
#include "..\..\GameObject\SpawnUFO\SpawnUFOParam.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\GameObject\Actor\Character\Alien\AlienList.h"

#include <filesystem>

namespace fs = std::filesystem;

CAlienParamEdit::CAlienParamEdit()
	: m_AlienParamList	()
	, m_Index			( 0 )
{
}

CAlienParamEdit::~CAlienParamEdit()
{
}

// 初期化関数.
bool CAlienParamEdit::Init()
{
	if( FileAllReading() == false ) return false;
	return true;
}

// 更新関数.
void CAlienParamEdit::Update()
{
}

// 描画関数.
void CAlienParamEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"宇宙人パラメータ設定" );

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

// モデルの描画.
void CAlienParamEdit::ModelRender()
{
}

// 全ファイルの読み込み.
bool CAlienParamEdit::FileAllReading()
{
	m_AlienPathList = CFileManager::TextLoading( FILE_PATH );
	
	for( const auto& f : m_AlienPathList ){
		const fs::path fileName = f;	// ファイルネームをパスに設定.
		// 宇宙人の名前に変換してリストに追加.
		m_AlienNameList.emplace_back( fileName.stem().string() );
		CAlien::SAlienParam alienParam = {};
		// バイナリ読み込み.
		if( FileReading( f.c_str(), alienParam ) == false ) return false; 
		m_AlienParamList.emplace_back( alienParam );	// パラメータリストに追加.
	}

	return true;
}

// ファイルの読み込み.
bool CAlienParamEdit::FileReading( const char* fileName, CAlien::SAlienParam& param )
{
	return CFileManager::BinaryReading( fileName, param );
}

// ファイルの書き込み.
bool CAlienParamEdit::FileWriting( const char* fileName, CAlien::SAlienParam& param )
{
	return CFileManager::BinaryWriting( fileName, param );
}

// スポーン情報の描画.
void CAlienParamEdit::SpawnParamRender( const int& index )
{
	if( m_AlienParamList.empty() == true ) return;

	auto& s = m_AlienParamList[index];
	bool hasMovedSillider = false;
	auto checkSetIndex = [&]()
	{
		if (hasMovedSillider == false) return;
		m_Index = index;
	};

	EAlienList alienType = static_cast<EAlienList>(index);

	ImGui::PushItemWidth(200.0f);

	//  各パラメータの設定.
	ImGui::InputFloat( u8"移動速度", &s.MoveSpeed );
	ImGui::InputFloat( u8"体力", &s.LifeMax );
	ImGui::InputFloat( u8"攻撃力", &s.AttackPower );
	ImGui::InputInt( u8"待機時間", &s.WaitTime );
	ImGui::InputInt( u8"無敵時間", &s.InvincibleTime );
	ImGui::InputFloat( u8"移動回転速度", &s.RotationalSpeed );
	ImGui::InputFloat( u8"スポーン時のモデル透過加算値", &s.ModelAlphaAddValue );
	ImGui::InputFloat( u8"スポーン時のモデル透過減算値", &s.ModelAlphaSubValue );

	switch( alienType )
	{
	case EAlienList::A:
		break;
	case EAlienList::Ada:
		break;
	case EAlienList::B:
	case EAlienList::Bda:
		ImGui::InputFloat( u8"プレイヤーを狙う範囲", &s.PlayerAimLenght );
		ImGui::InputFloat( u8"攻撃する距離", &s.AttackLenght );
		ImGui::InputFloat( u8"攻撃時の初期回転力", &s.AttackRotInitPower );
		ImGui::InputFloat( u8"攻撃時の回転力", &s.AttackRotPower );
		ImGui::InputFloat( u8"攻撃時の回転加算値", &s.AttackRotAddValue );
		ImGui::InputFloat( u8"攻撃移動速度", &s.AttackMoveSpeed );
		ImGui::InputFloat( u8"攻撃移動範囲", &s.AttackMoveRange );
		break;
	case EAlienList::C:
		break;
	case EAlienList::D:
		ImGui::InputFloat( u8"レーザーの移動速度", &s.LaserMoveSpeed );
		ImGui::InputFloat( u8"麻痺の時間", &s.ParalysisTime );
		ImGui::InputFloat( u8"攻撃する距離", &s.AttackLenght );
		ImGui::InputFloat( u8"攻撃時の範囲表示の調整用", &s.AttackRangeSpritePosY );
		ImGui::InputFloat( u8"攻撃時の範囲表示の画像サイズ", &s.AttackRangeSpriteScale );
		ImGui::InputFloat( u8"攻撃時の範囲表示のアルファが足される値", &s.AttackRangeAddValue );
		ImGui::InputFloat( u8"攻撃時の範囲表示のアルファが引かれる値", &s.AttackRangeSubValue );
		ImGui::InputFloat( u8"ベジェ曲線の一つ目操作座標の距離", &s.ControlPointOneLenght );
		ImGui::InputFloat( u8"ベジェ曲線の一つ目操作座標のy座標の距離", &s.ControlPointOneLenghtY );
		ImGui::InputFloat( u8"ベジェ曲線の二つ目操作座標の距離", &s.ControlPointTwoLenght );
		ImGui::InputFloat( u8"ベジェ曲線の二つ目操作座標のy座標の距離", &s.ControlPointTwoLenghtY );
		break;
	default:
		break;
	}

	ImGui::InputFloat( u8"スフィアの調整座標 X", &s.SphereAdjPos.x );
	ImGui::InputFloat( u8"スフィアの調整座標 Y", &s.SphereAdjPos.y );
	ImGui::InputFloat( u8"スフィアの調整座標 Z", &s.SphereAdjPos.z );
	ImGui::InputFloat( u8"スフィアの調整半径", &s.SphereAdjRadius );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"読込") ) 
		s_success.IsSucceeded = FileReading( m_AlienPathList[index].c_str(), s );
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") ) 
		s_success.IsSucceeded = FileWriting( m_AlienPathList[index].c_str(), s );
	ImGui::SameLine();
	s_success.Render();


	ImGui::PopItemWidth();
}