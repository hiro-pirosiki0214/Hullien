#ifndef LIFEGAUGE_H
#define LIFEGAUGE_H

#include "..\ChatacterWidget.h"
#include <vector>

/*************************************
*	ライフゲージクラス.
**/
class CLifeGauge : public CCharacterWidget
{
private:
	const char* SPRITE_GAGEBACK = "gage";	//ゲージ背景.
	const char* SPRITE_GAGE		= "gage";	//ゲージ.		

	// ゲージの情報.
	struct stGaugeState
	{
		D3DXVECTOR3 vPosition;	//位置.
		D3DXVECTOR3	vScale;		//大きさ.
		int						AnimNum;	//アニメーション番号.

		stGaugeState()
			: vPosition	(D3DXVECTOR3(0.0f,0.0f,0.0f))
			, vScale		(D3DXVECTOR3(1.0f,1.0f,1.0f))
			, AnimNum (0)
		{}
	}typedef SGaugeState;
public:
	CLifeGauge();
	virtual ~CLifeGauge();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;
	// パラメータ設定関数.
	virtual void SetParameter(CCharacter& pChara) override;

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	//スプライトクラス.
	std::vector<SGaugeState> m_GaugeState;			//ゲージの情報.
};

#endif	//#ifndef LIFEGAUGE_H
