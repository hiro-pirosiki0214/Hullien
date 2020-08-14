#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"

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
	OBJLIST List = pObj->GetObjPositionList();
	SpriteSetting( List );

	// アイコン位置の更新.
	int objCount = 1;	// マップ下地は含めないので配列番号は1から.
	for (const auto& obj : pObj->GetObjPositionList())
	{
		// タグ情報の更新.
		if (obj.first != m_IconList[objCount].EObjTag) m_IconList[objCount].EObjTag = obj.first;

		// Noneならば処理しない.
		if (obj.first == EObjectTag::None) continue;
		// 位置情報の更新.
		m_IconList[objCount].Pos.x = m_IconList[MAP_BACK].pSprite->GetRenderPos().x - obj.second.x;
		m_IconList[objCount].Pos.y = m_IconList[MAP_BACK].pSprite->GetRenderPos().y + obj.second.z;
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
		l.pSprite->SetDeprh( false );
		l.pSprite->RenderUI();
		l.pSprite->SetDeprh( true );
	}
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
		case EObjectTag::SPEffectTimeItem:
		case EObjectTag::AttackUpItem:
		case EObjectTag::LifeRecoveryItem:
		case EObjectTag::MoveSpeedUpItem:
		case EObjectTag::None:
			ObjSpriteSetting(SPRITE_DEFAULT_ICON, obj->first);
			break;

		default:
			// タグがNoneならオブジェクトが存在しないのでリストを減らす.
			//m_IconList[m_ObjPosListCount] = m_IconList.back();
			//m_IconList.pop_back();
			break;
		}
	}
}

// オブジェクトのスプライト設定関数.
void CMiniMap::ObjSpriteSetting(const char* spriteName, const EObjectTag& tag)
{
	SIconInfo info;
	// 読み込むスプライトの設定.
	info.pSprite = CSpriteResource::GetSprite(spriteName);
	// 初期位置の設定.
	info.Pos = m_IconList[MAP_BACK].pSprite->GetRenderPos();
	// タグの設定.
	info.EObjTag = tag;
	// リストを増やす.
	m_IconList.emplace_back(info);
}

