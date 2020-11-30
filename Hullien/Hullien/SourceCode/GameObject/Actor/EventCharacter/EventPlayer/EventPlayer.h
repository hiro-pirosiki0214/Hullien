#ifndef EVENT_PLAYER_H
#define EVENT_PLAYER_H

#include "..\EventCharacter.h"
#include "..\..\Character\Player\PlayerParam.h"
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

	// アニメーションの調整フレーム.
	const double ANIM_ADJ_FRAME_Wait = 0.0;	// 待機.
	const double ANIM_ADJ_FRAME_Walk = 0.0;	// 走り.
	const double ANIM_ADJ_FRAME_Attack1 = 0.5;	// 攻撃1.
	const double ANIM_ADJ_FRAME_Attack2 = 0.5;	// 攻撃2.
	const double ANIM_ADJ_FRAME_Attack3 = 0.5;	// 攻撃3.
	const double ANIM_ADJ_FRAME_Avoid = 0.4;	// 回避.
	const double ANIM_ADJ_FRAME_SP = 0.02;		// 特殊能力.
	const double ANIM_ADJ_FRAME_Damage = 0.4;	// ヒット時.
	const double ANIM_ADJ_FRAME_Dead = 0.01;	// 死亡.

public:
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
	// アニメーション速度の設定.
	inline void SetAnimSpeed() { m_AnimSpeed = 0.01f; }

	// Yボタンが押されたか.
	inline bool IsYButtonPressed() { return m_IsYButtonPressed; }
	// 特殊能力.
	bool IsSpecialAbility();

	// 衝突したか.
	inline bool IsAlienHit(){ return m_IsAlienHit; }
	// 衝突.
	inline void SetAlienHit(){ m_IsAlienHit = true; }
	// ダメージアニメーションが終了したか.
	inline bool IsEndDamageAnim(){ return m_AnimFrameList[player::EAnimNo_Damage].IsNowFrameOver(); }
	// ダメージアニメーションの更新.
	void DamageAnimUpdate();
	// ボーン位置の取得.
	D3DXVECTOR3 GetBonePosition(const char* name);
private:
	// 特殊能力操作関数.
	void SPController();
	// ノックバック動作関数.
	void KnockBack();

	// 移動関数.
	virtual void Move() override;
	// エフェクト描画関数.
	void EffectRender();

	// 当たり判定の設定.
	bool ColliderSetting();
	// エフェクトの設定.
	bool EffectSetting();
	// アニメーションフレームの設定.
	bool SetAnimFrameList();

	// サウンドの設定.
	bool SoundSetting();
	// 音量の設定.
	void VolumeSetting(const char* soung, float volume);

private:
	anim::AAnimFrameList							m_AnimFrameList;	// アニメーションフレームのリスト.
	player::EAnimNo									m_NowAnimNo;		// 今のアニメーション番号.
	player::EAnimNo									m_OldAnimNo;		// 前のアニメーション番号.
	EPlayerState									m_State;			// 状態.
	std::vector<std::shared_ptr<CEffectManager>>	m_pEffects;			// エフェクト.
	float											m_SpecialAbility;	// 特殊能力.
	bool											m_IsYButtonPressed;	// Yボタンが押されたか.
	bool											m_HasUsableSP;		// 特殊能力を使えるか.
	bool											m_IsAlienHit;		// 宇宙人と当たったか.
	bool											m_IsAttackSE;		// 攻撃SEを鳴らすか.
};



#endif // #ifndef EVENT_PLAYER_H.
