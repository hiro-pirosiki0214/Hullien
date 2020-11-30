#ifndef ACTOR_H
#define ACTOR_H

#include "..\GameObject.h"
#include <functional>

class CCollisionManager;			// �����蔻��N���X.
struct stBoxWall typedef SBoxWall;	// �����Ȃ��ǂ̍\����.

class CActor : public CGameObject
{
public:
	CActor();
	virtual ~CActor();

	// �G�t�F�N�g�̕`��.
	virtual void EffectRender(){}
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) = 0;

	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ){}
	// ����\�͉񕜎��ԁA���͎��Ԑݒ�֐�.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ){}
	// �U���́A���͎��Ԑݒ�֐�.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ){}
	// �ړ����x�A���͎��Ԑݒ�֐�.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ){}
	// ��Ⴢ̐ݒ�.
	virtual void SetParalysisTime( const std::function<void(float&)>& ){}
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& actor ){}
	// �x�N�g���̎擾.
	virtual void SetVector( const D3DXVECTOR3& vec ){};
	// �A�j���[�V�������~�߂�.
	virtual void StopAnimation(){};

	// �����Ȃ��ǂ̐ݒ�.
	inline void SetBoxWall( SBoxWall* box ){ m_pBoxWall = box; }

	// �����蔻��擾�֐�.
	inline CCollisionManager* GetCollManager(){ return m_pCollManager.get(); }

protected:
	// �ǂɏՓ˂�����.
	bool IsCrashedWall();
	// �ǂɏՓ˂����� : X.
	bool IsCrashedWallX();
	// �ǂɏՓ˂����� : Z.
	bool IsCrashedWallZ();

protected:
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// �����蔻��N���X.
	SBoxWall*							m_pBoxWall;		// �����Ȃ��� : �O������|�C���^�Ŏ擾.
};

#endif	// #ifndef ACTOR_H.