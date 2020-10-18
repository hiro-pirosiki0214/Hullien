#ifndef STG_CHARACTER_H
#define STG_CHARACTER_H

#include "..\STGActor.h"
#include <vector>

namespace STG
{
	class CBullet;	// ’eƒNƒ‰ƒX.

	class CCharacter : public STG::CActor
	{
	public:
		CCharacter();
		virtual ~CCharacter();

	protected:
		// ˆÚ“®ŠÖ”.
		virtual void Move() = 0;
		// ’e‚Ì‰Šú‰».
		virtual bool BulletInit( const char* modelName );
		// ’e‚ÌXV.
		virtual void BulletUpdate();
		// ’e‚Ì•`‰æ.
		virtual void BulletRender();
		// ’e‚ğŒ‚‚Â.
		virtual void BulletShot( const float& rot, const float& moveSpeed );

	protected:
		std::vector<std::shared_ptr<CBullet>> m_pBullets;
		int m_ShotCount;
	};

};

#endif	// #ifndef STG_CHARACTER_H.