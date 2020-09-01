#ifndef CURSOR_H
#define  CURSOR_H

#include "..\Widget.h"

/********************************************
*	カーソルクラス.
**/
class CCursor : public CWidget
{
private:
	const char* SPRITE_NAME = "selectHoversize";	//スプライトのファイル名.
	const float SCALE_MAX	= 1.0f;						//拡大最大値.
	const float SCALE_SPEED = 0.05f;						//拡大速度.
	const float ACC_SPEED   = 0.0011f;					//加速度.

public:
	CCursor();
	 virtual ~CCursor();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	// スケール動き関数.
	void MoveScale();
	// 拡大関数.
	void IncreaseScale();

private:
	D3DXVECTOR3	m_vOldPosition;	//移動前の座標.
	float		m_Acceleration;	//加速値.
};

#endif	//#ifndef CURSOR_H.