#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"
#include "..\..\..\..\..\Common\DebugText\DebugText.h"

/**********************************
*	ミニマップクラス.
**/
CMiniMap::CMiniMap()
	: m_IconList		()
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
	auto List = pObj->GetObjPositionList();
	SpriteSetting( List );

	// アイコン位置の更新.
	int objCount = 1;	// マップ下地は含めないので配列番号は1から.
	for (const auto& obj : pObj->GetObjPositionList())
	{
		// Noneならば処理しない.
		if (obj.first == EObjectTag::None) continue;
		// タグ情報の更新.
		if (obj.first != m_IconList[objCount].EObjTag) 
		{ 
			m_IconList[objCount].EObjTag = obj.first; 
			m_IconList[objCount].AnimNumber = SetAnimNumber(obj.first);
		}

		// 位置情報の更新.
		if (m_IconList[objCount].Pos == obj.second.first) return;
		m_IconList[objCount].Pos.x = m_IconList[MAP_BACK].pSprite->GetRenderPos().x - (obj.second.first.x * MAP_ICON_POSITION_CORRECTION_VALUE);
		m_IconList[objCount].Pos.y = m_IconList[MAP_BACK].pSprite->GetRenderPos().y + (obj.second.first.z * MAP_ICON_POSITION_CORRECTION_VALUE);
		m_IconList[objCount].Rot.z = obj.second.second;
		objCount++;
	}
}

// 描画関数.
void CMiniMap::Render()
{
	if (m_IconList.size() == 0) return;
	for (const auto& l : m_IconList)
	{
		// タグがNoneならば非表示.
		if (l.EObjTag == EObjectTag::None) continue;
		l.pSprite->SetPosition(l.Pos );
		l.pSprite->SetRotation(l.Rot );
		l.pSprite->SetAnimNumber(l.AnimNumber );
		l.pSprite->SetDeprh( false );
		l.pSprite->SetBlend( true );
		l.pSprite->RenderUI();
		l.pSprite->SetBlend( false );
		l.pSprite->SetDeprh( true );
	}
	// マップの描画.
	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );
}

// スプライト設定関数.
bool CMiniMap::SpriteSetting()
{
	if ( m_IconList.size() != 0 ) return true;

	// マップ背景の読み込み.
	m_IconList.emplace_back();
	m_IconList[MAP_BACK].pSprite = CSpriteResource::GetSprite(SPRITE_MAP_BACK);
	m_IconList[MAP_BACK].Pos = m_IconList[MAP_BACK].pSprite->GetRenderPos();
	if(m_IconList[MAP_BACK].pSprite == nullptr ) return false;

	m_pSprite = CSpriteResource::GetSprite(SPRITE_MAP_FRONT);
	if( m_pSprite == nullptr ) return false;

	return true;
}

// スプライト設定関数.
void CMiniMap::SpriteSetting(OBJLIST objList)
{
	if(m_ObjPosListCount == objList.size()) return;
	// カウントがオブジェクト数を超えたら.
	// オブジェクト数分まで減らす.
	if (m_ObjPosListCount > objList.size())
	{
		m_ObjPosListCount = objList.size();
		m_IconList.resize(objList.size()+1);	// マップ下地の分+1.
		m_IconList.shrink_to_fit();
		return;
	}

	// リストを増やす.
	for (auto obj = objList.begin() + m_ObjPosListCount; obj < objList.end(); obj++)
	{
		m_ObjPosListCount++;
		ObjSpriteSetting(SPRITE_DEFAULT_ICON, obj->first);
	}
}

// オブジェクトのスプライト設定関数.
void CMiniMap::ObjSpriteSetting(const char* spriteName, const EObjectTag& tag)
{
	SIconInfo info;
	// 読み込むスプライトの設定.
	info.pSprite = CSpriteResource::GetSprite( spriteName );
	// 初期位置の設定.
	info.Pos = m_IconList[MAP_BACK].pSprite->GetRenderPos();
	// タグの設定.
	info.EObjTag = tag;
	// アニメーション番号の設定.
	info.AnimNumber = SetAnimNumber( tag );
	// リストを増やす.
	m_IconList.emplace_back( info );
}

// アニメーション番号の設定関数.
int CMiniMap::SetAnimNumber(const EObjectTag& tag)
{
	int animNumber = 0;
	switch (tag)
	{
	case EObjectTag::Player:
		animNumber = static_cast<int>(EIconType::Player);
		break;
	case EObjectTag::Girl:
		animNumber = static_cast<int>(EIconType::Girl);
		break;
	case EObjectTag::Alien_A:
		animNumber = static_cast<int>(EIconType::Alien_A);
		break;
	case EObjectTag::Alien_B:
		animNumber = static_cast<int>(EIconType::Alien_B);
		break;
	case EObjectTag::Alien_C:
		animNumber = static_cast<int>(EIconType::Alien_C);
		break;
	case EObjectTag::Alien_D:
		animNumber = static_cast<int>(EIconType::Alien_D);
		break;
	case EObjectTag::SPEffectTimeItem:
	case EObjectTag::AttackUpItem:
	case EObjectTag::LifeRecoveryItem:
	case EObjectTag::MoveSpeedUpItem:
		animNumber = static_cast<int>(EIconType::Item);
		break;
	case EObjectTag::MotherShipUFO:
		animNumber = static_cast<int>(EIconType::MotherShipUFO);
		break;
	default:
		break;
	}

	return animNumber;
}

