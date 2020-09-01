#ifndef WIDGET_H
#define WIDGET_H

#include "..\GameObject.h"

/****************************************
*	UI元クラス.
**/
class CWidget : public CGameObject
{
public:
	// フェードの状態.
	enum class enFadeState
	{
		In,			//フェードイン.
		Out,		//フェードアウト.
		Finish,		//フェード終了.
	} typedef EFadeState;

protected:
	const float ALPHA_MAX = 1.0f;

public:
	CWidget();
	virtual ~CWidget();

	// フェードイン設定関数.
	void SetFadeIn(const float& alphaSpeed );
	// フェードアウト設定関数.
	void SetFadeOut( const float& alphaSpeed );
	//位置設定関数.
	void SetPosition( const D3DXVECTOR3& vPos ) { m_vPosition = vPos; }

protected:
	// スプライト設定関数.
	virtual bool SpriteSetting() = 0;
	// フェードイン.
	void FadeIn( float& alpha);
	// フェードアウト.
	void FadeOut( float& alpha );
	// フェード更新関数.
	void FadeUpdate( float& alpha );

protected:
	std::shared_ptr<CSprite>	m_pSprite;		//スプライトクラス.
	EFadeState						m_FadeState;	//フェード状態.
	float								m_Alpha;			//透過値.
	float								m_AlphaMax;	//透過最大値.
	float								m_AlphaSpeed;//透過値速度.

private:
	// 透過値設定関数.
	void SetAlpha(float& alpha, const float& alphaSpeed);

private:
	EFadeState		m_OldFadeState;		//過去のフェード状態.

};

#endif	// #ifndef WIDGET_H.