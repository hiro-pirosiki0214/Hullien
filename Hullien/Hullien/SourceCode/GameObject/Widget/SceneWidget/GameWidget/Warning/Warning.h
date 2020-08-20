#ifndef WARNING_H
#define WARNING_H

#include "..\..\SceneWidget.h"

class CGirl;
class CCameraManager;

/*************************************
*	警告クラス.
**/
class CWarning : public CSceneWidget
{
private:
	const char* SPRITE_NAME = "dangersignsize";	//スプライト名.
	const float ADJ_POS_Y = 6.0f;							//y座標調整.

public:
	CWarning();
	virtual ~CWarning();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;
	// 女の子の状態設定関数.
	void SetGirlState(CGirl* pGirl);

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	D3DXVECTOR3 m_vTargetPos;	//ターゲット位置.
	D3DXVECTOR3 m_vTargetRot;	//ターゲット回転置.
	bool m_IsDisp;				//表示するか.
	bool m_IsUI;				//UIかそうでないか.

	std::shared_ptr<CCameraManager>		m_pCamera;			// カメラ.

};


#endif	//#ifndef WARNING_H.