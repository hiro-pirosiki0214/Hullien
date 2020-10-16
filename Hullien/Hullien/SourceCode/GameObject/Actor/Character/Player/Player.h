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
	const float CAMERA_BACK_DIRECTION_Z		= 10.0f;
	const float CAMERA_BACK_HEIGHT			= 8.0f;
	const float CAMERA_BACK_LERP_VALUE		= 0.1f;
	const float CAMERA_FREQUENCY_LOOKPOS	= 15.0f;	// カメラ注視位置の周波数.
	const float CAMERA_AMPLITUDE_LOOKPOS	= 0.01f;	// カメラ注視位置の振幅.
	const float CAMERA_RETURN_COUNT_ADD		= 0.001f;
	const float CAMERA_RETURN_COUNT_MAX		= 0.1f;

	// アニメーションの調整フレーム.
	const double ANIM_ADJ_FRAME_Wait	= 0.0;	// 待機.
	const double ANIM_ADJ_FRAME_Walk	= 0.0;	// 走り.
	const double ANIM_ADJ_FRAME_Attack1	= 0.5;	// 攻撃1.
	const double ANIM_ADJ_FRAME_Attack2	= 0.5;	// 攻撃2.
	const double ANIM_ADJ_FRAME_Attack3	= 0.5;	// 攻撃3.
	const double ANIM_ADJ_FRAME_Avoid	= 0.4;	// 回避.
	const double ANIM_ADJ_FRAME_SP		= 0.02;	// 特殊能力.
	const double ANIM_ADJ_FRAME_Damage	= 0.4;	// ヒット時.
	const double ANIM_ADJ_FRAME_Dead	= 0.01;	// 死亡.

	const double ATTACK1_ADJ_ENABLED_END_FRAME	= 0.0;	// 攻撃1の調整用有効フレーム.
	const double ATTACK2_ADJ_ENABLED_END_FRAME	= 0.0;	// 攻撃2の調整用有効フレーム.
	const double ATTACK3_ADJ_ENABLED_END_FRAME	= 0.0;	// 攻撃3の調整用有効フレーム.

	const double ATTACK1_ADJ_DRAGING_FRAME_START = 0.12;
	const double ATTACK1_ADJ_DRAGING_FRAME_END = 0.24;
	const float ATTACK1_ADJ_DRAGING_SPEED = 0.35f;

	const double ATTACK2_ADJ_DRAGING_FRAME_START = 0.01;
	const double ATTACK2_ADJ_DRAGING_FRAME_END = 0.2;
	const float ATTACK2_ADJ_DRAGING_SPEED = 0.3f;

	const double ATTACK3_ADJ_DRAGING_FRAME_START = 0.01;
	const double ATTACK3_ADJ_DRAGING_FRAME_END = 0.9;
	const float ATTACK3_ADJ_DRAGING_SPEED = 0.3f;

	const D3DXVECTOR3 ATTACK_COLLISION_INVALID_POS = { 0.0f, -10.0f, 0.0f };
	const float ATTACK1_COLLISION_RADIUS	= 6.0f;
	const float ATTACK2_COLLISION_RADIUS	= 7.0f;
	const float ATTACK3_COLLISION_RADIUS	= 8.0f;
	const float ATTACK_COLLISION_DISTANCE	= 4.0f;
	const float ATTACK_COLLISION_HEIGHT		= 4.0f;


	const double DEAD_CERTAIN_RANGE_ANIM_FRAME_MIN	= 0.18;		// 死亡アニメーションの一定範囲値の最小.
	const double DEAD_CERTAIN_RANGE_ANIM_FRAME_MAX	= 0.5;		// 死亡アニメーションの一定範囲値の最大.
	const float	DEAD_ANIM_DRAGING_ADJ_SPEED			= 0.05f;	// 死亡アニメーションの引きずりの調整速度.
	const float DAMAGE_HIT_KNOC_BACK_SPEED			= 0.3f;

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
	bool IsDead(){ return m_AnimFrameList[player::EAnimNo_Dead].IsNowFrameOver(); }
	// カメラの方向.
	float GetCameraRadianX();

	// ベクトルの取得.
	virtual void SetVector( const D3DXVECTOR3& vec ){ m_HitVector = vec; }

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

	// 移動関数(アニメーションとのずれの調整).
	virtual void Move() override;
	// 攻撃時の移動.
	void AttackMove();
	// 回避動作関数.
	void AvoidMove();
	// ノックバック動作関数.
	void KnockBack();
	// 死亡動作関数.
	void Dead();
	// カメラの更新.
	void CameraUpdate();

	// 攻撃の当たり判定.
	void AttackCollision( CActor* pActor );
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
	// 攻撃の追加ができたか.
	bool IsPushAttack();

	// ライフ計算関数.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ) override;
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
	// アニメーションフレームの設定.
	bool SetAnimFrameList();

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
	std::shared_ptr<CCollisionManager>				m_pAttackCollManager;	// 攻撃用の当たり判定.
	std::vector<std::shared_ptr<CEffectManager>>	m_pEffects;				// エフェクト.
	std::vector<std::shared_ptr<CEffectTimer>>		m_pEffectTimers;		// 効果時間計測クラス.
	std::vector<std::shared_ptr<CCharacterWidget>>	m_pWidget;				// Widgetクラス.

	int								m_AttackComboCount;			// 攻撃コンボカウント.
	std::queue<player::SAttackData>	m_AttackDataQueue;			// 攻撃データのキュー.
	D3DXVECTOR3						m_AttackPosition;			// 攻撃用当たり判定座標.

	D3DXVECTOR3		m_GirlPosition;		// 女の子の座標.
	D3DXVECTOR3		m_AvoidVector;		// 回避ベクトル.
	D3DXVECTOR3		m_HitVector;		// 衝突時のベクトル.
	D3DXVECTOR3		m_TargetVector;		// 目的のベクトル.

	SPlayerParam	m_Parameter;			// パラメーター.
	float			m_LifePoint;			// 体力.
	float			m_SpecialAbility;		// 特殊能力.
	bool			m_IsDuringAvoid;		// 回避中かどうか.
	bool			m_IsYButtonPressed;		// Yボタンが押されたか.
	bool			m_IsUsableSP;			// 特殊能力を使ったか.
	bool			m_IsKnockBack;			// ノックバックするか.
	bool			m_IsDead;				// 死亡フラグ.

	float			m_SpecialAbilityValue;		// 特殊能力回復力.
	float			m_ItemSpecialAbilityValue;	// アイテム特殊能力回復値.
	float			m_AttackPower;				// 攻撃力.
	float			m_MoveSpeed;				// 移動速度.
	float			m_MoveSpeedMulValue;		// 移動速度に掛け合わせる値.

	D3DXVECTOR3		m_CameraNextPosition;		// カメラの座標.
	D3DXVECTOR3		m_CameraPosition;			// カメラの座標.
	D3DXVECTOR3		m_CameraLookPosition;		// カメラの視点座標.
	float			m_CameraCount;				// カメラのカウント.
	float			m_CameraReturnCount;		// カメラの戻るカウント.
	float			m_CameraLerp;				// カメラ移動の補間値.
	int				m_NowSPCameraStete;			// 特殊能力のカメラ状態.
	
	bool			m_IsAttackSE;				//攻撃SEを鳴らすか.
};

#endif	// #ifndef PLAYER_H.