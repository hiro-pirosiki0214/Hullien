#ifndef CONFIG_H
#define CONFIG_H

#include "..\..\SceneBase\SceneBase.h"

class CConfigWidget;	// 設定UIクラス.

/**********************************
*	設定シーンクラス.
*/
class CConfig : public CSceneBase
{
public:
	CConfig( CSceneManager* pSceneManager );
	virtual ~CConfig();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	// シーン切り替え関数.
	void ChangeScene();

private:
	std::unique_ptr<CConfigWidget>	m_pConfigWidget;	// 設定UIクラス.
	bool	m_IsChangeScene;
};

#endif	// #ifndef CONFIG_H.