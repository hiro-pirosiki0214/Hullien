#include "ItemEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\GameObject\Actor\Item\ItemList.h"

CItemEdit::CItemEdit()
	: m_Prameter		()
	, m_EachItemEffect	()
{
}

CItemEdit::~CItemEdit()
{
}

// 初期化関数.
bool CItemEdit::Init()
{
	if( FileReading() == false ) return false;
	return true;
}

// 描画関数.
void CItemEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE, ImGuiCond_::ImGuiCond_Once );
	ImGui::SetNextWindowPos( RENDER_POSITION, ImGuiCond_::ImGuiCond_Once );
	ImGui::GetWindowSize();

	bool isOpen = true;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.3f, 0.3f, 0.3f, 0.9f };
	ImGui::Begin( u8"アイテム設定", &isOpen );

	if( ImGui::BeginTabBar( "TabBarID" ) == true ){
		for( int i = 0; i < (int)TAG_LIST.size(); i++ ){
			if( ImGui::BeginTabItem( TAG_LIST[i].c_str() ) == false ) continue;
			TagRender( i );
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

// 各タグの描画.
void CItemEdit::TagRender( const int& index )
{
	EItemList itemNo = static_cast<EItemList>(index);
	switch( itemNo )
	{
	case EItemList::None:
		ParamRender();
		return;
	case EItemList::LifeRecovery:
		ImGui::InputFloat( u8"回復力", &m_EachItemEffect.LifeRecoveryValue );
		break;
	case EItemList::SPEffectTime:
		ImGui::InputFloat( u8"特殊能力回復力", &m_EachItemEffect.SPRecoveryValue );
		ImGui::InputFloat( u8"特殊能力回復時間", &m_EachItemEffect.SPRecoveryTime );
		break;
	case EItemList::AttackUp:
		ImGui::InputFloat( u8"攻撃力", &m_EachItemEffect.AttackPower );
		ImGui::InputFloat( u8"攻撃力UP時間", &m_EachItemEffect.AttackUpTime );
		break;
	case EItemList::MoveSpeedUp:
		ImGui::InputFloat( u8"移動速度", &m_EachItemEffect.MovePower );
		ImGui::InputFloat( u8"移動速度UP時間", &m_EachItemEffect.MoveUpTime );
		break;
	default:
		break;
	}
	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"読込") ) 
		s_success.IsSucceeded = CFileManager::BinaryReading( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect );
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") ) 
		s_success.IsSucceeded = CFileManager::BinaryWriting( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect );
	ImGui::SameLine();
	s_success.Render();
}

// パラメータの描画.
void CItemEdit::ParamRender()
{
	ImGui::InputFloat( u8"初期加速値",					&m_Prameter.InitAccelerationValue );
	ImGui::InputFloat( u8"初期重力",					&m_Prameter.InitGravity );
	ImGui::InputInt(   u8"バウンド最大数",				&m_Prameter.BoundCountMax );
	ImGui::InputFloat( u8"モデルサイズ最大",			&m_Prameter.ModelScaleMax );
	ImGui::InputFloat( u8"モデルサイズ加算値",			&m_Prameter.ModelScaleAddValue );
	ImGui::InputFloat( u8"最終的な描画の高さ",			&m_Prameter.FinalRenderHeight );
	ImGui::InputFloat( u8"最終的な移動速度",			&m_Prameter.FinalMoveSpeed );
	ImGui::InputFloat( u8"回転速度",					&m_Prameter.RotationSpeed );
	ImGui::InputFloat( u8"アクティブ時間",				&m_Prameter.ActiveTime );
	ImGui::InputFloat( u8"消える時間",					&m_Prameter.DisappearTime );
	ImGui::InputFloat( u8"点滅加算値",					&m_Prameter.FlashingAddValue );
	ImGui::InputFloat( u8"モデル透過値最大",			&m_Prameter.ModelAlphaMax );
	ImGui::InputFloat( u8"ヒット時のエフェクト時間",	&m_Prameter.HitEffectTime );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"読込") ) 
		s_success.IsSucceeded = CFileManager::BinaryReading( ITEM_PARAM_FILE_PATH, m_Prameter );
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") ) 
		s_success.IsSucceeded = CFileManager::BinaryWriting( ITEM_PARAM_FILE_PATH, m_Prameter );
	ImGui::SameLine();
	s_success.Render();
}

// ファイルの読み込み.
bool CItemEdit::FileReading()
{
	if( CFileManager::BinaryReading( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect ) == false ) return false;
	if( CFileManager::BinaryReading( ITEM_PARAM_FILE_PATH, m_Prameter ) == false ) return false;
	return true;
}