#ifndef STG_ENEMY_MANAGER_H
#define STG_ENEMY_MANAGER_H

#include <memory>
#include <vector>
#include <functional>

namespace STG
{
	class CActor;			// �K��N���X.
	class CEnemy;			// �G�N���X.
	class CEnemyFactory;	// �G�쐬�N���X.
}

namespace STG
{
	class CEnemyManager
	{
	public:
		CEnemyManager();
		~CEnemyManager();

		// �������֐�.
		bool Init();
		// �X�V�֐�.
		void Update( std::function<void(STG::CActor*)> updateProc );
		// �`��֐�.
		void Render();

		// �Ō�̓G�����S������.
		bool IsEndEnmeyDead() const;

	private:
		std::unique_ptr<STG::CEnemyFactory>			m_pEnemyFactory;	// �G�쐬�N���X.
		std::vector<std::shared_ptr<STG::CEnemy>>	m_Enemys;			// �G���X�g.
		float										m_EndEnemySpawnTime;// �Ō�̓G�̏o������.
	};
};

#endif	// #ifndef STG_ENEMY_MANAGER_H.