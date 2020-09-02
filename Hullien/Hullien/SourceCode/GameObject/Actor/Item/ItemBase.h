#ifndef ITEM_BASE_H
#define ITEM_BASE_H

#include "..\Actor.h"

class CEffectManager;	// エフェクトクラス.

class CItemBase : public CActor
{
	const float INIT_MODEL_ALPHA			= 1.0f;		// 初期モデル透過値.
	const float DEFAULT_ACCELERATION_VALUE	= -0.05f;	// デフォルト加速値.
	const float DEFAULT_GRAVITY				= 0.001f;	// デフォルト重力.
public:
	// パラメータ.
	struct stParameter
	{
		float	InitAccelerationValue;	// 初期加速値.
		float	InitGravity;			// 初期重力.

		int		BoundCountMax;			// バウンド最大数.
		float	FinalRenderHeight;		// 最終的な描画の高さ.
		float	FinalMoveSpeed;			// 最終的な移動速度.
		float	ModelScaleMax;			// モデルサイズ最大.
		float	ModelScaleAddValue;		// モデルサイズ加算値.
		float	RotationSpeed;			// 回転速度.
		float	ActiveTime;				// アクティブ時間.
		float	DisappearTime;			// 消える時間.
		float	FlashingAddValue;		// 点滅加算値.
		float	ModelAlphaMax;			// モデル透過値最大.
		float	HitEffectTime;			// ヒット時のエフェクト時間.
		stParameter()
			: InitAccelerationValue	( 0.0f )
			, InitGravity			( 0.0f )
			, BoundCountMax			( 0 )
			, FinalRenderHeight		( 0.0f )
			, FinalMoveSpeed		( 0.0f )
			, ModelScaleMax			( 0.0f )
			, ModelScaleAddValue	( 0.0f )
			, RotationSpeed			( 0.0f )
			, ActiveTime			( 0.0f )
			, DisappearTime			( 0.0f )
			, FlashingAddValue		( 0.0f )
			, ModelAlphaMax			( 0.0f )
			, HitEffectTime			( 0.0f )
		{}
	} typedef SParameter;

private:
	const SParameter* pPRAMETER;	// パラメータ(外部からポインタで取得する).

protected:
	// 現在の状態.
	enum class enNowState
	{
		None,

		Drop,			// 出現.
		Active,			// アクティブ.
		HitDisappear,	// 当たった際に消える.
		TimeDisappear,	// 時間が経った時に消える.
		Delete,			// 消えた.

		Max,
	} typedef ENowState;

	// 使用するエフェクト番号.
	enum class enEffectNumber
	{
		None = -1,

		DropAndActive,	// ドロップ アクティブ時.
		Hit,			// 当たった時.

		Max,	// 最大.
	} typedef EEffectNumber;

public:
	CItemBase();
	CItemBase( const SParameter* pParam );
	virtual ~CItemBase();

	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// 出現処理.
	void Drop( const D3DXVECTOR3& vPos );

	// 消えたかどうか.
	bool IsDelete(){ return m_NowState == ENowState::Delete; }

protected:
	// 当たった際の効果を与える.
	virtual void GiveEffect( CActor* pActor ) = 0;
	// 出現.
	void Drop();
	// アクティブ.
	void Active();
	// 当たった際に消える処理.
	void HitDisappear();
	// 時間が経った特に消える.
	void TimeDisappear();

	// 回転関数.
	void Rotate();

	// エフェクトの設定.
	virtual bool EffectSetting() = 0;
	// モデルの取得.
	bool GetModel( const char* modelName );
	// 当たり判定の設定.
	bool ColliderSetting();

private:
	// ドロップ　アクティブ時の描画.
	void DropAndActiveRender();
	// ヒット時の描画.
	void HitRender();

protected:
	std::shared_ptr<CDX9StaticMesh> m_pStaticMesh;	// スタティックメッシュ.
	std::vector<std::shared_ptr<CEffectManager>> m_pEffects; // エフェクト.
	ENowState m_NowState;	// 現在の状態.
	float m_Scale;
	float m_ModelAlpha;

	float m_AccelerationValue;	// 加速値.
	float m_Gravity;			// 重力.
	int m_BoundCount;			// バウンド数.

	int m_ActiveCount;		// アクティブカウント.

	float m_FlashingCount;	// 点滅カウント.
	float m_DisappearCount;	// 消える時間.
	float m_FlashingAccValue;	// 点滅加速値.

	float m_HitEffectCount;	// ヒット時のエフェクトカウント.
};

#endif	// #ifndef ITEM_BASE_H.