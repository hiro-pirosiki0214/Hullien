#ifndef STG_BULLET_H
#define STG_BULLET_H

#include "..\STGActor.h"

namespace STG
{
	class CBullet : public STG::CActor
	{
		const char* MODEL_NAME;	// ���f���� : �O������擾.
	public:
		CBullet();
		CBullet( const char* modelName );
		virtual ~CBullet();

		// �������֐�.
		virtual bool Init() override;
		// �X�V�֐�.
		virtual void Update() override;
		// �`��֐�.
		virtual void Render() override;
		// �����蔻��.
		virtual void Collision( STG::CActor* pActor ) override;
		// ����.
		virtual bool Shoot( 
			const D3DXVECTOR3& pos,
			const float& rot, 
			const float& moveSpeed );

		// �F�̐ݒ�.
		void SetColor( const D3DXVECTOR3& color ){ m_Color = { color.x, color.y, color.z, 1.0f }; }
		// ��������.
		bool IsShot() const { return m_IsActive; }
		// ���������e��������ݒ�.
		void SetCollDisappear(){ m_IsBulletToBulletCollJudge = true; }

	private:
		// �����蔻��̍쐬.
		bool CollisionInit();

	private:
		D3DXVECTOR4 m_Color;						// �e�̐F.
		float		m_MoveSpeed;					// �ړ����x.
		bool		m_IsBulletToBulletCollJudge;	// �e�ǂ����̓����蔻����s����.
	};
};

#endif	// #ifndef STG_BULLET_H