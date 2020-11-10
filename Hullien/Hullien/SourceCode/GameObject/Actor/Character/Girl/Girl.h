#ifndef GIRL_H
#define GIRL_H

#include "..\Character.h"
#include "GirlParam.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\Warning\Warning.h"

class CGirl : public CCharacter
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Girl\\Girl.bin";
	const char* MODEL_NAME			= "yuri-dy_s";	// モデル名.
	const float TOLERANCE_RADIAN		= static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.
	const float THRESHOLD_VALUE_RADIAN	= static_cast<float>(D3DXToRadian(140.0));	// しきい値.


	enum enAnimNo
	{
		EAnimNo_None = -1,

		EAnimNo_Wait,	// 待機.
		EAnimNo_Move,	// 移動.
		EAnimNo_Abduct,	// 連れ去られている.

		EAnimNo_Max,

		EAnimNo_Begin	= EAnimNo_Wait,
		EAnimNo_End		= EAnimNo_Max,

		Abduct_Max,
	} typedef EAnimNo;

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
	virtual void SetPosition( const D3DXVECTOR3& pos ) override;
	// スプライトの描画.
	virtual void SpriteRender() override;
	// 危険な状態か.
	inline bool IsDanger(){ return m_IsDanger; }
	// カメラのラジアン値の設定関数.
	inline void SetCameraRadianX( const float& radian ) { m_CameraRadianX = radian; }

private:
	// 移動関数.
	virtual void Move() override;
	// 連れ去れている.
	void Abduct();
	// 目的の場所に向けて回転.
	void TargetRotation();
	// 目的の場所に向けて移動.
	void TargetMove();

	// 索敵の当たり判定.
	void SearchCollision( CActor* pActor );

	// 当たり判定の作成.
	bool ColliderSetting();

	// アニメーションフレームの設定.
	virtual bool SetAnimFrameList() override;

	// サウンド.
	void Sound();

	// 危険矢印の回転.
	void WarningRotation();
	// デバッグ描画関数.
	void DebugRender();

private:
	SGirlParam m_Parameter;	// 女の子のパラメーター.
	std::shared_ptr<CCollisionManager>	m_pSearchCollManager;	// 索敵用の当たり判定.
	std::unique_ptr<CWarning>			m_pWarning;				// 警告クラス.
	D3DXVECTOR3	m_OldPosition;	// 前回の座標.
	ENowState	m_NowState;		// 現在の状態.
	EMoveState	m_NowMoveState;	// 現在の移動状態.
	float		m_CameraRadianX;// カメラのラジアン値.
	bool		m_IsDanger;		// 危険かどうか.
	bool		m_IsOnlyFirst;	//一度しか処理しない.
};

#endif	// #ifndef GIRL_H.