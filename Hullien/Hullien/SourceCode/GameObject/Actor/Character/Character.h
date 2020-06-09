#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"

class CCharacter : public CActor
{
public:
	CCharacter();
	virtual ~CCharacter();

	// 移動関数.
	virtual void Move() = 0;

protected:
	// モデルの取得.
	bool GetModel( const char* modelName );

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;	// スキンメッシュ.
	float		m_Life;			// 体力.
	D3DXVECTOR3	m_MoveVector;	// 移動ベクトル.
};

#endif	// #ifndef CHARACTER_H.