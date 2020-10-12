#ifndef FADE_H
#define FADE_H

#include "..\Widget.h"

/************************************
*	フェードクラス.
* (継承を解除し、各関数をstaticにして外部でも呼び出せるようにする).
**/
class CFade
{
	const float ALPHA_MAX = 1.0f;

public:
	// フェードの状態.
	enum class enFadeState
	{
		In,			//フェードイン.
		Out,		//フェードアウト.
		Finish,		//フェード終了.
	} typedef EFadeState;

public:
	CFade();
	virtual ~CFade();

	// インスタンスの生成.
	static CFade* GetInstance()
	{
		static std::unique_ptr<CFade> pInstance = std::make_unique<CFade>();
		return pInstance.get();
	}

	// 初期化関数.
	static bool Init();
	//描画関数.
	static void Render();
	// フェード中か.
	static bool GetIsFade() { return GetInstance()->m_IsFade; }
	// フェード状態取得関数.
	static EFadeState GetFadeState() { return GetInstance()->m_FadeState; }
	// フェードイン設定関数.
	static void SetFadeIn();
	// フェードアウト設定関数.
	static void SetFadeOut();

private:
	// 更新関数.
	void Update();
	// スプライト設定関数.
	bool SpriteSetting();

private:
	std::shared_ptr<CSprite> m_pSprite;			//スプライトクラス.
	EFadeState m_FadeState;						//フェードの状態.
	float m_Alpha;								//透過値.
	float m_AlphaSpeed;							//透過速度.
	bool m_IsFade;								//フェード中か.

	//コピー・ムーブコンストラクタ、代入演算子の削除.
	CFade(const CFade &)				= delete;
	CFade(CFade &&)						= delete;
	CFade& operator = (const CFade &)	= delete;
	CFade& operator =(CFade &&)			= delete;

};

#endif	//#ifndef FADE_H.