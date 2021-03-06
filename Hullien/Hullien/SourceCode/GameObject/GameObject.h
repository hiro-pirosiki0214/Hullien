#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "..\Global.h"
#include "..\Common\Shader\ShadowMap\ShadowMap.h"
#include "ObjectList.h"

/****************************************
*	ゲームオブジェクト元クラス.
**/
class CGameObject
{
	// ウィンドウ外の調整値.
	inline static const float WND_OUT_ADJ_SIZE = 300.0f;
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
	inline EObjectTag GetObjectTag() const { return m_ObjectTag; }
	// 座標取得関数.
	inline D3DXVECTOR3 GetPosition() const { return m_vPosition; }
	// 座標設定関数.
	virtual void SetPosition( const D3DXVECTOR3& vPos ){ m_vPosition = vPos; }
	// Y軸の回転値の取得.
	inline float GetRotatinY() const { return m_vRotation.y; }
	// X軸の回転値の設定.
	inline void SetRotationX( const float& rotX ){ m_vRotation.x = rotX; }
	// Y軸の回転値の設定.
	inline void SetRotationY( const float& rotY ){ m_vRotation.y = rotY; }
	// Z軸の回転値の設定.
	inline void SetRotationZ( const float& rotZ ){ m_vRotation.z = rotZ; }
	// サイズの設定.
	inline void SetScale( const float& size ){ m_vScale = { size, size, size }; }
	// サイズの設定.
	inline void SetScale( const D3DXVECTOR3& vScale ){ m_vScale = vScale; }

	// 画面の外に出ているか.
	bool IsDisplayOut( const float& adjSize = WND_OUT_ADJ_SIZE );

protected:
	EObjectTag	m_ObjectTag;	// オブジェクトタグ.
	D3DXVECTOR3 m_vPosition;	// 座標.
	D3DXVECTOR3 m_vRotation;	// 回転.
	D3DXVECTOR3 m_vScale;		// 大きさ.
};

#endif	// #ifndef GAME_OBJECT_H.