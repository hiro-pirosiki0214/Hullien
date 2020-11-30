#ifndef STG_PLAYER_H
#define STG_PLAYER_H

#include "..\STGCharacter.h"

namespace STG
{
	class CPlayer : public STG::CCharacter
	{
		const char* MODEL_NAME			= "ufo_mini";
		const char* BULLET_MODEL_NAME	= "item cure";
		const char* SHOT_SE_NAME		= "STGshot";
		const char* HIT_SE_NAME			= "STGHit";
		const float LIFE_POINT_MAX		= 5.0f;						// ���C�t�̍ő吔.
		const float MOVE_SPEED			= 0.4f;						// �ړ����x.
		const float BULLET_MOVE_SPEED	= 0.8f;						// �e�̑��x.
		const int	BULLET_COUNT_MAX	= 30;						// �e�̍ő吔.
		const int	SHOT_INTERVAL_FRAME	= 15;						// �e�����Ԋu�t���[��.
		const D3DXVECTOR3 BULLET_COLOR	= { 1.0f, 1.0f, 1.0f };		// �e�̐F.
		const D3DXVECTOR3 INIT_POSITION = { 0.0f, 0.0f, 70.0f };	// �������W.

		const float SPAWN_END_POS_Z		= 40.0f;	// �X�|�[���̏I�����W.
		const float SPAWN_SPEED_SUB		= 0.005f;	// �X�|�[���̈ړ����x�̌��Z�l.

		const float OUT_POSITION_X		= 100.0f;	// ��ʊO�̍��W : X.
		const float OUT_POSITION_Z		= 55.0f;	// ��ʊO�̍��W : Z.
		const float DEAD_SPEED			= 0.02f;	// ���S���x.
	public:
		CPlayer();
		virtual ~CPlayer();

		// �������֐�.
		virtual bool Init() override;
		// �X�V�֐�.
		virtual void Update() override;
		// �`��֐�.
		virtual void Render() override;
		// �����蔻��.
		virtual void Collision( STG::CActor* pActor ) override;
		// ����֐�.
		void Controller();

	private:
		// �X�|�[���ړ�.
		void SpawnMove();
		// �ړ��֐�.
		virtual void Move() override;
		// ���S�㏈��.
		void DeadUpdate();
		// �e��������֐�.
		void ShotController();

		// ���C�t�v�Z�֐�.
		virtual void LifeCalculation( const std::function<void(float&)>& ) override;

		// ��ʊO�ɍs�������̏���.
		void OutDispMove();

		// �����蔻��̍쐬.
		bool CollisionInit();
		
	private:
		D3DXVECTOR3 m_Direction;		// ����.
		float		m_SpawnMoveSpeed;	// �X�|�[�����̈ړ����x.
		bool		m_IsDead;			// ���S������.
	};
};

#endif	// #ifndef STG_PLAYER_H.