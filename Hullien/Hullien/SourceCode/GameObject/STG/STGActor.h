#ifndef STG_ACTOR_H
#define STG_ACTOR_H

#include "..\GameObject.h"
#include <vector>
#include <functional>

class CCollisionManager;	// �����蔻��N���X.

namespace STG
{
	class CBullet;	// �e�N���X.
	class CActor : public CGameObject
	{
	public:
		CActor();
		virtual ~CActor();

		// ���b�V���̕`��.
		virtual void MeshRender();
		// �����蔻��.
		virtual void Collision( STG::CActor* pActor ) = 0;
		// ���C�t�v�Z�֐�.
		virtual void LifeCalculation( const std::function<void(float&)>& ){}

		// �����蔻��̎擾.
		CCollisionManager* GetColl(){ return m_pCollManager.get(); }

		// �ړI�̍��W�̐ݒ�.
		void SetTargetPos( const D3DXVECTOR3& pos ){ m_TargetPositon = pos; }

		// ���삷�邩�ǂ����ݒ�.
		void SetActive( const float& isActive ){ m_IsActive = isActive; }
		// ���삵�Ă��邩�擾.
		bool GetActive() const { return m_IsActive; }

		// �e�̎擾.
		virtual std::vector<std::shared_ptr<CBullet>> GetBullets(){ return std::vector<std::shared_ptr<CBullet>>(); }

	protected:
		std::shared_ptr<CDX9StaticMesh>		m_pStaticMesh;	// �X�^�e�B�b�N���b�V��.
		std::shared_ptr<CCollisionManager>	m_pCollManager;	// �����蔻��N���X.
		D3DXVECTOR3 m_MoveVector;		// �ړ��p�x�N�g��.
		D3DXVECTOR3 m_TargetPositon;	// �ړI�̍��W.
		bool		m_IsActive;			// ���삵�Ă��邩�ǂ���.
	};
};

#endif	// #ifndef STG_ACTOR_H.