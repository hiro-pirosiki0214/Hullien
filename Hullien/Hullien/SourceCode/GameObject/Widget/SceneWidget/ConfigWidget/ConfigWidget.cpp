#include "ConfigWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "VolumeConifgWidget/VolumeConifgWidget.h"

/********************************************
*	設定UIクラス.
**/
CConfigWidget::CConfigWidget()
	: m_pVolumeConfig	( nullptr )
	, m_NowConfigState	( EConfigState_None )
{
	m_pVolumeConfig = std::make_unique<CVolumeConfigWidget>();
}

CConfigWidget::~CConfigWidget()
{
}

// 初期化関数.
bool CConfigWidget::Init()
{
	if( m_pVolumeConfig->Init() == false ) return false;
	return true;
}

// 更新関数.
void CConfigWidget::Update()
{
	if( GetAsyncKeyState('B') & 0x0001 ){
		m_NowConfigState = EConfigState_Volume;
		m_pVolumeConfig->OnVolumeSeting();
	}
	if( GetAsyncKeyState('V') & 0x0001 ){
		m_NowConfigState = EConfigState_None;
		m_pVolumeConfig->OffVolumeSeting();
	}
	switch( m_NowConfigState )
	{
	case EConfigState_Volume:
		m_pVolumeConfig->Update();	// 音量設定UIの更新.
		break;
	case EConfigState_Key:
		break;
	default:
		break;
	}
}

// 描画関数.
void CConfigWidget::Render()
{
	switch( m_NowConfigState )
	{
	case EConfigState_Volume:
		m_pVolumeConfig->Render();	// 音量設定UIの描画.
		break;
	case EConfigState_Key:
		break;
	default:
		break;
	}
}

// スプライト設定関数.
bool CConfigWidget::SpriteSetting()
{
	return true;
}