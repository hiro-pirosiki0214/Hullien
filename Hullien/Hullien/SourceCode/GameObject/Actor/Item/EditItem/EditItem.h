#ifndef EDIT_ITME_H
#define EDIT_ITME_H

#include "..\ItemBase.h"

class CEditItem : public CItemBase
{
	const char* CURE_MODEL_NAME		= "item cure";		// モデル名.
	const char* ABILITY_MODEL_NAME	= "item ability";	// モデル名.
	const char* ATTACK_MODEL_NAME	= "item attack";	// モデル名.
	const char* SPEED_MODEL_NAME	= "item speed";		// モデル名.

	const char* CURE_EFFECT_NAME	= "item_effect_cure";		// エフェクト名.
	const char* ABILITY_EFFECT_NAME	= "item_effect_ability";	// エフェクト名.
	const char* ATTACK_EFFECT_NAME	= "item_effect_attack";		// エフェクト名.
	const char* SPEED_EFFECT_NAME	= "item_effect_speed";		// エフェクト名.
public:
	CEditItem();
	virtual ~CEditItem();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// エフェクトの描画.
	virtual void EffectRender() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& pActor ) override;

	// 出現処理.
	virtual void Drop( const D3DXVECTOR3& vPos ) override;

	// パラメータの設定.
	void SetParamter( const SParameter& param );
	// アイテム番号の設定.
	inline void SetItemNo( const int& no ) { m_NowItemNo = no; }

private:
	// モデルの設定.
	bool ModelSetting();
	// 当たった際の効果を与える.
	virtual void GiveEffect( CActor* pActor ) override{};
	// エフェクトの設定.
	virtual bool EffectSetting() override;
	// 当たり判定の設定.
	virtual bool ColliderSetting() override;

private:
	std::vector<std::shared_ptr<CDX9StaticMesh>>	m_pStaticMeshs;
	SParameter	m_Parameter;
	int			m_NowItemNo;
	float		m_RotatonY;
};

#endif	// #ifndef EDIT_ITME_H.
