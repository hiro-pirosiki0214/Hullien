#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"

/**********************************
*	ミニマップクラス.
**/
CMiniMap::CMiniMap()
	: m_mView					()
	, m_mProj					()
	, m_pSprite					()
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
	if (InitSpriteSetting() == false ) return false;

	return true;
}

// 更新関数.
void CMiniMap::Update()
{
}

void CMiniMap::SetPosition(CGameActorManager* pObj)
{
	SpriteSetting( pObj );

	for (const auto& obj : pObj->GetObjPositionList())
	{
		switch (obj.first)
		{
		case EObjectTag::Player:
			m_vPosition[1].x = m_pSprite[0]->GetRenderPos().x - obj.second.x;
			m_vPosition[1].y = m_pSprite[0]->GetRenderPos().y + obj.second.z;
			break;
		case EObjectTag::Girl:
			m_vPosition[2].x = m_pSprite[0]->GetRenderPos().x - obj.second.x;
			m_vPosition[2].y = m_pSprite[0]->GetRenderPos().y + obj.second.z;
			break;
		case EObjectTag::Alien_B:
		case EObjectTag::Alien_A:
			break;
		default:
			break;
		}
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

// 描画関数.
void CMiniMap::Render(const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	m_mView = view;
	m_mProj = proj;

	Render();
}

// スプライト設定関数.
bool CMiniMap::InitSpriteSetting()
{
	if ( m_pSprite.size() != 0 ) return true;

	const char* spriteName[] = 
	{
		"minimapsize",
	};
	const int spriteMax = sizeof( spriteName ) / sizeof( spriteName[0] );

	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back( CSpriteResource::GetSprite( spriteName[sprite] ) );
		m_vPosition.emplace_back( m_pSprite[sprite]->GetRenderPos() );
		if( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}

// スプライト設定関数.
void CMiniMap::SpriteSetting(CGameActorManager * pObj)
{
	std::vector<std::pair<EObjectTag, D3DXVECTOR3>> v = pObj->GetObjPositionList();

	if (m_ObjPosListCount > pObj->GetObjPositionList().size()) return;

	for (std::vector<std::pair<EObjectTag, D3DXVECTOR3>>::const_iterator obj = v.begin() + m_ObjPosListCount; obj < v.end(); obj++)
	{
		m_ObjPosListCount++;
		switch (obj->first)
		{
		case EObjectTag::Player:
			m_pSprite.emplace_back(CSpriteResource::GetSprite("mapiconsize"));
			m_vPosition.emplace_back(D3DXVECTOR3(
				m_pSprite[0]->GetRenderPos().x - obj->second.x,
				m_pSprite[0]->GetRenderPos().y + obj->second.z,
				0.0f
			));
			break;
		case EObjectTag::Girl:
			m_pSprite.emplace_back(CSpriteResource::GetSprite("mapiconsize"));
			m_vPosition.emplace_back(D3DXVECTOR3(
				m_pSprite[0]->GetRenderPos().x - obj->second.x,
				m_pSprite[0]->GetRenderPos().y + obj->second.z,
				0.0f
			));
			break;
		case EObjectTag::Alien_B:
		case EObjectTag::Alien_A:
			break;

		default:
			break;
		}
	}
}
