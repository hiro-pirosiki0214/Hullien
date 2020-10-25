#include "ConfigWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\Cursor\Cursor.h"
#include "VolumeConfigWidget/VolumeConfigWidget.h"
#include "CameraConfigWidget/CameraConfigWidget.h"

/********************************************
*	設定UIクラス.
**/
CConfigWidget::CConfigWidget()
	: m_pSprites		()
	, m_pCursor			( nullptr )
	, m_pVolumeConfig	( nullptr )
	, m_pCameraConfig	( nullptr )
	, m_SelectState		( EConfigState_Volume )
	, m_NowConfigState	( EConfigState_None )
{
	m_pCursor = std::make_unique<CCursor>();
	m_pVolumeConfig = std::make_unique<CVolumeConfigWidget>();
	m_pCameraConfig = std::make_unique<CCameraConfigWidget>();
}

CConfigWidget::~CConfigWidget()
{
}

// 初期化関数.
bool CConfigWidget::Init()
{
	if( SpriteSetting() == false ) return false;
	if( m_pCursor->Init() == false ) return false;
	if( m_pVolumeConfig->Init() == false ) return false;
	if( m_pCameraConfig->Init()	== false ) return false;
	return true;
}

// 更新関数.
void CConfigWidget::Update()
{
	m_InputWaitTime--;
	m_pVolumeConfig->OnVolumeSeting();	// 音量の設定を有効にする.
	switch( m_NowConfigState )
	{
	case EConfigState_None:
		CursorSetting();
		m_pCursor->Update();
		if( GetAsyncKeyState(VK_RETURN) & 0x0001 || 
			CXInput::B_Button() == CXInput::enPRESSED_MOMENT ){
			m_NowConfigState = m_SelectState; 
			CSoundManager::PlaySE("Determination");
		}
		if( GetAsyncKeyState(VK_BACK) & 0x0001 || 
			CXInput::A_Button() == CXInput::enPRESSED_MOMENT ){
			m_NowConfigState = EConfigState_End;
			CSoundManager::PlaySE("CancelDetermination");
		}
		break;
	case EConfigState_Volume:
		m_pVolumeConfig->Update();	// 音量設定UIの更新.
		if( m_pVolumeConfig->IsSaveEnd() == true ) m_NowConfigState = EConfigState_None;
		break;
	case EConfigState_Camera:
		m_pCameraConfig->Update();
		if( GetAsyncKeyState(VK_BACK) & 0x0001 || CXInput::A_Button() == CXInput::enPRESSED_MOMENT ){
			m_NowConfigState = EConfigState_None;
			CSoundManager::PlaySE("CancelDetermination");
		}
		break;
	default:
		break;
	}
}

// 描画関数.
void CConfigWidget::Render()
{
	for( size_t i = 0; i < m_pSprites.size(); i++ ){
		m_pSprites[i]->SetDeprh(false);
		m_pSprites[i]->RenderUI();
		m_pSprites[i]->SetDeprh(false);
		if( m_NowConfigState != EConfigState_None || i != BACKGROUND ) continue;
		m_pCursor->SetPosition( m_vPosition );
		m_pCursor->Render();
	}
	switch( m_NowConfigState )
	{
	case EConfigState_None:
		break;
	case EConfigState_Volume:
		m_pVolumeConfig->Render();	// 音量設定UIの描画.
		break;
	case EConfigState_Camera:
		m_pCameraConfig->Render();
		break;
	default:
		break;
	}
}

// 音量の設定をできないようにする.
void CConfigWidget::OffVolumeSeting()
{
	m_pVolumeConfig->OffVolumeSeting();
}

// 設定を終了したか.
bool CConfigWidget::IsEndConfig()
{
	return m_NowConfigState == EConfigState_End;
}

// カーソルの設定.
void CConfigWidget::CursorSetting()
{
	if( m_InputWaitTime <= 0.0f ){
		if( GetAsyncKeyState( VK_UP ) & 0x8000 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX )
		{
			m_SelectState--;
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			if( m_SelectState <= EConfigState_Volume ){
				m_SelectState = EConfigState_Volume;
				m_InputWaitTime = 0.0f;
			}
		}
		if( GetAsyncKeyState( VK_DOWN ) & 0x8000 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN )
		{
			m_SelectState++;
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			if( m_SelectState >= EConfigState_Camera ){
				m_SelectState = EConfigState_Camera;
				m_InputWaitTime = 0.0f;
			}
		}
	}

	switch( m_SelectState )
	{
	case EConfigState_Volume:
		m_vPosition = m_pSprites[VOLUME]->GetRenderPos();
		break;
	case EConfigState_Camera:
		m_vPosition = m_pSprites[CAMERA]->GetRenderPos();
		break;
	default:
		break;
	}

	m_pCursor->Update();
}

// スプライト設定関数.
bool CConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_BACK,
		SPRITE_VOLUME,
		SPRITE_CAMERA,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
	}
	return true;
}