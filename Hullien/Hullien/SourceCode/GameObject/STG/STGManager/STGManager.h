#ifndef STG_MANAGER_H
#define STG_MANAGER_H

namespace STG
{
	class CPlayer;	// プレイヤークラス.
}
class CCamera;

#include <memory>

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
	std::shared_ptr<CCamera>		m_pCamera;
	std::shared_ptr<STG::CPlayer>	m_pPlayer;	// プレイヤークラス.
};

#endif	// #ifndef STG_MANAGER_H.