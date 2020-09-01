#ifndef FADE_H
#define FADE_H

#include "..\Widget.h"

/************************************
*	フェードクラス.
**/
class CFade : public CWidget
{

public:
	CFade();
	virtual ~CFade();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;
	// フェード中か取得関数.
	bool GetIsFade() const { return GetInstance()->m_IsFade; }
	// フェード状態取得関数.
	EFadeState GetFadeState() const { return GetInstance()->m_FadeState; }
	// フェードイン設定関数.
	void SetFadeIn();
	// フェードアウト設定関数.
	void SetFadeOut();

	// シングルトンの生成.
	static CFade* GetInstance()
	{
		static std::unique_ptr<CFade> pInstance = std::make_unique<CFade>();
		return pInstance.get();
	}

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	bool m_IsFade;		//フェード中か.

	//コピー・ムーブコンストラクタ、代入演算子の削除.
	CFade(const CFade &)						= delete;
	CFade(CFade &&)							= delete;
	CFade& operator = (const CFade &)	= delete;
	CFade& operator =(CFade &&)			= delete;

};

#endif	//#ifndef FADE_H.