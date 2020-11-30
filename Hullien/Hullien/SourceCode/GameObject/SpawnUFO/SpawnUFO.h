#ifndef SPAWN_UFO_H
#define SPAWN_UFO_H

#include "..\GameObject.h"
#include "..\Actor\Character\Alien\AlienList.h"
#include "..\Actor\Character\Alien\Alien.h"

#include "SpawnUFOParam.h"

#include <vector>
#include <random>

class CActor;
class CUFOLight;
class CCollisionManager;	// �����蔻��N���X.
class CUltemateSing;

class CSpawnUFO : public CGameObject
{
	const char* MODEL_NAME = "ufo_mini";	// ���f����.
	const float COLLISION_RADIUS = 5.0f;
	const float ADD_POS_POWER = 0.1f;
public:
	CSpawnUFO();
	virtual ~CSpawnUFO();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �X�v���C�g�̕`��.
	void SpriteRender();

	// �F���l���X�|�[��������.
	void SpawnAlien( std::vector<std::shared_ptr<CAlien>>& );
	// �F���l�̃p�����[�^���X�g��ݒ肷��.
	void SetAlienParameterList( std::vector<SAlienParam>* );
	// �X�|�[���p�����[�^�̐ݒ�.
	void SetSpawnParameter( const SSpawnUFOParam& param );
	// �A�ꋎ��UFO�̍��W�ݒ�.
	inline void SetAbductUFOPosition( D3DXVECTOR3* pPos ){ m_pAbductUFOPosition = pPos; }

	// ���C�g�����o��.
	void LightDischarge();
	// ���C�g�����܂�.
	void LightCleanUP();
	// �������S�ɕ��o����.
	void DischargePreparation();
	// ����ЂÂ���.
	void CleanUPPreparation();

	// �`��t���O�ݒ�֐�.
	inline void SetDisp(const bool& disp) { m_IsDisp = disp; }

	// �X�P�[���l�̐ݒ�.
	inline void SetScale(const D3DXVECTOR3& scale) { m_vScale = scale; }
	// �����蔻��(�C�x���g�Ŏg�p).
	D3DXVECTOR3 Collision(CActor* pActor);

private:
	// �F���l�̍쐬.
	std::shared_ptr<CAlien> AlienFactory();
	// �F���l�ԍ��̎擾.
	int GetAlienNo();
	// �F���l�ԍ��̍쐬.
	int CreateAlienNo( const int& min, const int& max, const int& outVal );
	// �A�C�e�����擾����.
	EItemList ProbabilityGetItem( const bool& isAlienD );

	// ���f���̎擾.
	bool GetModel();
	// �����蔻��̐ݒ�.
	bool CollisionSetting();

private:
	std::shared_ptr<CDX9StaticMesh>		m_pStaticMesh;			// ���b�V��.
	std::shared_ptr<CUFOLight>			m_pUFOLight;			// UFO���C�g.
	std::unique_ptr<CUltemateSing>		m_pUltemateSing;		// �F���lD�̌x��UI.
	std::vector<std::shared_ptr<CAlien>>m_AilenList;			// �F���l���X�g.
	std::shared_ptr<CCollisionManager>	m_pCollManager;			// �����蔻��N���X.
	SSpawnUFOParam						m_SpawnParameter;		// �X�|�[���p�����[�^.
	D3DXVECTOR3							m_SpawnPoint;			// �X�|�[���|�C���g.
	D3DXVECTOR3*						m_pAbductUFOPosition;	// �A�ꋎ��UFO�̍��W.
	std::vector<SAlienParam>*			m_pAlienParamList;		// �F���l�p�����[�^���X�g.
	int	m_FrameCount;	// �o�߃t���[���J�E���g.
	int m_SpawnCount;	// �X�|�[���J�E���g.
	int m_AlienIndex;	// �F���l.
	int	m_AlienSpawnCount;
	bool m_IsDisp;		//�`��t���O.

	std::mt19937 m_RandomSeed;	// �����_���V�[�h.
};

#endif	// #ifndef SPAWN_UFO_H.