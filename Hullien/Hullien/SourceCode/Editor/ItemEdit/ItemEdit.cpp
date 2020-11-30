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

// 初期化関数.
bool CItemEdit::Init()
{
	if( FileReading()	== false ) return false;
	if( m_pItem->Init()	== false ) return false;
	return true;
}

// 更新関数.
void CItemEdit::Update()
{
	m_pItem->SetItemNo( m_ItemNo );
	m_pItem->SetParamter( m_Prameter );
	m_pItem->Update();
}

// 描画関数.
void CItemEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();

	ImGui::Begin( u8"アイテム設定" );

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

// モデルの描画.
void CItemEdit::ModelRender()
{
	m_pItem->Render();
}

// エフェクトの描画.
void CItemEdit::EffectRender()
{
	m_pItem->EffectRender();
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
		CImGuiManager::DragFloat( u8"回復力", &m_EachItemEffect.LifeRecoveryValue );
		break;
	case EItemList::SPEffectTime:
		CImGuiManager::DragFloat( u8"特殊能力回復力", &m_EachItemEffect.SPRecoveryValue );
		CImGuiManager::DragFloat( u8"特殊能力回復時間", &m_EachItemEffect.SPRecoveryTime );
		break;
	case EItemList::AttackUp:
		CImGuiManager::DragFloat( u8"攻撃力", &m_EachItemEffect.AttackPower );
		CImGuiManager::DragFloat( u8"攻撃力UP時間", &m_EachItemEffect.AttackUpTime );
		break;
	case EItemList::MoveSpeedUp:
		CImGuiManager::DragFloat( u8"移動速度", &m_EachItemEffect.MovePower );
		CImGuiManager::DragFloat( u8"移動速度UP時間", &m_EachItemEffect.MoveUpTime );
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
	ImGui::NewLine();
	if( ImGui::Button(u8"再生") )
		m_pItem->Drop( { 0.0f, 5.0f, 0.0f } );
}

// パラメータの描画.
void CItemEdit::ParamRender()
{
	CImGuiManager::DragFloat( u8"初期加速値",					&m_Prameter.InitAccelerationValue );
	CImGuiManager::DragFloat( u8"初期重力",					&m_Prameter.InitGravity );
	CImGuiManager::DragInt(   u8"バウンド最大数",				&m_Prameter.BoundCountMax );
	CImGuiManager::DragFloat( u8"モデルサイズ最大",			&m_Prameter.ModelScaleMax );
	CImGuiManager::DragFloat( u8"モデルサイズ加算値",			&m_Prameter.ModelScaleAddValue );
	CImGuiManager::DragFloat( u8"最終的な描画の高さ",			&m_Prameter.FinalRenderHeight );
	CImGuiManager::DragFloat( u8"最終的な移動速度",			&m_Prameter.FinalMoveSpeed );
	CImGuiManager::DragFloat( u8"回転速度",					&m_Prameter.RotationSpeed );
	CImGuiManager::DragFloat( u8"アクティブ時間",				&m_Prameter.ActiveTime );
	CImGuiManager::DragFloat( u8"消える時間",					&m_Prameter.DisappearTime );
	CImGuiManager::DragFloat( u8"点滅加算値",					&m_Prameter.FlashingAddValue );
	CImGuiManager::DragFloat( u8"モデル透過値最大",			&m_Prameter.ModelAlphaMax );
	CImGuiManager::DragFloat( u8"ヒット時のエフェクト時間",	&m_Prameter.HitEffectTime );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"読込") ) 
		s_success.IsSucceeded = CFileManager::BinaryReading( ITEM_PARAM_FILE_PATH, m_Prameter );
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") ) 
		s_success.IsSucceeded = CFileManager::BinaryWriting( ITEM_PARAM_FILE_PATH, m_Prameter );
	ImGui::SameLine();
	s_success.Render();
	ImGui::NewLine();
	if( ImGui::Button(u8"再生") )
		m_pItem->Drop( { 0.0f, 5.0f, 0.0f } );
}

// ファイルの読み込み.
bool CItemEdit::FileReading()
{
	if( CFileManager::BinaryReading( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect ) == false ) return false;
	if( CFileManager::BinaryReading( ITEM_PARAM_FILE_PATH, m_Prameter ) == false ) return false;
	return true;
}