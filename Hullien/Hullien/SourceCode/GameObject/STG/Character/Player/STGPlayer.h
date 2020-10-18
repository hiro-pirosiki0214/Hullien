#ifndef STG_PLAYER_H
#define STG_PLAYER_H

#include "..\STGCharacter.h"

namespace STG
{
	class CPlayer : public STG::CCharacter
	{
		const char* MODEL_NAME = "a";
		const char* BULLET_MODEL_NAME = "item cure";
		const float MOVE_SPEED = 0.2f;
		const float BULLET_MOVE_SPEED	= 0.5f;	// ’e‚Ì‘¬“x.
		const int	BULLET_COUNT_MAX	= 30;	// ’e‚ÌÅ‘å”.
		const int	SHOT_INTERVAL_FRAME	= 15;	// ’e‚ğŒ‚‚ÂŠÔŠuƒtƒŒ[ƒ€.
	public:
		CPlayer();
		virtual ~CPlayer();

		// ‰Šú‰»ŠÖ”.
		virtual bool Init() override;
		// XVŠÖ”.
		virtual void Update() override;
		// •`‰æŠÖ”.
		virtual void Render() override;
		// ‘€ìŠÖ”.
		void Controller();

	private:
		// ˆÚ“®ŠÖ”.
		virtual void Move() override;
		// ’e‚ğŒ‚‚Â‘€ìŠÖ”.
		void ShotController();

		
	private:
		D3DXVECTOR3 m_Direction;
	};
};

#endif	// #ifndef STG_PLAYER_H.