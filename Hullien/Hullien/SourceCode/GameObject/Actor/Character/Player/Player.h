#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"
#include <queue>

class CRotLookAtCenter;

class CPlayer : public CCharacter
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME	= "Sayaka_s";		// モデル名.
	const char* MODEL_TEMP_NAME = "kaitosize";	// 仮モデル名.

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
			: AttackComboMax	( 0 )
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

private:
	// 操作関数.
	void Controller();
	// カメラ操作.
	void CameraController();
	// 攻撃操作関数.
	void AttackController();
	// 回避操作関数.
	void AvoidController();

	// 移動関数.
	virtual void Move() override;
	// 回避動作関数.
	void AvoidMove();

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
	// 麻痺の設定.
	virtual void SetParalysisTime( const std::function<void(float&)>& ) override;

	// 当たり判定の設定.
	bool ColliderSetting();

	// エディット用の描画関数.
	void EditRender();
	// デバッグ用の描画.
	void DebugRender();

private:
	std::shared_ptr<CRotLookAtCenter>	m_pCamera;		// カメラクラス.
	D3DXVECTOR3	m_OldPosition;			// 前回の座標.
	enAnimNo	m_NowAnimNo;			// 今のアニメーション番号.
	enAnimNo	m_OldAnimNo;			// 前のアニメーション番号.
	int			m_AttackComboCount;					// 攻撃コンボカウント.
	std::vector<double>	m_AttackEnabledFrameList;	// 攻撃有効フレームのリスト.
	std::queue<SAttackData>	m_AttackDataQueue;		// 攻撃データのキュー.
	bool			m_IsDuringAvoid;	// 回避中かどうか.
	D3DXVECTOR3		m_AvoidVector;		// 回避ベクトル.
	float			m_AvoidMoveSpeed;	// 回避用移動速度.

	bool			m_IsParalysis;		// 麻痺中か.
	float			m_ParalysisTime;	// 麻痺時間.

	SPlayerParam	m_Parameter;		// パラメーター.
};

#endif	// #ifndef PLAYER_H.