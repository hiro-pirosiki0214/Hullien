#ifndef WARNING_H
#define WARNING_H

#include "..\..\SceneWidget.h"
#include <vector>

/*************************************
*	警告クラス.
**/
class CWarning : public CSceneWidget
{
private:
	const char* SPRITE_NAME			= "dangersignsize";	// スプライト名.
	const char* SPRITEUI_NAME		= "arrowsize";			// スプライトUI名.
	const int	DANGER_SIGN			= 0;						// 警告サイン.
	const int	DANGER_ARROW		= 1;					    // 警告矢印.
	const float ADJ_POS_Y					= 6.0f;					// y座標調整.

	// 警告矢印.
	const float RIGHT_POSITION_X		= WND_W - 90.0f;		//右表示のy座標.
	const float LEFT_POSITION_X		= 70.0f;					//左表示のy座標.
	const float ARROWPOSITION_Y		= WND_H / 2;			//矢印のy座標.
	const float RIGHT_ROTATION		= 0.0f;					//右表示の回転値.
	const float LEFT_ROTATION			= static_cast<float>(D3DXToDegree(180.0));	//左表示の回転値.

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
	virtual void SetPosition(const D3DXVECTOR3& vPos)override { m_vTargetPos = vPos; }
	// 女の子が画面外か.
	void SetIsGirlOffScreen(const bool& IsOffScreen) { m_IsGirlOffScreen = IsOffScreen; }
	// 矢印を右に表示するか.
	void SetIsDispRight(const bool& IsDispRight) { m_IsDispRight = IsDispRight; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	// スプライトの描画関数.
	void RenderDangerSign();
	// スプライトUIの描画関数.
	void RenderDangerArrow();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;					// スプライトクラス.
	D3DXVECTOR3								m_vTargetPos;				//ターゲット位置.
	bool												m_IsGirlOffScreen;		//女の子が画面外か.
	bool												m_IsDispRight;				//右に表示するか.
};


#endif	//#ifndef WARNING_H.