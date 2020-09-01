#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"
#include <queue>

class CRotLookAtCenter;
class CCharacterWidget;

class CPlayer : public CCharacter
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME	= "kaito_s";	// モデル名.
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
	};

	// プレイヤーパラメータ.
	struct stPlayerParam : public stParameter
	{
		float		SpecialAbilityMax;	// 特殊能力最大値.
		float		SpecialAbilityValue;// 特殊能力回復値.
		int			AttackComboMax;		// 攻撃の最大数.
		int			AttackQueueMax;		// 攻撃キューの最大数.
		float		AvoidMoveDistance;	// 回避の移動距離.
		float		AvoidMoveSpeed;		// 回避用の移動速度.
		float		CameraMoveSpeed;	// カメラの移動速度.
		float		CameraDistance;		// カメラの距離.
		float		CameraHeight;		// カメラの高さ.
		D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
		float		SphereAdjRadius;	// スフィアの調整半径.

		stPlayerParam()
			: SpecialAbilityMax	( 0.0f )
			, AttackComboMax	( 0 )
			, AttackQueueMax	( 0 )
			, AvoidMoveDistance	( 0.0f )
			, AvoidMoveSpeed	( 0.0f )
			, CameraMoveSpeed	( 0.01f )
			, CameraDistance	( 7.0f )
			, CameraHeight		( 4.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius	( 0.0f )
		{}
	} typedef SPlayerParam;

	// 攻撃用データ.
	struct stAttackData
	{
		enAnimNo	AnimNo;				// アニメーション番号.
		double		Frame;				// 経過フレーム.
		double		EnabledEndFrame;	// 有効終了フレーム.
		double		EndFrame;			// 終了フレーム.
		stAttackData()
			: AnimNo			( enAnimNo::None )
			, Frame				( 0.0 )
			, EnabledEndFrame	( 0.0 )
			, EndFrame			( 0.0 )
		{}
	} typedef SAttackData;

	// アイテムや、麻痺などのタイマー.
	struct stEffectTimer
	{
		bool	IsUpdate;	// 更新中か.
		float	Time;		// 時間.

		stEffectTimer()
			: IsUpdate	( false )
			, Time		( 0.0f )
		{}

		// タイマーの設定.
		void Set()
		{
			Time *= FPS;
			IsUpdate = true;
		}

		// 更新.
		bool Update()
		{
			if( IsUpdate == false ) return false;
			Time--;

			if( Time > 0.0f ) return false;
			IsUpdate = false;
			return true;
		}
	} typedef SEffectTimer;

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
	void SetAnimation( const enAnimNo& animNo );

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

	// エディット用の描画関数.
	void EditRender();
	// デバッグ用の描画.
	void DebugRender();

	// ウィジェット設定.
	bool WidgetSetting();

private:
	std::shared_ptr<CRotLookAtCenter>				m_pCamera;		// カメラクラス.
	std::vector<std::shared_ptr<CCharacterWidget>>	m_pWidget;		// Widgetクラス.
	D3DXVECTOR3	m_OldPosition;			// 前回の座標.
	enAnimNo	m_NowAnimNo;			// 今のアニメーション番号.
	enAnimNo	m_OldAnimNo;			// 前のアニメーション番号.
	int			m_AttackComboCount;					// 攻撃コンボカウント.
	std::vector<double>	m_AttackEnabledFrameList;	// 攻撃有効フレームのリスト.
	std::queue<SAttackData>	m_AttackDataQueue;		// 攻撃データのキュー.
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

	SEffectTimer	m_ItemSPRecoveryTimer;	// アイテムでの特殊能力回復タイマー.
	SEffectTimer	m_ItemAttackTimer;		// アイテムでの攻撃力UPタイマー.
	SEffectTimer	m_ItemMoveSpeedUpTimer;	// アイテムでの移動速度UPタイマー.
	SEffectTimer	m_ParalysisTimer;		// 麻痺タイマー.
};

#endif	// #ifndef PLAYER_H.