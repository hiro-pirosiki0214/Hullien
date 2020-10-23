#include "VolumeConifgWidget.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Slider\Slider.h"
#include "..\..\..\Cursor\Cursor.h"

/********************************************
*	設定UIクラス.
**/
CVolumeConfigWidget::CVolumeConfigWidget()
	: m_pCursor				( nullptr )
	, m_pVolumeSlinders		( ESelectType_VolumeMax )
	, m_pSprites			()
	, m_SlinderPositions	( ESelectType_VolumeMax )
	, m_CursorPosition		( 0.0f, 0.0f, 0.0f )
	, m_NowConfigState		( EConfigState_Select )
	, m_NowSelectVolume		( ESelectType_Master )
	, m_OldSelectVolume		( ESelectType_Master )
	, m_InputWaitTime		( 0.0f )
	, m_IsOneStep			( false )
{
	m_pCursor = std::make_unique<CCursor>();
	for( auto& v : m_pVolumeSlinders ) v = std::make_shared<CSlinder>();

	// 現在の音量の値の取得.
	m_pVolumeSlinders[ESelectType_Master]->SetValue( CSoundManager::GetInstance()->m_fMasterVolume );
	m_pVolumeSlinders[ESelectType_BGM]->SetValue( CSoundManager::GetInstance()->m_fMaxBGMVolume );
	m_pVolumeSlinders[ESelectType_SE]->SetValue( CSoundManager::GetInstance()->m_fMaxSEVolume );
}

CVolumeConfigWidget::~CVolumeConfigWidget()
{
}

// 初期化関数.
bool CVolumeConfigWidget::Init()
{
	if( SpriteSetting() == false ) return false;
	if( m_pCursor->Init() == false ) return false;
	for( auto& v : m_pVolumeSlinders ) if( v->Init() == false ) return false;

	return true;
}

// 更新関数.
void CVolumeConfigWidget::Update()
{
	m_InputWaitTime--;
	switch( m_NowConfigState )
	{
	case EConfigState_Select:
		SelectType();		// 設定する種類の選択.
		Determination();	// 決定後の処理.
		// キャンセル.
		if( GetAsyncKeyState(VK_BACK) & 0x0001 || 
			CXInput::A_Button() == CXInput::enPRESSED_MOMENT ){
			m_NowSelectVolume = EVolumeType_End;
			CSoundManager::PlaySE("CancelDetermination");
		}
		break;
	case EConfigState_Seting:
		VolumeSeting();		// 音量の設定.
		// 決定.
		if( GetAsyncKeyState(VK_RETURN) & 0x0001 ||
			CXInput::B_Button() == CXInput::enPRESSED_MOMENT ){
			CSoundManager::PlaySE("Determination");
			m_NowConfigState = EConfigState_Select;
		}
		break;
	default:
		break;
	}
	for( auto& v : m_pVolumeSlinders ) v->Update();	// スライダーの更新.
	m_pCursor->Update();	// カーソルの更新.
}

// 描画関数.
void CVolumeConfigWidget::Render()
{
	// カーソルの座標設定.
	m_pCursor->SetPosition( m_pSprites[m_NowSelectVolume]->GetRenderPos() );
	m_pCursor->Render();
	// スライダーの描画.
	for( int i = ESelectType_Begin; i < ESelectType_End; i++ ){
		if( i == ESpriteNo_SelectIcon ) continue;
		if( i <= ESpriteNo_SE ){
			m_pVolumeSlinders[i]->SetPosition(m_SlinderPositions[i] );
			m_pVolumeSlinders[i]->Render();
		}
		m_pSprites[i]->SetDeprh( false );
		m_pSprites[i]->RenderUI();
		m_pSprites[i]->SetDeprh( true );
	}
	// 選択中のアイコンの表示.
	if( m_NowConfigState == EConfigState_Seting ){
		m_pSprites[ESpriteNo_SelectIcon]->SetPosition( m_pVolumeSlinders[m_NowSelectVolume]->GetIconPosition() );
		m_pSprites[ESpriteNo_SelectIcon]->SetDeprh( false );
		m_pSprites[ESpriteNo_SelectIcon]->RenderUI();
		m_pSprites[ESpriteNo_SelectIcon]->SetDeprh( true );
	}
}

// 音量の選択.
void CVolumeConfigWidget::SelectType()
{
	if( m_InputWaitTime > 0.0f ) return;

	// 上に移動.
	if( GetAsyncKeyState(VK_UP) & 0x0001 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX ){
		m_NowSelectVolume--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectVolume = m_NowSelectVolume <= ESelectType_Master ? ESelectType_Master : m_NowSelectVolume;
	} 
	// 下に移動.
	if( GetAsyncKeyState(VK_DOWN) & 0x0001 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN ){
		m_NowSelectVolume++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectVolume = m_NowSelectVolume >= ESelectType_Save ? ESelectType_Save : m_NowSelectVolume;
	} 
	// 左に移動.
	if( GetAsyncKeyState(VK_LEFT) & 0x0001 || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN ){
		if( m_NowSelectVolume >= ESelectType_Reset ){
			m_NowSelectVolume--;
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			m_NowSelectVolume = m_NowSelectVolume <= ESelectType_Master ? ESelectType_Master : m_NowSelectVolume;
		}
	} 
	// 右に移動.
	if( GetAsyncKeyState(VK_RIGHT) & 0x0001 || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX ){
		if( m_NowSelectVolume >= ESelectType_SE ){
			m_NowSelectVolume++;
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			m_NowSelectVolume = m_NowSelectVolume >= ESelectType_Save ? ESelectType_Save : m_NowSelectVolume;
		}
	}
	// SEを鳴らす.
	if( m_NowSelectVolume != m_OldSelectVolume ){
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldSelectVolume = m_NowSelectVolume;
	}
}

