#ifndef STG_ENEMY_H
#define STG_ENEMY_H

#include "..\STGCharacter.h"

namespace STG
{
	class CEnemy : public STG::CCharacter
	{
		const char* MODEL_NAME			= "a";
		const char* BULLET_MODEL_NAME	= "item cure";
		const float MOVE_SPEED			= 0.2f;
		const float BULLET_MOVE_SPEED	= 0.5f;	// ’e‚Ì‘¬“x.
		const int	BULLET_COUNT_MAX	= 30;	// ’e‚ÌÅ‘å”.
		const int	SHOT_INTERVAL_FRAME	= 20;	// ’e‚ğŒ‚‚ÂŠÔŠuƒtƒŒ[ƒ€.
	public:
		CEnemy();
		virtual ~CEnemy();

		// ‰Šú‰»ŠÖ”.
		virtual bool Init() override;
		// XVŠÖ”.
		virtual void Update() override;
		// •`‰æŠÖ”.
		virtual void Render() override;

	private:
		// ˆÚ“®ŠÖ”.
		virtual void Move() override;

		// ’e‚ğŒ‚‚Â(ˆê”­‚¸‚Â).
		virtual void BulletShot( const float& rot, const float& moveSpeed ) override;
		// ’e‚ğŒ‚‚Â(•¡”).
		void BulletShotAnyWay( 
			const float& rot, 
			const float& angle, 
			const float& moveSpeed,
			const int& bulletCount );

	private:
		int m_NowShotBulletCount;	// Œ»İ‚ÌŒ‚‚Á‚½’e‚Ì”.
	};
}

#endif	// #ifndef STG_ENEMY_H.