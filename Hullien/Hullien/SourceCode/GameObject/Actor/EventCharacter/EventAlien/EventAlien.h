#ifndef EVENT_ALIEN_H
#define EVENT_ALIEN_H

#include "..\EventCharacter.h"

class CArm;

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

	enum enAnimNo
	{
		EAnimNo_None = -1,

		EAnimNo_Move,
		EAnimNo_Arm,
		EAnimNo_Damage,
		EAnimNo_Dead,

		EAnimNo_Max,

		EAnimNo_Begin	= EAnimNo_Move,
		EAnimNo_End		= EAnimNo_Max,
	} typedef EAnimNo;

public:
	CEventAlien();
	virtual ~CEventAlien();

	// 相手座標の設定.
	virtual void SetTargetPos(CActor& actor) override;
	// バリアに当たっているか.
	inline bool IsBarrierHit() const { return m_IsBarrierHit; }
	// 宇宙人の状態設定.
	inline void SetAlienState(const EEventAlienState& state) { m_NowState = state; }
	// ターゲット位置取得関数.
	inline D3DXVECTOR3 GetTargetPosition() const { return m_vTargetPosition; }
	// ターゲットをつかんでいるか.
	bool IsGrab() const;


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

	// 女の子との当たり判定.
	void GirlCollision(CActor* pActor);
	// バリアとの当たり判定.
	void BarrierCollision(CActor* pActor);

protected:
	std::shared_ptr<CArm>	m_pArm;						// アームクラス.
	D3DXVECTOR3*			m_pAbductUFOPosition;		// UFOの座標.
	D3DXVECTOR3				m_vTargetPosition;			// ターゲット位置.
	EEventAlienState		m_NowState;					// 現在の状態.
	bool					m_IsBarrierHit;				// バリアに当たっているか.
	float					m_Speed;					// 移動速度,

};


#endif	//#ifndef EVENT_ALIEN_H.