#ifndef MINIMAP_H
#define MINIMAP_H

#include "..\..\SceneWidget.h"
#include <vector>

class CGameActorManager;	//アクタ管理クラス.

/**********************************
*	ミニマップクラス.
**/
class CMiniMap : public CSceneWidget
{
private:
	typedef std::pair<D3DXVECTOR3, float> pos_rot_pair;	// 座標と回転のペア.
	typedef std::vector<std::pair<EObjectTag, pos_rot_pair>> OBJLIST;

	const char* SPRITE_MAP_BACK		= "minimapBase";		//ミニマップ背景スプライト名.
	const char* SPRITE_MAP_FRONT	= "minimapWaku";		//ミニマップ背景スプライト名.
	const char* SPRITE_DEFAULT_ICON = "mapiconsize";	//標準アイコンスプライト名.
	const int	MAP_BACK = 0;							//ミニマップ背景配列番号.
	const float MAP_ICON_POSITION_CORRECTION_VALUE	= 0.55f;
	// アイコン情報.
	struct stIconInfo
	{
		std::shared_ptr<CSprite> pSprite;	// スプライト.
		D3DXVECTOR3 Pos;					// 位置.
		D3DXVECTOR3 Rot;					// 回転.
		EObjectTag EObjTag;					// タグ.
		int AnimNumber;						// アニメーション番号.

		stIconInfo()
			: pSprite	( nullptr )
			, Pos		(D3DXVECTOR3(0.0f,0.0f,0.0f))
			, Rot		(D3DXVECTOR3(0.0f,0.0f,0.0f))
			, EObjTag	(EObjectTag::Max)
			, AnimNumber (0)
		{}
	}typedef SIconInfo;

	// アイコンの種類.
	enum class IconType
	{
		Player = 0,
		Girl,
		Alien_A,
		Alien_B,
		Alien_C,
		Alien_D,
		Item,
		MotherShipUFO,
	}typedef EIconType;

public:
	CMiniMap();
	virtual ~CMiniMap();

	// 初期化関数.
	virtual bool Init() override;

	// 更新関数.
	virtual void Update() override;
	// オブジェクト位置設定関数.
	void SetObjPosition(CGameActorManager* pObj);

	// 描画関数.
	virtual void Render() override;

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	//スプライト設定関数.
	void SpriteSetting(OBJLIST objList);
	// オブジェクトのスプライト設定関数.
	void ObjSpriteSetting(const char* spriteName, const EObjectTag& tag);
	// アニメーション番号の設定関数.
	int SetAnimNumber(const EObjectTag& tag);

private:
	std::vector<SIconInfo>	m_IconList;			//アイコンリスト,
	size_t					m_ObjPosListCount;	//オブジェクトのカウント.

};

#endif	//#ifndef MINIMAP_H.
