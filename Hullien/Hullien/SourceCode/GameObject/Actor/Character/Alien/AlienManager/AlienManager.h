#ifndef ALIEN_MANAGER_H
#define ALIEN_MANAGER_H

#include "..\Alien.h"
#include "..\..\..\Explosion\Explosion.h"

#include <vector>
#include <memory>
#include <functional>

class CMotherShipUFO;	// �}�U�[�V�b�vUFO.
class CSpawnUFO;		// �X�|�[��UFO.
class CExplosion;		// �����N���X.

class CAlienManager
{
	const char* SPAWN_PARAM_FILE_PATH = "Data\\GameParam\\Alien\\SpaenParam\\SpaenParam.bin";
	const char* EXPLOSION_PARAM_FILE_PATH = "Data\\GameParam\\Alien\\Explosion\\Explosion.bin";
	const char* ALIEN_PARAM_LIST_FILE_PATH = "Data\\GameParam\\Alien\\AlienParamPathList.txt";
public:
	CAlienManager();
	~CAlienManager();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update( std::function<void(CActor*)> updateProc );
	// �`��֐�.
	void Render();
	// �G�t�F�N�g�`��֐�.
	void EffectRender();
	// �X�v���C�g�`��֐�.
	void SpriteRender();
	// �}�U�[�V�b�v�̍��W��ݒ�.
	void SetMotherShipUFOPos( const D3DXVECTOR3& vPos ){ m_MotherShipUFOPos = vPos; }
	// ���Ƃ��A�C�e�����X�g�̎擾.
	DropItemList GetDropItemList(){ return m_DropItemList; }
	// ���̎q��A�ꋎ���Ă��邩.
	bool IsGirlAbduct();
	// �A�j���[�V�������~�߂�.
	void StopAnimation();

private:
	// �X�|�[��.
	void Spawn();

	// �����ł��邩�ǂ����m�F.
	void ExplosionConfirming( CAlien* ailen );
	// ���Ƃ��A�C�e���̐ݒ�.
	void SetDropItemList( CAlien* ailen );
	// �X�|�[��UFO�̏�����.
	bool SpawnUFOInit();
	// �F���l�p�����[�^���X�g�̓ǂݍ���.
	bool ReadAlienParamList();
	// �����p�����[�^�̓ǂݍ���.
	bool ReadExplosionParam();

private:
	std::vector<std::shared_ptr<CAlien>>	m_AilenList;			// �F���l���X�g.
	std::vector<std::shared_ptr<CSpawnUFO>>	m_SpawnUFOList;			// �X�|�[��UFO���X�g.
	std::vector<SAlienParam>				m_AlienParamList;		// �F���l�p�����[�^���X�g.
	std::vector<CExplosion>					m_ExplosionList;		// �������X�g.
	CExplosion::SExplosionParam				m_ExplosionParam;		// �����p�����[�^�[.
	DropItemList							m_DropItemList;			// ���Ƃ��A�C�e���̃��X�g.
	D3DXVECTOR3								m_MotherShipUFOPos;		// �A�ꋎ��UFO�̍��W.
	bool									m_IsAlienAbduct;		// �F���l���A�ꋎ���Ă��邩�ǂ���.
	int										m_SortCount;			// �\�[�g���鎞�̃J�E���g.
	bool									m_IsRisingMotherShip;	// �}�U�[�V�b�v�ɏ����Ă��邩.
};

#endif	// #ifdef ALIEN_MANAGER_H.