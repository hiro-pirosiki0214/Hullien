#ifndef TITLE_H
#define TITLE_H

#include "..\..\SceneBase\SceneBase.h"

#include "..\..\..\Common\Sprite\CSprite.h"

#include "..\..\..\Collider\Colliders\Capsule\CapsuleModel.h"
#include "..\..\..\Collider\Colliders\Capsule\CapsuleCollision.h"


class CRotLookAtCenter;
class CTitleWidget;

/**********************************
*	タイトルシーンクラス.
*/
class CTitle : public CSceneBase
{
public:
	CTitle(CSceneManager* pSceneManager);
	virtual ~CTitle();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;


private:
	std::unique_ptr<CTitleWidget>	m_pWidget;		//UIクラス.

};

#endif	// #ifndef TITLE_H.