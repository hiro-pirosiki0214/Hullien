#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"
#include "PlayerParam.h"
#include <queue>

class CRotLookAtCenter;
class CCharacterWidget;
class CEffectManager;
class CEffectTimer;

#define INTERMEDIATE_ANNOUCEMENT_ATTACK // 中間発表用の攻撃.

class CPlayer : public CCharacter
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME			= "Sayaka_s";	// モデル名.
	const char* MODEL_TEMP_NAME		= "kaito";		// 仮モデル名.

	// 使用攻撃エフェクト名.
	const char* ATTACK_ONE_EFFECT_NAME		= "barrier";
	const char* ATTACK_TWO_EFFECT_NAME		= "uvtest";
	const char* ATTACK_THREE_EFFECT_NAME	= "uvtest";

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

	// 攻撃番号,
	enum enAttackNo
	{
		EAttackNo_None,

		EAttackNo_One,
		EAttackNo_Two, 
		EAttackNo_Three,

		EAttackNo_Max = EAttackNo_Three,
	} typedef EAttackNo;

	// 効果時間計測番号.
	enum enEffectTimerNo
	{
		EEffectTimerNo_None,

		EEffectTimerNo_SPRecovery = 0,	// 特殊能力回復.
		EEffectTimerNo_Attack,			// 攻撃力.
		EEffectTimerNo_MoveSpeedUp,		// 移動速度.
		EEffectTimerNo_Paralysis,		// 麻痺.

		EEffectTimerNo_Max,
	} typedef EEffectTimerNo;

	// 攻撃用データ.
	struct stAttackData
	{
		EAnimNo		AnimNo;				// アニメーション番号.
		double		Frame;				// 経過フレーム.
		double		EnabledEndFrame;	// 有効終了フレーム.
		double		EndFrame;			// 終了フレーム.

		stAttackData()
			: AnimNo			( EAnimNo::None )
			, Frame				( 0.0 )
			, EnabledEndFrame	( 1.0 )
			, EndFrame			( 2.0 )
		{}
	} typedef SAttackData;

public:
	CPlayer();
	virtual ~CPlayer();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override; 
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

	// エフェクト描画関数.
	void EffectRender();

	// 攻撃の当たり判定.
	void AttackCollision( CActor* pActor );

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
	void SetAnimation( const EAnimNo& animNo );

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
	// デバッグ用の描画.
	void DebugRender();

	// ウィジェット設定.
	bool WidgetSetting();

private:
	std::shared_ptr<CRotLookAtCenter>				m_pCamera;		// カメラクラス.
	std::vector<std::shared_ptr<CCharacterWidget>>	m_pWidget;		// Widgetクラス.
	std::shared_ptr<CCollisionManager>				m_pAttackCollManager;	// 攻撃用の当たり判定.
	D3DXVECTOR3	m_OldPosition;		// 前回の座標.
	EAnimNo	m_NowAnimNo;			// 今のアニメーション番号.
	EAnimNo	m_OldAnimNo;			// 前のアニメーション番号.
	int						m_AttackComboCount;			// 攻撃コンボカウント.
	std::vector<double>		m_AttackEnabledFrameList;	// 攻撃有効フレームのリスト.
	std::queue<SAttackData>	m_AttackDataQueue;			// 攻撃データのキュー.
	D3DXVECTOR3				m_AttackPosition;			// 攻撃用当たり判定座標.
	std::vector<std::shared_ptr<CEffectManager>> m_pEffects;	// エフェクト.
	bool			m_IsDuringAvoid;	// 回避中かどうか.
	D3DXVECTOR3		m_AvoidVector;		// 回避ベクトル.

	SPlayerParam	m_Parameter;			// パラメーター.
	float			m_LifePoint;			// 体力.
	float			m_SpecialAbility;		// 特殊能力.
	bool			m_HasUsableSP;			// 特殊能力を使えるか.

	float			m_SpecialAbilityValue;		// 特殊能力回復力.
	float			m_ItemSpecialAbilityValue;	// アイテム特殊能力回復値.
	float			m_AttackPower;				// 攻撃力.
	float			m_MoveSpeed;				// 移動速度.

	std::vector<std::shared_ptr<CEffectTimer>>	m_pEffectTimers;	// 効果時間計測クラス.
	
	bool				m_IsAttackSE;				//攻撃SEを鳴らすか.
};

#endif	// #ifndef PLAYER_H.