#ifndef ALIEN_MANAGER_H
#define ALIEN_MANAGER_H

#include "..\Alien.h"

#include <vector>
#include <memory>
#include <functional>

class CSpawnUFO;

class CAlienManager
{
	const char* SPAWN_PARAM_FILE_PATH = "Data\\GamePram\\Alien\\SpaenParam\\SpaenParam.bin";
	const char* ALIEN_PARAM_LIST_FILE_PATH = "Data\\GamePram\\Alien\\AlienParamPathList.txt";
public:
	CAlienManager();
	~CAlienManager();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update( CActor* pActor, std::function<void(CActor*)> collProc );
	// �`��֐�.
	void Render();

private:
	// �X�|�[��.
	void Spawn();

	// �X�|�[��UFO�̏�����.
	bool SpawnUFOInit();
	// �F���l�p�����[�^���X�g�̓ǂݍ���.
	bool ReadAlienParamList();
	// �F���l�̃��f���̒l�ł̃\�[�g�֐�.
	void ModelAlphaSort();

	// �f�o�b�O�p�̕`��֐�.
	void DebugRender();

private:
	std::vector<std::shared_ptr<CAlien>> m_AilenList;	// �F���l���X�g.
	std::vector<CSpawnUFO> m_SpawnUFOList;				// �X�|�[��UFO���X�g.
	std::vector<CAlien::SAlienParam> m_AlienParamList;	// �F���l�p�����[�^���X�g.
	D3DXVECTOR3	m_AbductUFOPosition;	// �A�ꋎ��UFO�̍��W.
	bool		m_IsAlienAbduct;		// �F���l���A�ꋎ���Ă��邩�ǂ���.
	int			m_SortCount;			// �\�[�g���鎞�̃J�E���g.
};

#endif	// #ifdef ALIEN_MANAGER_H.