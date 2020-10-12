#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"
#include "PlayerParam.h"
#include <queue>

class CRotLookAtCenter;
class CCharacterWidget;
class CEffectManager;
class CEffectTimer;

class CPlayer : public CCharacter
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME			= "kaito_s";	// モデル名.
	const char* MODEL_TEMP_NAME		= "kaito";		// 仮モデル名.

	// 使用攻撃エフェクト名.
	const char* ATTACK_ONE_EFFECT_NAME		= "slash_effect";
	const char* ATTACK_TWO_EFFECT_NAME		= "slash_effect";
	const char* ATTACK_THREE_EFFECT_NAME	= "slash_effect";
	// 特殊能力エフェクト名.
	const char* SPECIAL_ABILITY_EFFECT_NAME	= "uvtest";
	// 回避エフェクト名.
	const char* AVOIDANCE_EFFECT_NAME		= "uvtest";

	// 足のボーン名.
	const char* RIGHT_FOOT = "kaito_rifa_2_R_asi_4";
	const char* LEFT_FOOT = "kaito_rifa_2_L_asi_4";

	// 声の音量.
	const float VOICE_VOLUME = 1.5f;

	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(7.0));
	const float ROTATIONAL_SPEED = 0.2f;	// 回転速度.

	const float MOVE_SPEED_MUL_VALUE_ADD	= 0.035f;	// 掛け合わせる移動量の加算値.
	const float MOVE_SPEED_MUL_VALUE_MAX	= 1.0f;		// 掛け合わせる移動量の最大値.

	const float TWO = 2.0f;
	const float CAMERA_COUNT_MAX			= 100.0f;
	const float CAMERA_BACK_DIRECTION_X		= 20.0f;
	const float CAMERA_BACK_DIRECTION_Y		= 10.0f;
	const float CAMERA_BACK_HEIGHT			= 8.0f;
	const float CAMERA_BACK_LERP_VALUE		= 0.1f;
	const float CAMERA_FREQUENCY_LOOKPOS	= 15.0f;	// カメラ注視位置の周波数.
	const float CAMERA_AMPLITUDE_LOOKPOS	= 0.01f;	// カメラ注視位置の振幅.
	const float CAMERA_RETURN_COUNT_ADD		= 0.001f;
	const float CAMERA_RETURN_COUNT_MAX		= 0.1f;

public:
	CPlayer();
	virtual ~CPlayer();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override; 
	// エフェクト描画関数.
	virtual void EffectRender() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& actor ) override;
	// スプライトの描画.
	virtual void SpriteRender() override;

	// 特殊能力を使っているか.
	bool IsSpecialAbility();
	// 死亡したか.
	bool IsDead(){ return m_LifePoint <= 0.0f; }
	// カメラの方向.
	float GetCameraRadianX();

private:
	// 操作関数.
	void Controller();
	// カメラ操作.
	void CameraController();
	// 攻撃操作関数.
	void AttackController();
	// 特殊能力操作関数.
	void SPController();
	// 回避操作関数.
	void AvoidController();

	// 移動関数.
	virtual void Move() override;
	// 回避動作関数.
	void AvoidMove();

	// 目的の座標へ回転.
	bool TargetRotation( const D3DXVECTOR3& targetVec, const float& rotSpeed, const float& ToleranceRadian );

	// 攻撃の当たり判定.
	void AttackCollision( CActor* pActor );
	// 攻撃ヒット時のカメラ動作.
	void AttackHitCameraUpdate();
	// 特殊能力時のカメラ動作.
	void SPCameraUpdate();

	// 特殊能力回復更新関数.
	void SpecialAbilityUpdate();
	// 攻撃力UP更新関数.
	void AttackUpUpdate();
	// 移動速度UP更新関数.
	void MoveSpeedUpUpdate();
	// 麻痺中の更新関数.
	void ParalysisUpdate();

	// 攻撃アニメーション.
	void AttackAnimation();
	// アニメーション設定.
	void SetAnimation( const player::EAnimNo& animNo );
	void SetAnimationBlend( const player::EAnimNo& animNo );

	// 攻撃アニメーションフレームリストの設定.
	void SetAttackFrameList();
	// 攻撃の追加ができたか.
	bool IsPushAttack();

	// ライフ計算関数.
	virtual void LifeCalculation( const std::function<void(float&)>& ) override;
	// 特殊能力回復時間、効力時間設定関数.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ) override;
	// 攻撃力、効力時間設定関数.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ) override;
	// 移動速度、効力時間設定関数.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ) override;
	// 麻痺の設定.
	virtual void SetParalysisTime( const std::function<void(float&)>& ) override;

	// 当たり判定の設定.
	bool ColliderSetting();
	// エフェクトの設定.
	bool EffectSetting();

	// エディット用の描画関数.
	void EditRender();

	// サウンドの設定.
	bool SoundSetting();
	// 音量の設定.
	void VolumeSetting(const char* soung, float volume);
	// ウィジェット設定.
	bool WidgetSetting();

