#ifndef EVENT_ALIEN_H
#define EVENT_ALIEN_H

#include "..\EventCharacter.h"

/****************************************
*	イベント用宇宙人クラス.
**/
class CEventAlien : public CEventCharacter
{
public:
	// 宇宙人状態.
	enum class enEventAlienState
	{
		None,

		Spawn,		// スポーン.
		Move,		// 移動.
		Abduct,		// 拐う.
		Escape,		// 逃げる.
		BlowAway,	// 吹き飛ぶ.
		Wait,		// 待機.

		Max,

	} typedef EEventAlienState;

public:
	CEventAlien();
	virtual ~CEventAlien();

	// 相手座標の設定.
	virtual void SetTargetPos(CActor& actor) override;
	// バリアに当たっているか.
	bool IsBarrierHit() const { return m_IsBarrierHit; }
	// 宇宙人の状態設定.
	void SetAlienState(const EEventAlienState& state) { m_NowState = state; }


private:
	// スポーン.
	virtual bool Spawn(const D3DXVECTOR3& spawnPos) = 0;
	// モデルのアルファ値の取得.
	float GetModelAplha() const { return m_Parameter.ModelAlpha; }
	// 連れ去っているかどうか.
	bool IsAbduct() const { return m_NowState == EEventAlienState::Abduct; }
	// 連れ去るUFOの座標の取得.
	void SetAbductUFOPosition(D3DXVECTOR3* pos) { m_pAbductUFOPosition = pos; }

protected:
	// 現在の状態の更新関数.
	void CurrentStateUpdate();
	// 女の子の座標を設定.
	void SetGirlPos(CActor& actor);

	// スポーン中.
	virtual void Spawning();
	// 移動.
	virtual void Move();
	// 拐う.
	virtual void Abduct();
	// 逃げる.
	virtual void Escape();
	// 吹き飛ぶ.
	virtual void BlowAway();

	// 女の子との当たり判定.
	void GirlCollision(CActor* pActor);
	// バリアとの当たり判定.
	void BarrierCollision(CActor* pActor);

protected:
	D3DXVECTOR3*		m_pAbductUFOPosition;		// UFOの座標.
	EEventAlienState	m_NowState;					// 現在の状態.
	bool				m_IsBarrierHit;				// バリアに当たっているか.
	float				m_Speed;					// 移動速度,

};


#endif	//#ifndef EVENT_ALIEN_H.