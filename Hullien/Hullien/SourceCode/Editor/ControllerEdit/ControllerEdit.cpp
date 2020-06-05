#include "ControllerEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

CControllerEdit::CControllerEdit()
{
	FileReading();
}

CControllerEdit::~CControllerEdit()
{
}

//--------------------------.
// 描画関数.
//--------------------------.
void CControllerEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE, ImGuiCond_::ImGuiCond_Once );
	ImGui::SetNextWindowPos( RENDER_POSITION, ImGuiCond_::ImGuiCond_Once );
	ImGui::GetWindowSize();

	bool isOpen = true;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.3f, 0.3f, 0.3f, 0.9f };
	ImGui::Begin( u8"コントローラー設定", &isOpen );

	ImGui::PushItemWidth( WINDOW_SIZE.x/2.0f );

	// ラベルの設定 ( u8 で日本語表示 ).
	ImGui::LabelText( u8"変更ボタン", u8"設定ボタン" );

	// 各ラベルの表示.
	for( int i = CXInput::A; i < CXInput::Max; i++ ){
		LabelRender( i );
	}

	// ボタン設定の決定.
	if( ImGui::Button( u8"決定" ) ){
		ButtonSettingDetermination();
	}

	ImGui::End();
}

//--------------------------.
// ラベルの描画.
//--------------------------.
void CControllerEdit::LabelRender( const int& index )
{
	// 設定前のボタン情報を記憶.
	CXInput::enBUTTON_LIST oldNum = m_ItemList[index];

	// ラベルの表示.
	if( ImGui::Combo( m_LabelList[index].u8string().c_str(), (int*)&m_ItemList[index], 
		BUTTON_ITEMS, IM_ARRAYSIZE(BUTTON_ITEMS) )){
		
		// ボタンを入れ替える.
		for( int i = CXInput::A; i < CXInput::Max; i++ ){
			if( i == index ) continue;
			if( m_ItemList[index] == m_ItemList[i] ){
				m_ItemList[i] = oldNum;
			}
		}
	}
}

//--------------------------.
// ボタン設定の決定.
//--------------------------.
void CControllerEdit::ButtonSettingDetermination()
{
	// 各種ボタンを設定する.
	for( int i = CXInput::A; i < CXInput::Max; i++ ){
		CXInput::enBUTTON_LIST no = static_cast<CXInput::enBUTTON_LIST>(i);
		CXInput::SetButton( no, m_ItemList[no] );
	}
	// ファイルに書き込む.
	FileWriting();
}

//--------------------------.
// ファイルの読み込み.
//--------------------------.
void CControllerEdit::FileReading()
{
	std::vector<std::string> readList;
	readList = CXInput::FileReading();

	// テキストの情報を記憶.
	for( size_t i = 0; i < readList.size(); i+=2 ){
		m_LabelList.emplace_back( readList[i].c_str() );
		CXInput::enBUTTON_LIST temp = static_cast<CXInput::enBUTTON_LIST>(std::stoi(readList[i+1]));
		m_ItemList.emplace_back( temp ); 
	}
}

//--------------------------.
// ファイルの書き込み.
//--------------------------.
void CControllerEdit::FileWriting()
{
	std::ofstream outFile;
	outFile.open( CXInput::FILE_PATH, std::ios::trunc );

	// ファイルが読み込めてなかったら終了.
	if( !outFile.is_open() ) return;

	// リストのサイズ分テキストに保存.
	for( size_t i = 0; i < m_ItemList.size(); i++ ){
		outFile << m_LabelList[i].string() << ",";
		outFile << static_cast<int>(m_ItemList[i]) << std::endl;
	}

	outFile.close();
}