#ifndef EVENT_GIRL_H
#define EVENT_GIRL_H

#include "..\EventCharacter.h"
#include "..\..\Character\Girl\GirlParam.h"

/****************************************
*	イベント用女の子クラス.
**/
class CEventGirl : public CEventCharacter
{
	const char* MODEL_NAME = "yuri-dy_s";	// モデル名.

	// パラメータ.
	struct stGirlParam : public SCharacterParam
	{
		float		SearchCollRadius;	// 索敵用のスフィアの半径.

		stGirlParam()
			: SearchCollRadius(10.0f)
		{}
	} typedef SGirlParam;

public:
	// 現在の状態.
	enum class enNowState
	{
		None,

		Protected,	// 守られている.
		Abduct,		// 連れ去れている.
		Move,			// 移動.
		Wait,			// 待機.

		Max,

	} typedef ENowState;

public:
	CEventGirl();
	virtual ~CEventGirl();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision(CActor* pActor) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos(CActor& actor) override;
	// 女の子が危険な状態か.
	inline bool GetIsDanger() const { return m_IsDanger; }
	// 情報設定関数.
	virtual void SetOptionalState(const SOptionalState& state) override;

	// 女の子の状態設定関数.
	inline void SetNowState(const ENowState& state) { m_NowState = state; }

private:
	// 移動関数.
	virtual void Move() override;

	// 索敵の当たり判定.
	void SearchCollision(CActor* pActor);

	// 当たり判定の作成.
	bool  ColliderSetting();

private:
	SGirlParam m_Parameter;	// 女の子のパラメーター.
	std::shared_ptr<CCollisionManager>	m_pSearchCollManager;	// 索敵用の当たり判定.
	D3DXVECTOR3	m_OldPosition;			// 前回の座標.
	ENowState	m_NowState;				// 現在の状態.
	bool		m_IsDanger;				// 危険かどうか.

};

#endif //#ifndef EVENT_GIRL_H.
