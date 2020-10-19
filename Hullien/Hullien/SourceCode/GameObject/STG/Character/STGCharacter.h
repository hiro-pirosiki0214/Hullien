#ifndef STG_CHARACTER_H
#define STG_CHARACTER_H

#include "..\STGActor.h"
#include <vector>

namespace STG
{
	class CCharacter : public STG::CActor
	{
	public:
		CCharacter();
		virtual ~CCharacter();

		// ’e‚Ìæ“¾.
		virtual std::vector<std::shared_ptr<CBullet>> GetBullets(){ return m_pBullets; }

	protected:
		// ˆÚ“®ŠÖ”.
		virtual void Move() = 0;
		// ’e‚Ì‰Šú‰».
		virtual bool BulletInit( 
			std::vector<std::shared_ptr<CBullet>>& bullets, 
			const int& bulletCount,
			const char* modelName );
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