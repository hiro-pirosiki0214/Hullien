#ifndef EVENT_PLAYER_H
#define EVENT_PLAYER_H

#include "..\EventCharacter.h"
#include <vector>

class CEffectManager;

/********************************
*	イベント用プレイヤークラス.
**/
class CEventPlayer : public CEventCharacter
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME = "kaito_s";	// モデル名.
	const char* MODEL_TEMP_NAME = "kaito";	// 仮モデル名.

	// 足のボーン名.
	const char* RIGHT_FOOT = "kaito_rifa_2_R_asi_4";
	const char* LEFT_FOOT = "kaito_rifa_2_L_asi_4";

	// 声の音量.
	const float VOICE_VOLUME = 1.5f;

public:
	// アニメーション番号.
	enum class enAnimNo
	{
		None = -1,

		Wait,		// 待機.
		Walk,		// 走り.
		Attack1,	// 攻撃1.
		Attack2,	// 攻撃2.
		Attack3,	// 攻撃3.

		Max = Attack3,
	} typedef EAnimNo;

	enum class enPlayerState
	{
		None = 0,

		Move,			//移動.
		Wait,			//待機.
		SpecialAbility,	//特殊能力.
	} typedef EPlayerState;

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
	// アニメーション設定.
	void SetAnimation(const EAnimNo& animNo);

	// 特殊能力を使っているか.
	bool IsSpecialAbility();

private:
	// 特殊能力操作関数.
	void SPController();
	// 移動関数.
	virtual void Move() override;
	// エフェクト描画関数.
	void EffectRender();
	// 当たり判定の設定.
	bool ColliderSetting();
	// エフェクトの設定.
	bool EffectSetting();
	// サウンドの設定.
	bool SoundSetting();
	// 音量の設定.
	void VolumeSetting(const char* soung, float volume);

private:
	std::shared_ptr<CCollisionManager>	m_pAttackCollManager;	// 攻撃用の当たり判定.
	EAnimNo										m_NowAnimNo;			// 今のアニメーション番号.
	EAnimNo										m_OldAnimNo;				// 前のアニメーション番号.
	D3DXVECTOR3								m_AttackPosition;
	EPlayerState									m_State;						// 状態.
	std::vector<std::shared_ptr<CEffectManager>> m_pEffects;		// エフェクト.
	float			m_SpecialAbility;			// 特殊能力.
	bool			m_HasUsableSP;			// 特殊能力を使えるか.
	bool			m_IsAttackSE;				// 攻撃SEを鳴らすか.

};



#endif // #ifndef EVENT_PLAYER_H.
