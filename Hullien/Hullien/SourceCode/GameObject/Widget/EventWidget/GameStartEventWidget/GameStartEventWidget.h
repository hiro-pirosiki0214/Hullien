#ifndef GAMESTART_EVENT_WIDGET_H
#define GAMESTART_EVENT_WIDGET_H

#include "..\EventWidget.h"
#include <vector>

/*****************************************
*	ゲームスタートシーンのUIクラス.
**/
class CGameStartEventWidget : public CEventWidget
{
	const char* SPRITE_BUTTON_NAME			= "buttonY";	//スプライトのファイル名.
	const char* SPRITE_PUSH_NAME			= "push";	//スプライトのファイル名.
	const char* SPRITE_PRESERVE_GIRL_NAME	= "PreserveGirl";

	const int	PUSH_YBUTTON	= 0;		// Yボタン指示の配列番号.
	const int	PRESERVE_GIRL	= 2;		// 女の子を守る指示の配列番号.
	const float ALPHA_SPEED		= 0.05f;	// 透過速度.
	const float WAITCOUNT_MAX	= 100.0f;	// 待機カウント最大.

public:
	// UIの情報.
	enum class enWidgetState
	{
		None = 0,		// なし.

		Push_YButton,	// Yボタンを押せ.
		Preserve_Girl,	// 女の子を守れ.

		Max,
	}typedef EWidgetState;

public:
	CGameStartEventWidget();
	virtual ~CGameStartEventWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// UIの状態設定関数.
	void SetWidgetState(const EWidgetState& state);
	// 描画が終了したか.
	inline bool IsDispEnd() { return m_IsDispEnd == true; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;			// スプライトクラス.
	std::vector<bool>						m_IsDisp;			// 描画フラグ.
	D3DXVECTOR3								m_ButtonPos;
	EWidgetState							m_WidgetState;		// UIの情報.
	float									m_Alpha;			// 透過値.
	float									m_WaitCount;		// 待機カウント.
	bool									m_IsDispEnd;		// 描画終了.
};

#endif	//#ifndef GAMESTART_EVENT_WIDGET_H.