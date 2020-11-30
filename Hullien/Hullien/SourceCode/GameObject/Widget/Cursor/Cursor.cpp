#include "Cursor.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/********************************************
*	カーソルクラス.
**/
CCursor::CCursor()
	:	m_pSelectSprite(nullptr)
	,	m_vOldPosition(D3DXVECTOR3( 0.0f, 0.0f ,0.0f ))
	,	m_Acceleration( 0.0f )
{
	m_vScale.x = 0.0f;
}

CCursor::~CCursor()
{
}

// 初期化関数.
bool CCursor::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// 更新関数.
void CCursor::Update()
{
	if (m_pSprite == nullptr) return;
	MoveScale();	// スケールの動き.
}

// 描画関数.
void CCursor::Render()
{
	if (m_pSprite == nullptr) return;

	m_vPosition.x -= 67.0f;
	m_vPosition.y -= 5.0f;
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetScale( m_vScale );

	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );

	m_SlectPosition = m_vPosition;
	m_SlectPosition.x -= 10.0f;
	m_pSelectSprite->SetPosition( m_SlectPosition );
	m_pSelectSprite->SetDeprh( false );
	m_pSelectSprite->SetBlend( true );
	m_pSelectSprite->RenderUI();
	m_pSelectSprite->SetBlend( false );
	m_pSelectSprite->SetDeprh( true );
}

// スプライト設定関数.
bool CCursor::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_NAME);
	if (m_pSprite == nullptr) return false;
	m_pSelectSprite = CSpriteResource::GetSprite(SPRITE_NAME2);
	if (m_pSelectSprite == nullptr) return false;
	m_SlectPosition = m_pSelectSprite->GetRenderPos();
	return true;
}

// スケール動き関数.
void CCursor::MoveScale()
{
	// 移動前の座標と現在の座標が違う場合.
	if (m_vPosition != m_vOldPosition)
	{
		// 数値を初期化.
		m_vScale.x = 0.0f;
		m_Acceleration = 0.0f;
	}

	// 拡大値が標準ならば処理しない.
	if (m_vScale.x >= SCALE_MAX) return;
	// 拡大.
	IncreaseScale();
}

// 拡大関数.
void CCursor::IncreaseScale()
{
	m_vOldPosition = m_vPosition;	// 現在地を保持する.

	m_vScale.x += SCALE_SPEED - m_Acceleration;
	m_Acceleration += ACC_SPEED;

	// 拡大値が標準値になった時.
	if ( m_vScale.x >= SCALE_MAX)
	{
		m_vScale.x = SCALE_MAX;
		m_Acceleration = 0.0f;	// 加速度の初期化.
	}
}

