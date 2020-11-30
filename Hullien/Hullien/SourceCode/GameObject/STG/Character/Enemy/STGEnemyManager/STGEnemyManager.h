#ifndef STG_ENEMY_MANAGER_H
#define STG_ENEMY_MANAGER_H

#include <memory>
#include <vector>
#include <functional>

namespace STG
{
	class CActor;			// 規定クラス.
	class CEnemy;			// 敵クラス.
	class CEnemyFactory;	// 敵作成クラス.
}

namespace STG
{
	class CEnemyManager
	{
	public:
		CEnemyManager();
		~CEnemyManager();

		// 初期化関数.
		bool Init();
		// 更新関数.
		void Update( std::function<void(STG::CActor*)> updateProc );
		// 描画関数.
		void Render();

		// 最後の敵が死亡したか.
		bool IsEndEnmeyDead() const;

	private:
		std::unique_ptr<STG::CEnemyFactory>			m_pEnemyFactory;	// 敵作成クラス.
		std::vector<std::shared_ptr<STG::CEnemy>>	m_Enemys;			// 敵リスト.
		float										m_EndEnemySpawnTime;// 最後の敵の出現時間.
	};
};

#endif	// #ifndef STG_ENEMY_MANAGER_H.