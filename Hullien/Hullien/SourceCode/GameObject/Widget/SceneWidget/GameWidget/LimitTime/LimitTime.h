#ifndef LIMITTIME_H
#define LIMITTIME_H

#include "..\..\SceneWidget.h"
#include <vector>

/*****************************************
*	制限時間クラス.
**/
class CLimitTime : public CSceneWidget
{
private:
	const char* SPRITE_TIME				= "timeNumber";		//時間のスプライト名.
	const char* SPRITE_TIME_BACK		= "timeBack";		//時間の背景スプライト名.
	const char* SPRITE_TIME_BACK_MINI	= "timeBackMini";	//時間のミニ背景スプライト名.
	const D3DXVECTOR3 ONETIME_INITPOS	= D3DXVECTOR3(655.0f, 80.0f, 0.0f);	//時間一の位初期位置.
	const D3DXVECTOR3 TENTIME_INITPOS	= D3DXVECTOR3(625.0f, 80.0f, 0.0f);	//時間十の位初期位置.
	const D3DXVECTOR3 MINIBACK_INITPOS	= D3DXVECTOR3(640, 40.0f, 0.0f);	//ミミ背景の初期位置.
	const float MINIBACK_MAX_WIDTH		= 50.0f;
	const float MINIBACK_COUNT_ADD		= 0.005f;
	const int TIME_BACK_NUM				= 2;	// 背景の配列番号.
	const int LIMITTIME_MAX				= 60;	//制限時間最大.
	const int FRAME_DIVISION			= 100;//フレーム割る数.
	const int ANIM_MAX					= 10;	//アニメーション最大数.
	const int TEN						= 10;	//十.

public:
	CLimitTime();
	virtual ~CLimitTime();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// 終了の取得関数.
	bool IsFinish() const { return m_IsFinish; }	

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	// 位置設定関数.
	bool PositionSetting();
	// アニメーション番号設定関数.
	bool AnimSetting();
	// カウントダウン関数.
	void CountDown();
	// フレームから秒に変換関数.
	void FramesToSeconds();
	// ミニ背景の座標更新.
	void MiniBackPositionUpdate();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pTimeSprites;		// スプライトクラス.
	std::vector<std::shared_ptr<CSprite>>	m_pMiniBackSprites;		// スプライトクラス.
	std::vector<D3DXVECTOR3>				m_vTimePosition;	// 位置.
	std::vector<D3DXVECTOR3>				m_vMiniBackPosition;	// 位置.
	std::vector<float>						m_vMiniBackScale;	// 位置.
	std::vector<int>						m_Anim;			// アニメーション番号.
	int										m_FrameCount;	// フレームカウント.
	int										m_Seconds;		// 秒.
	bool									m_IsFinish;		// 終了したかどうか.
	float	m_RotationCount;
};

#endif	//#ifndef LIMITTIME_H.