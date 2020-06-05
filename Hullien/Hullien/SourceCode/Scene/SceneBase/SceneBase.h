#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include <Windows.h>
#include <string>
#include <stack>
#include <memory>

#include "..\..\Global.h"

class CSceneManager;

class CSceneBase
{
public:
	CSceneBase( CSceneManager* pSceneManager )
		: m_pSceneManager	( pSceneManager )
	{}

	virtual ~CSceneBase(){};
	
	// 読込関数.
	virtual bool Load() = 0;
	// 更新関数.
	virtual void Update() = 0;
	// 描画関数.
	virtual void Render() = 0;

protected:
	CSceneManager* m_pSceneManager;	// シーン情報.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CSceneBase( const CSceneBase & )				= delete;
	CSceneBase& operator = ( const CSceneBase & )	= delete;
	CSceneBase( CSceneBase && )						= delete;
	CSceneBase& operator = ( CSceneBase && )		= delete;
};

#endif // #ifndef SCENE_BASE_H.