// 決定.
void CVolumeConfigWidget::Determination()
{
	if(!(GetAsyncKeyState(VK_RETURN) & 0x0001 ) && CXInput::B_Button() != CXInput::enPRESSED_MOMENT ) return;
	switch( m_NowSelectVolume )
	{
	case ESelectType_Master:
	case ESelectType_BGM:
	case ESelectType_SE:
		// 音量の設定へ移動.
		m_NowConfigState = EConfigState_Seting;
		CSoundManager::PlaySE("Determination");
		break;
	case ESelectType_Reset:
		// 音量をリセットする.
		m_pVolumeSlinders[ESelectType_Master]->SetValue( DEFALUT_VOLUME );
		m_pVolumeSlinders[ESelectType_BGM]->SetValue( DEFALUT_VOLUME );
		m_pVolumeSlinders[ESelectType_SE]->SetValue( DEFALUT_VOLUME );
		CSoundManager::GetInstance()->m_fMasterVolume	= DEFALUT_VOLUME;
		CSoundManager::GetInstance()->m_fMaxBGMVolume	= DEFALUT_VOLUME;
		CSoundManager::GetInstance()->m_fMaxSEVolume	= DEFALUT_VOLUME;
		m_NowSelectVolume = ESelectType_Master;
		break;
	case ESelectType_Save:
		// 音量を保存する.
		if( CSoundManager::SaveVolume() == true ) m_NowSelectVolume = EVolumeType_End;
		break;
	default:
		break;
	}
}

// 音量の設定.
void CVolumeConfigWidget::VolumeSeting()
{
	// 値を増やす.
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX )
		m_pVolumeSlinders[m_NowSelectVolume]->AddValue( 0.01f );
	// 値を減らす.
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN )
		m_pVolumeSlinders[m_NowSelectVolume]->SubValue( 0.01f );
	
	// 設定した値をサウンドマネージャーへ渡す.
	switch( m_NowSelectVolume )
	{
	case ESelectType_Master:
		CSoundManager::GetInstance()->m_fMasterVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
		break;
	case ESelectType_BGM:
		CSoundManager::GetInstance()->m_fMaxBGMVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
		break;
	case ESelectType_SE:
		CSoundManager::GetInstance()->m_fMaxSEVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
		break;
	default:
		break;
	}
}

// 音量の設定をできるようにする.
void CVolumeConfigWidget::OnVolumeSeting()
{
	if( m_IsOneStep == true ) return;
	// 音量の設定を許可する.
	CSoundManager::CreateChangeSoundVolumeThread();
	CSoundManager::StateChangeVolumeThread( true );

	// 現在の音量の値の取得.
	m_pVolumeSlinders[ESelectType_Master]->SetValue( CSoundManager::GetInstance()->m_fMasterVolume );
	m_pVolumeSlinders[ESelectType_BGM]->SetValue( CSoundManager::GetInstance()->m_fMaxBGMVolume );
	m_pVolumeSlinders[ESelectType_SE]->SetValue( CSoundManager::GetInstance()->m_fMaxSEVolume );
	m_IsOneStep = true;
}

// 音量の設定をできないようにする.
void CVolumeConfigWidget::OffVolumeSeting()
{
	// 音量の設定を許可する.
	CSoundManager::StateChangeVolumeThread( false );
}

// 保存終了.
bool CVolumeConfigWidget::IsSaveEnd()
{
	if( m_NowSelectVolume == EVolumeType_End ){
		m_OldSelectVolume = m_NowSelectVolume = ESelectType_Master;
		return true;
	}
	return false;
}

// スプライト設定関数.
bool CVolumeConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_MASTER_NAME,	// マスター.
		SPRITE_BGM_NAME,	// BGM.
		SPRITE_SE_NAME,		// SE.
		SPRITE_RESET,		// リセット.
		SPRITE_SAVE,		// 保存.
		SPRITE_ICON_NAME,	// アイコン.
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
		if( sprite <= ESpriteNo_SE ){
			m_SlinderPositions[sprite] = m_pSprites[sprite]->GetRenderPos();
			m_SlinderPositions[sprite].x += BAR_POSITION_X;
			m_SlinderPositions[sprite].y += m_pSprites[sprite]->GetSpriteSize().y/2.0f;
		}
	}

	return true;
}