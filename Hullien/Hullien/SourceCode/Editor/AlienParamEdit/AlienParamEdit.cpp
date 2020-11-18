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
	, m_AlienIndex			( 0 )
	, m_NowParamIndex		( 0 )

{
}

CAlienParamEdit::~CAlienParamEdit()
{
}

// 初期化関数.
bool CAlienParamEdit::Init()
{
	if( FileAllReading()		== false ) return false;
	if( InitAlien()				== false ) return false;
	return true;
}

// 更新関数.
void CAlienParamEdit::Update()
{
	m_pEditAliens[m_AlienIndex]->SetParamter(m_AlienParamList[m_NowParamIndex]);
	m_pEditAliens[m_AlienIndex]->Update();
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
	m_pEditAliens[m_AlienIndex]->Render();
}

// エフェクトの描画.
void CAlienParamEdit::EffectRender()
{
	m_pEditAliens[m_AlienIndex]->EffectRender();
}

// 全ファイルの読み込み.
bool CAlienParamEdit::FileAllReading()
{
	m_AlienPathList = CFileManager::TextLoading( FILE_PATH );
	
	for( const auto& f : m_AlienPathList ){
		const fs::path fileName = f;	// ファイルネームをパスに設定.
		// 宇宙人の名前に変換してリストに追加.
		m_AlienNameList.emplace_back( fileName.stem().string() );
		SAlienParam alienParam = {};
		// バイナリ読み込み.
		if( FileReading( f.c_str(), alienParam ) == false ) return false; 
		m_AlienParamList.emplace_back( alienParam );	// パラメータリストに追加.
	}

	return true;
}

// ファイルの読み込み.
bool CAlienParamEdit::FileReading( const char* fileName, SAlienParam& param )
{
	return CFileManager::BinaryReading( fileName, param );
}

// ファイルの書き込み.
bool CAlienParamEdit::FileWriting( const char* fileName, SAlienParam& param )
{
	return CFileManager::BinaryWriting( fileName, param );
}

// スポーン情報の描画.
void CAlienParamEdit::SpawnParamRender( const int& index )
{
	if( m_AlienParamList.empty() == true ) return;

	auto& s = m_AlienParamList[index];
	m_NowParamIndex = index;

	EAlienList alienType = static_cast<EAlienList>(index);

	ImGui::PushItemWidth(200.0f);

	//  各パラメータの設定.
	ImGui::InputFloat( u8"移動速度", &s.MoveSpeed );
	ImGui::InputFloat( u8"体力", &s.LifeMax );
	ImGui::InputFloat( u8"攻撃力", &s.AttackPower );
	ImGui::InputInt( u8"無敵時間", &s.InvincibleTime );

	ImGui::InputFloat( u8"移動回転速度", &s.RotationalSpeed );
	ImGui::InputFloat( u8"スポーン時のスケールの加算値", &s.SpawnScaleAddValue );
	ImGui::InputFloat( u8"スポーン時の降下速度", &s.SpawnDownSpeed );
	ImGui::InputFloat( u8"マザーシップに上昇する時のスケールの減算値", &s.MotherShipUpScaleSubValue );
	ImGui::InputInt( u8"待機時間", &s.WaitTime );
	ImGui::InputInt( u8"ノックバックの時間", &s.KnockBackTime );

	ImGui::InputFloat( u8"死亡カウントの加算値", &s.DeadCountAddValue );
	ImGui::InputFloat( u8"死亡時のスケールの減算値", &s.DeadScaleSubValue );

	switch( alienType )
	{
	case EAlienList::Ada:
	case EAlienList::A:
		m_AlienIndex = index / 2;
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
		m_AlienIndex = index / 2;
		break;
	case EAlienList::C:
		m_AlienIndex = index / 2;
		break;
	case EAlienList::D:
		ImGui::InputFloat( u8"レーザーの移動速度", &s.LaserMoveSpeed );
		ImGui::InputFloat( u8"麻痺の時間", &s.ParalysisTime );
		ImGui::InputFloat( u8"攻撃する距離", &s.AttackLenght );
		ImGui::InputFloat( u8"再度検索する距離", &s.ResearchLenght );
		ImGui::InputFloat( u8"攻撃時の範囲表示の調整用", &s.AttackRangeSpritePosY );
		ImGui::InputFloat( u8"攻撃時の範囲表示の画像サイズ", &s.AttackRangeSpriteScale );
		ImGui::InputFloat( u8"攻撃時の範囲表示のアルファが足される値", &s.AttackRangeAddValue );
		ImGui::InputFloat( u8"攻撃時の範囲表示のアルファが引かれる値", &s.AttackRangeSubValue );
		ImGui::InputFloat( u8"ベジェ曲線の一つ目操作座標の距離", &s.ControlPointOneLenght );
		ImGui::InputFloat( u8"ベジェ曲線の一つ目操作座標のy座標の距離", &s.ControlPointOneLenghtY );
		ImGui::InputFloat( u8"ベジェ曲線の二つ目操作座標の距離", &s.ControlPointTwoLenght );
		ImGui::InputFloat( u8"ベジェ曲線の二つ目操作座標のy座標の距離", &s.ControlPointTwoLenghtY );
		m_AlienIndex = index / 2+1;
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
	s_success.Render(); ImGui::NewLine();

	if( ImGui::Button(u8"スポーンプレビュー") ) m_pEditAliens[m_AlienIndex]->Spawn( { 0.0f, 10.0f, 0.0f } );
	if( ImGui::Button(u8"アタックプレビュー") ) m_pEditAliens[m_AlienIndex]->PlayAttack();
	if( ImGui::Button(u8"怯みプレビュー") ) m_pEditAliens[m_AlienIndex]->PlayFright();
	if( ImGui::Button(u8"死亡プレビュー") ) m_pEditAliens[m_AlienIndex]->PlayDeath();
	if( ImGui::Button(u8"上昇プレビュー") ) m_pEditAliens[m_AlienIndex]->PlayRisingMotherShip( { 0.0f, 20.0f, 0.0f } );

	ImGui::PopItemWidth();
}

// 宇宙人の初期化.
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