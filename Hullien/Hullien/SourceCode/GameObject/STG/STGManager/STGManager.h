#ifndef STG_MANAGER_H
#define STG_MANAGER_H

#include <memory>

namespace STG
{
	class CPlayer;			// プレイヤークラス.
	class CEnemyManager;	// 敵管理クラス.
}
class CCamera;

class CSTGManager
{
public:
	CSTGManager();
	~CSTGManager();

	// 初期化関数.
	bool Init();
	// 更新関数.
	void Update();
	// 描画関数.
	void Render();

private:
	std::shared_ptr<CCamera>			m_pCamera;
	std::shared_ptr<STG::CPlayer>		m_pPlayer;			// プレイヤークラス.
	std::unique_ptr<STG::CEnemyManager>	m_pEnemyManager;	// 敵管理クラス.
};

#endif	// #ifndef STG_MANAGER_H.