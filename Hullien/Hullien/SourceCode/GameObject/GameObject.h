#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "..\Global.h"

#include "ObjectList.h"

/****************************************
*	ゲームオブジェクト元クラス.
**/
class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	// 初期化関数.
	virtual bool Init() = 0;
	// 更新関数.
	virtual void Update() = 0;
	// 描画関数.
	virtual void Render() = 0;

	// オブジェクトタグ取得関数.
	EObjectTag GetObjectTag() const;
	// 座標取得関数.
	D3DXVECTOR3 GetPosition() const;
	// 座標設定関数.
	virtual void SetPosition( const D3DXVECTOR3& vPos );

protected:
	EObjectTag	m_ObjectTag;	// オブジェクトタグ.
	D3DXVECTOR3 m_vPosition;	// 座標.
	D3DXVECTOR3 m_vRotation;	// 回転.
	D3DXVECTOR3 m_vSclae;		// 大きさ.
};

#endif	// #ifndef GAME_OBJECT_H.