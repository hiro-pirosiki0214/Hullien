#ifndef EVENT_PLAYER_H
#define EVENT_PLAYER_H

#include "..\Character.h"

class CEffectManager;

/********************************
*	イベント用プレイヤークラス.
**/
class CEventPlayer : public CCharacter
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME = "Kaito_s";	// モデル名.
	const char* MODEL_TEMP_NAME = "kaito";	// 仮モデル名.

	// アニメーション番号.
	enum class enAnimNo
	{
		None = -1,

		Dead = 0,
		Wait,
		Walk,
		Wait1,
		Happy,

		Max = Happy,
	} typedef EAnimNo;

public:
	CEventPlayer();
	virtual ~CEventPlayer();

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
	// スプライトの描画.
	virtual void SpriteRender() override;
	// アニメーション設定.
	void SetAnimation(const EAnimNo& animNo);

	// 特殊能力を使っているか.
	bool IsSpecialAbility();

private:
	// 攻撃操作関数.
	void AttackController();
	// 特殊能力操作関数.
	void SPController();
	// 移動関数.
	virtual void Move() override;
	// エフェクト描画関数.
	void EffectRender();
	// 攻撃の当たり判定.
	void AttackCollision(CActor* pActor);
	// 当たり判定の設定.
	bool ColliderSetting();
	// エフェクトの設定.
	bool EffectSetting();

private:
	std::shared_ptr<CCollisionManager>	m_pAttackCollManager;	// 攻撃用の当たり判定.
	EAnimNo	m_NowAnimNo;			// 今のアニメーション番号.
	EAnimNo	m_OldAnimNo;			// 前のアニメーション番号.
	D3DXVECTOR3		m_AttackPosition;
	std::vector<std::shared_ptr<CEffectManager>> m_pEffects;	// エフェクト.
	float			m_SpecialAbility;		// 特殊能力.
	bool			m_HasUsableSP;			// 特殊能力を使えるか.
	bool			m_IsAttackSE;				//攻撃SEを鳴らすか.

};



#endif // #ifndef EVENT_PLAYER_H.
