#ifndef GIRL_H
#define GIRL_H

#include "..\Character.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\Warning\Warning.h"

class CGirl : public CCharacter
{
	const char* MODEL_NAME = "Towa_s";	// モデル名.
	const char* MODEL_TEMP_NAME = "yuri-dy";	// 仮モデル名.

	// プレイヤーパラメータ.
	struct stGirlParam : public stParameter
	{
		float		SearchCollRadius;	// 索敵用のスフィアの半径.
		D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
		float		SphereAdjRadius;	// スフィアの調整半径.

		stGirlParam()
			: SearchCollRadius	( 10.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius	( 0.0f )
		{}
	} typedef SGirlParam;

	// 現在の状態.
	enum class enNowState
	{
		None,

		Protected,	// 守られている.
		Abduct,		// 連れ去れている.
		Move,		// 移動.

		Max,

	} typedef ENowState;

	// 移動状態.
	enum class enMoveState
	{
		None,

		Rotation,	// 回転.
		Move,		// 移動.
		Wait,		// 待機.

		Max,
	} typedef EMoveState;
public:
	CGirl();
	~CGirl();

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
	// 危険な状態か.
	bool IsDanger(){ return m_IsDanger; }
	// カメラのラジアン値の設定関数.
	void SetCameraRadianX(const float& radian) { m_CameraRadianX = radian; }

private:
	// 移動関数.
	virtual void Move() override;
	// 目的の場所に向けて回転.
	void TargetRotation();
	// 目的の場所に向けて移動.
	void TargetMove();

	// 索敵の当たり判定.
	void SearchCollision( CActor* pActor );

	// 当たり判定の作成.
	bool  ColliderSetting();

	// サウンド.
	void Sound();

	// 危険矢印の回転.
	void WarningRotation();
	// デバッグ描画関数.
	void DebugRender();

private:
	SGirlParam m_Parameter;	// 女の子のパラメーター.
	std::shared_ptr<CCollisionManager>	m_pSearchCollManager;	// 索敵用の当たり判定.
	std::unique_ptr<CWarning>					m_pWarning;						// 警告クラス.
	D3DXVECTOR3	m_OldPosition;	// 前回の座標.
	ENowState	m_NowState;		// 現在の状態.
	EMoveState	m_NowMoveState;	// 現在の移動状態.
	float		m_CameraRadianX;// カメラのラジアン値.
	bool		m_IsDanger;		// 危険かどうか.
	bool		m_IsOnlyFirst;	//一度しか処理しない.
};

#endif	// #ifndef GIRL_H.