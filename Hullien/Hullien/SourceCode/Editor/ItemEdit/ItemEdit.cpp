#include "ItemEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\GameObject\Actor\Item\ItemList.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

CItemEdit::CItemEdit()
	: m_pStaticMeshs	()
	, m_Prameter		()
	, m_EachItemEffect	()
	, m_RotY			( 0.0f )
	, m_ItemNo			( 0 )
{
}

CItemEdit::~CItemEdit()
{
}

// 初期化関数.
bool CItemEdit::Init()
{
	if( FileReading() == false ) return false;
	if( GetModel() == false ) return false;
	return true;
}

// 更新関数.
void CItemEdit::Update()
{
	m_RotY += 0.01f;
	if( D3DXToDegree(m_RotY) >= 360.0 ) m_RotY = 0.0f;
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
	m_pStaticMeshs[m_ItemNo]->SetPosition( { 0.0f, 2.0f, 0.0f } );
	m_pStaticMeshs[m_ItemNo]->SetRotation( { 0.0f, m_RotY, 0.0f } );
	m_pStaticMeshs[m_ItemNo]->SetScale( { 1.0f, 1.0f, 1.0f } );
	m_pStaticMeshs[m_ItemNo]->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pStaticMeshs[m_ItemNo]->Render();
	m_pStaticMeshs[m_ItemNo]->SetRasterizerState( CCommon::enRS_STATE::None );
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

// モデルの取得.
bool CItemEdit::GetModel()
{
	//読み込むモデル名設定.
	const char* modelNames[] =
	{
		CURE_MODEL_NAME,
		ABILITY_MODEL_NAME,
		ATTACK_MODEL_NAME,
		SPEED_MODEL_NAME,
	};
	const int modelMax = sizeof(modelNames) / sizeof(modelNames[0]);

	// 各情報の設定.
	for( int no = 0; no < modelMax; no++ )
	{
		m_pStaticMeshs.emplace_back();
		if( CMeshResorce::GetStatic( m_pStaticMeshs[no], modelNames[no] ) == false ) return false;
	}
	return true;
}