private:
	std::shared_ptr<CRotLookAtCenter>				m_pCamera;				// カメラクラス.
	std::shared_ptr<CCamera>						m_pSPCamera;			// 特殊能力カメラクラス.
	std::vector<std::shared_ptr<CCharacterWidget>>	m_pWidget;				// Widgetクラス.
	std::shared_ptr<CCollisionManager>				m_pAttackCollManager;	// 攻撃用の当たり判定.
	D3DXVECTOR3		m_OldPosition;			// 前回の座標.
	D3DXVECTOR3		m_GirlPosition;			// 女の子の座標.
	player::EAnimNo	m_NowAnimNo;			// 今のアニメーション番号.
	player::EAnimNo	m_OldAnimNo;			// 前のアニメーション番号.

	int								m_AttackComboCount;			// 攻撃コンボカウント.
	std::vector<double>				m_AttackEnabledFrameList;	// 攻撃有効フレームのリスト.
	std::queue<player::SAttackData>	m_AttackDataQueue;			// 攻撃データのキュー.
	D3DXVECTOR3						m_AttackPosition;			// 攻撃用当たり判定座標.

	std::vector<std::shared_ptr<CEffectManager>> m_pEffects;	// エフェクト.
	bool			m_IsDuringAvoid;	// 回避中かどうか.
	D3DXVECTOR3		m_AvoidVector;		// 回避ベクトル.

	SPlayerParam	m_Parameter;			// パラメーター.
	float			m_LifePoint;			// 体力.
	float			m_SpecialAbility;		// 特殊能力.
	bool			m_IsYButtonPressed;		// Yボタンが押されたか.
	bool			m_IsUsableSP;			// 特殊能力を使ったか.

	float			m_SpecialAbilityValue;		// 特殊能力回復力.
	float			m_ItemSpecialAbilityValue;	// アイテム特殊能力回復値.
	float			m_AttackPower;				// 攻撃力.
	float			m_MoveSpeed;				// 移動速度.
	float			m_MoveSpeedMulValue;		// 移動速度に掛け合わせる値.

	float			m_CameraDefaultHeight;		// カメラのデフォルト高さ.
	float			m_CameraHeight;				// カメラの高さ.


	D3DXVECTOR3		m_CameraNextPosition;		// カメラの座標.
	D3DXVECTOR3		m_CameraPosition;			// カメラの座標.
	D3DXVECTOR3		m_CameraLookPosition;		// カメラの視点座標.
	float			m_CameraCount;				// カメラのカウント.
	float			m_CameraReturnCount;		// カメラの戻るカウント.
	float			m_CameraLerp;				// カメラ移動の補間値.
	int				m_NowSPCameraStete;			// 特殊能力のカメラ状態.

	bool			m_IsAttackHitCamera;		// 攻撃ヒット時のカメラが有効か.
	float			m_CameraShakeCount;			// カメラの揺れカウント.
	float			m_CameraShakeTime;			// カメラの揺れ時間.
	float			m_CameraShakeCountAdd;		// カメラの揺れカウント加算値.
	std::vector<std::shared_ptr<CEffectTimer>>	m_pEffectTimers;	// 効果時間計測クラス.
	
	bool			m_IsAttackSE;				//攻撃SEを鳴らすか.

};

#endif	// #ifndef PLAYER_H.