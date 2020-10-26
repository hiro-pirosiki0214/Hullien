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

	const float STG_END_FRAME = 30*60;

public:
	CSTGManager();
	~CSTGManager();

	// 初期化関数.
	bool Init();
	// 更新関数.
	void Update();
	// 描画関数.
	void Render();
	// シューティングが終了したか.
	bool IsSTGEnd();

private:
	std::shared_ptr<CCamera>			m_pCamera;
	std::shared_ptr<STG::CPlayer>		m_pPlayer;			// プレイヤークラス.
	std::unique_ptr<STG::CEnemyManager>	m_pEnemyManager;	// 敵管理クラス.
	float	m_STGEndFrame;	// STGの終了フレーム.
	float	m_STGNowFrame;	// STGの現在フレーム.
};

#endif	// #ifndef STG_MANAGER_H.