#ifndef WIDGET_H
#define WIDGET_H

#include "..\GameObject.h"

/****************************************
*	UI元クラス.
**/
class CWidget : public CGameObject
{
protected:
	const float ALPHA_MAX = 1.0f;

	// フェードの状態.
	enum class enFadeState
	{
		In,			//フェードイン.
		Out,		//フェードアウト.
		Finish,		//フェード終了.
	} typedef EFadeState;

public:
	CWidget();
	virtual ~CWidget();

	// フェードイン設定関数.
	void SetFadeIn();
	// フェードアウト設定関数.
	void SetFadeOut();
	//位置設定関数.
	void SetPosition(const D3DXVECTOR3& vPos) { m_vPosition = vPos; }

protected:
	// スプライト設定関数.
	virtual bool SpriteSetting() = 0;
	// フェードイン.
	void FadeIn(float& alpha, const float& alphaSpeed);
	// フェードアウト.
	void FadeOut(float& alpha, const float& alphaSpeed);

protected:
	std::shared_ptr<CSprite>	m_pSprite;		//スプライトクラス.
	EFadeState					m_FadeState;	//フェード状態.
	float						m_Alpha;		//透過値.
	float						m_AlphaMax;		//透過最大値.

private:
	// 透過値設定関数.
	void SetAlpha(float& alpha, const float& alphaSpeed);

private:
	EFadeState		m_OldFadeState;		//過去のフェード状態.

};

#endif	// #ifndef WIDGET_H.