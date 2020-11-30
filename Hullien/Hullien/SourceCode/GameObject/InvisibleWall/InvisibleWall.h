#ifndef INVISIBLE_WALL_H
#define INVISIBLE_WALL_H

#include "..\GameObject.h"
#include "BoxStruct.h"

class CCollisionManager;	// 当たり判定クラス.

/**************************************
*	見えない壁.
*/
class CInvisibleWall : public CGameObject
{
	const float HALF = 0.5f;	// 半分.
	const char* INVISIBLE_WALL_PARAM_FILE_NAME = "Data\\GameParam\\InvisibleWall.bin";
public:
	CInvisibleWall();
	virtual ~CInvisibleWall();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// ボックスの設定.
	void SetBoxWall( const SBoxWall& boxWall );
	// ボックスの取得.
	inline SBoxWall* GetBoxWall() { return &m_BoxWall; }

private:
	// パラメータの初期化.
	bool InitParam();
	// 当たり判定の初期化.
	bool InitCollision();

private:
	SBoxWall	m_BoxWall;
#ifdef _DEBUG
	std::unique_ptr<CCollisionManager>	m_pCollision;
#endif	// #ifdef _DEBUG
};

#endif	// #ifndef INVISIBLE_WALL_H.