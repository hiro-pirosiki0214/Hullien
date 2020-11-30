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

		// �e�̎擾.
		virtual std::vector<std::shared_ptr<CBullet>> GetBullets(){ return m_pBullets; }

	protected:
		// �ړ��֐�.
		virtual void Move() = 0;
		// �e�̏�����.
		virtual bool BulletInit( 
			std::vector<std::shared_ptr<CBullet>>& bullets, 
			const int& bulletCount,
			const char* modelName );
		// �e�̍X�V.
		virtual void BulletUpdate();
		// �e�̕`��.
		virtual void BulletRender( const D3DXVECTOR3& color = { 0.0f, 0.0f, 0.0f } );
		// �e������.
		virtual bool BulletShot( const float& rot, const float& moveSpeed );

	protected:
		std::vector<std::shared_ptr<CBullet>> m_pBullets;
		int			m_ShotCount;	// �e�̌�������.
		float		m_LifePoint;	// �̗�.
	};

};

#endif	// #ifndef STG_CHARACTER_H.