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
	const char* SPRITE_TIME				= "timeNumber";	//時間のスプライト名.
	const D3DXVECTOR3 ONETIME_INITPOS	= D3DXVECTOR3(660.0f, 30.0f, 0.0f);	//時間一の位初期位置.
	const D3DXVECTOR3 TENTIME_INITPOS	= D3DXVECTOR3(630.0f, 30.0f, 0.0f);	//時間十の位初期位置.
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

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;		// スプライトクラス.
	std::vector<D3DXVECTOR3>				m_vPosition;	// 位置.
	std::vector<int>						m_Anim;			// アニメーション番号.
	int										m_FrameCount;	// フレームカウント.
	int										m_Seconds;		// 秒.
	bool									m_IsFinish;		// 終了したかどうか.
};

#endif	//#ifndef LIMITTIME_H.