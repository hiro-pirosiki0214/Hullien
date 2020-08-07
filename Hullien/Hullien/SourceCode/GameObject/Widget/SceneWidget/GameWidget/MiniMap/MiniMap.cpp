#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"

/**********************************
*	ミニマップクラス.
**/
CMiniMap::CMiniMap()
	: m_pSprite					()
	, m_ObjPosListCount	( 0 )

{
}

CMiniMap::~CMiniMap()
{
}

// 初期化関数.
bool CMiniMap::Init()
{
	// 初期スプライト読み込み.
	if (SpriteSetting() == false ) return false;

	return true;
}

// 更新関数.
void CMiniMap::Update()
{
}

// オブジェクト位置設定関数.
void CMiniMap::SetObjPosition(CGameActorManager* pObj)
{
	// スプライトの設定.
	OBJLIST List = pObj->GetObjPositionList();
	SpriteSetting( List );

	// アイコン位置の更新.
	int objCount = 0;
	for (const auto& obj : pObj->GetObjPositionList())
	{
		objCount++;
		m_vPosition[objCount].x = m_pSprite[MAP_BACK]->GetRenderPos().x - obj.second.x;
		m_vPosition[objCount].y = m_pSprite[MAP_BACK]->GetRenderPos().y + obj.second.z;
	}
}

// 描画関数.
void CMiniMap::Render()
{
	for ( size_t sprite = 0; sprite < m_pSprite.size(); sprite++ )
	{
		m_pSprite[sprite]->SetPosition( m_vPosition[sprite] );
		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh( true );
	}
}

// スプライト設定関数.
bool CMiniMap::SpriteSetting()
{
	if ( m_pSprite.size() != 0 ) return true;

	// マップ背景の読み込み.
	m_pSprite.emplace_back( CSpriteResource::GetSprite(SPRITE_MAP_BACK) );
	m_vPosition.emplace_back( m_pSprite[MAP_BACK]->GetRenderPos() );
	if( m_pSprite[MAP_BACK] == nullptr ) return false;

	return true;
}

// スプライト設定関数.
void CMiniMap::SpriteSetting(OBJLIST objList)
{
	if (m_ObjPosListCount > objList.size()) return;

	for (OBJLIST::const_iterator obj = objList.begin() + m_ObjPosListCount; obj < objList.end(); obj++)
	{
		m_ObjPosListCount++;

		switch (obj->first)
		{
		case EObjectTag::Player:
		case EObjectTag::Girl:
		case EObjectTag::Alien_B:
		case EObjectTag::Alien_A:
		case EObjectTag::Alien_C:
		case EObjectTag::Alien_D:
			ObjSpriteSetting( SPRITE_DEFAULT_ICON );
			break;

		default:
			break;
		}
	}
}

// オブジェクトのスプライト設定関数.
void CMiniMap::ObjSpriteSetting(const char* spriteName)
{
	// 読み込むスプライトの設定.
	m_pSprite.emplace_back(CSpriteResource::GetSprite( spriteName ));
	// 初期位置の設定.
	m_vPosition.emplace_back( m_pSprite[MAP_BACK]->GetRenderPos() );
}

